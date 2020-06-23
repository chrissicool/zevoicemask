#include <Arduino.h>

#include <Ticker.h>
#include <ESP8266WiFi.h>
#include <MAX4466.h>

#include <NeoPixelAnimator.h>
#include <NeoPixelBrightnessBus.h>

#include <running_avg.h>
#include <animations.h>

template< typename T, size_t N >
constexpr size_t count_of( const T (&)[N] )
{ return N; }

template< typename T, typename U>
static inline bool is_set(T const &var, U pos)
{ return !!(var & (1 << pos)); }

unsigned long long operator"" _millis(unsigned long long x) {
  return x;
}
unsigned long long operator"" _secs(unsigned long long x) {
  return x * 1000_millis;
}

static void init_serial()
{
#ifdef VOICEMASK_DEBUG
  Serial.begin(115200);
  while (!Serial)
    ;
  delay(500);
  Serial.println();
  Serial.println( F("~~~ ESP8266 ~~~") );
  Serial.print( F("Boot Vers: ") ); Serial.println(ESP.getBootVersion());
  Serial.print( F("Vcc: ") ); Serial.println(ESP.getVcc());
  Serial.print( F("Chip ID: ") ); Serial.println(ESP.getChipId());
  Serial.print( F("Heap: ") ); Serial.println(ESP.getFreeHeap());
  Serial.print( F("CPU: ") ); Serial.println(ESP.getCpuFreqMHz());
  Serial.print( F("SDK: ") ); Serial.println(ESP.getSdkVersion());
  Serial.print( F("Flash Size: ") ); Serial.println(ESP.getFlashChipSize());
  Serial.println();
#endif
}

static void init_random()
{
  uint32_t seed;

  // random works best with a seed that can use 31 bits
  // analogRead on an unconnected pin tends toward less than four bits
  seed = analogRead(0);
  delayMicroseconds(100);

  for (int shifts = 3; shifts < 31; shifts += 3)
  {
    seed ^= analogRead(0) << shifts;
    delayMicroseconds(100);
  }

  randomSeed(seed);
}

#ifdef ARDUINO_ARCH_ESP8266
/* This function is not available in ESP8266 framework. */
static void ets_waiti0(void)
{
   __asm __volatile__ ("waiti 0" : : : "memory");
}
#endif
static inline void halt() { ets_waiti0(); }


static HardwareMAX4466<10> MAX4466;
static Ticker display;
static Ticker sampler;
static Ticker bored;

static const uint16_t PixelCount = COLUMNS * ROWS;
static const uint8_t PixelPin = D9;  // RX

static NeoTopology<ColumnMajorAlternatingLayout> topo(COLUMNS, ROWS);
static NeoPixelBrightnessBus<NeoGrbFeature, NeoEsp8266Dma800KbpsMethod> strip(PixelCount, PixelPin);

static Animations animations;

static BwFrames(6) mouth = {
{
  B00000000,
  B00000000,
  B00000000,
  B10000001,
  B11111111,
  B01111110,
  B00000000,
  B00000000
},{
  B00000000,
  B00000000,
  B00000000,
  B11111111,
  B10000001,
  B01111110,
  B00000000,
  B00000000
},{
  B00000000,
  B00000000,
  B01111110,
  B10000001,
  B10000001,
  B01000010,
  B00111100,
  B00000000
},{
  B00000000,
  B00111100,
  B01000010,
  B10000001,
  B10000001,
  B01000010,
  B00111100,
  B00000000
},{
  B00000000,
  B00111100,
  B01000010,
  B10000001,
  B10000001,
  B10000001,
  B01000010,
  B00111100
},{
  B00111100,
  B01000010,
  B10000001,
  B10000001,
  B10000001,
  B10000001,
  B01000010,
  B00111100
}};

enum FaceState
{
  Smiling,   // amplitude too low; mount[0] state.
  Talking,   // amplitude upto count_of(mouth).
  Screaming, // amplitude higher than count_of(mouth).
};

static FaceState state = Smiling;

#define DISPLAY_DELAY   (80)    // 12.5 fps
#define SAMPLES         (7)
#define SAMPLE_DELAY    ((DISPLAY_DELAY * 3) / (2 * SAMPLES))

