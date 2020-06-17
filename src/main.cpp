#include <Arduino.h>

#include <Ticker.h>
#include <ESP8266WiFi.h>
#include <MAX4466.h>

#include <NeoPixelAnimator.h>
#include <NeoPixelBrightnessBus.h>

#include <running_avg.h>

template< typename T, size_t N >
constexpr size_t count_of( const T (&)[N] )
{ return N; }

template< typename T, typename U>
static bool checkbit(T const &var, U pos)
{ return !!(var & (1 << pos)); }

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

static const uint8_t PanelWidth = 8;
static const uint8_t PanelHeight = 8;
static const uint16_t PixelCount = PanelWidth * PanelHeight;
static const uint8_t PixelPin = D9;  // RX
static const RgbColor black(0);
static const RgbColor white(255);

static NeoTopology<ColumnMajorAlternatingLayout> topo(COLUMNS, ROWS);
static NeoPixelBrightnessBus<NeoGrbFeature, NeoEsp8266Dma800KbpsMethod> strip(PixelCount, PixelPin);

static const PROGMEM uint8_t mouth[][8] = {
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

  return idx;
}

static void ShowFace(unsigned idx)
{
  strip.ClearTo(black);
  idx = min(idx, count_of(mouth) - 1);
  for (unsigned row = 0; row < 8; row++) {
    uint8_t this_row = pgm_read_byte(&mouth[idx][row]);
    for (unsigned col = 0; col < 8; col++) {
      if (checkbit(this_row, col))
        strip.SetPixelColor(topo.Map(7 - col, row), white);
    }
  }
}

#define DISPLAY_DELAY   (80)    // 12.5 fps
#define SAMPLES         (7)
#define SAMPLE_DELAY    ((DISPLAY_DELAY * 3) / (2 * SAMPLES))

static Running_avg<SAMPLES, unsigned> percent;

static void sound_sample()
{
  percent += MAX4466.scaled_loudness<99>();
}

static void display_update()
{
  unsigned idx = SetState(percent);
  ShowFace(idx);
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
  strip.ClearTo(black);
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