static void display_update();

static void boring()
{
  Animation *anim = animations.random();

  /* XXX This is blocking */
  display.detach();

#ifdef VOICEMASK_DEBUG
  Serial.print( F("I am bored, show me a ") ); Serial.println(anim->name());
#endif
  anim->init();
  for (unsigned i = 0; i < anim->frames(); ++i) {
    anim->frame(i);
    strip.Show();
    delay(DISPLAY_DELAY);
  }
  anim->deinit();

  bored.detach();
  display.attach_ms(DISPLAY_DELAY, display_update);
}

static unsigned SetState(unsigned percent)
{
  unsigned idx;
  enum {
    Talk_range_min = 10,
    Talk_range_max = 85,
    Talk_range = Talk_range_max - Talk_range_min,
  };
  switch (percent) {
  case 0 ... Talk_range_min:
    state = Smiling;
    idx = 0;
    break;
  case Talk_range_min + 1 ... Talk_range_max:
    state = Talking;
    idx = map(percent, Talk_range_min + 1, Talk_range_max, 1, count_of(mouth) - 2);
    break;
  default:
    state = Screaming;
    idx = count_of(mouth) - 1;
    break;
  }

  if (state == Smiling) {
    if (!bored.active())
      bored.once_ms_scheduled(5_secs + random(20_secs), boring);
  } else {
    bored.detach();
  }

  return idx;
}

void ClearFrame()
{ strip.ClearTo(0); }

void ShowPixel(unsigned col, unsigned row, const RgbColor &c)
{ strip.SetPixelColor(topo.Map(col, row), c); }

RgbColor const GetPixel(unsigned col, unsigned row)
{ return strip.GetPixelColor(topo.Map(col, row)); }

void ShowFrame(ColorFrames(1) const &frame)
{
  for (unsigned row = 0; row < ROWS; row++) {
    for (unsigned col = 0; col < COLUMNS; col++) {
      const RgbColor c = frame[row][col];
      ShowPixel(col, row, c);
    }
  }
}

void ShowFrame(BwFrames(1) const &frame, const RgbColor &c)
{
  for (unsigned row = 0; row < ROWS; row++) {
    auto this_row = frame[row];
    for (unsigned col = 0; col < COLUMNS; col++) {
      if (is_set(this_row, col))
        ShowPixel(COLUMNS - 1 - col, row, c);
    }
  }
}

static void ShowMouth(unsigned idx)
{
  const RgbColor white(255);

  ClearFrame();
  ShowFrame(mouth[idx], white);
}

void ShowMouthSmiling()
{ ShowMouth(0); }

static Running_avg<SAMPLES, unsigned> percent;

static void sound_sample()
{
  percent += MAX4466.scaled_loudness<99>();
}

static void display_update()
{
  static unsigned idx;

  idx = SetState(percent);
  ShowMouth(idx);
  strip.Show();

#ifdef VOICEMASK_DEBUG
  Serial.print(F("ADC (raw): ")); Serial.print(analogRead(A0));
  Serial.print(F(" --> percent (avg): ")); Serial.print(percent);
  Serial.print(F(" --> idx: ")); Serial.println(idx);
#endif
}

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  init_serial();

  strip.Begin();
  strip.ClearTo(0);
  strip.Show();

  WiFi.mode(WIFI_OFF);
  WiFi.forceSleepBegin();
  yield();

  init_random();
  MAX4466.setup(A0);

#ifdef VOICEMASK_DEBUG
  Serial.print( F("Display delay: ") ); Serial.println(DISPLAY_DELAY);
  Serial.print( F("Sample delay: ") ); Serial.println(SAMPLE_DELAY);
  Serial.println();
#endif
  static_assert(SAMPLE_DELAY > 0, "SAMPLE_DELAY is too tight.");
  static_assert(SAMPLE_DELAY < DISPLAY_DELAY, "DISPLAY_DELAY is too low.");
  sampler.attach_ms(SAMPLE_DELAY, sound_sample);
  display.attach_ms(DISPLAY_DELAY, display_update);

  digitalWrite(LED_BUILTIN, HIGH);
}

void loop()
{
  halt();
}
