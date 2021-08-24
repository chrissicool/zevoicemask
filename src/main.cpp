#include <Arduino.h>
extern "C" {
#include <user_interface.h>
}

#include <Ticker.h>
#include <MAX4466.h>

#include <NeoPixelAnimator.h>
#include <NeoPixelBrightnessBus.h>

#include <running_avg.h>
#include <animations.h>
#include <effects.h>
#include <frame.h>

//#define TESTMODE VOICE

template< typename T, size_t N >
constexpr size_t count_of( const T (&)[N] )
{ return N; }

template< typename T, typename U>
static inline bool is_set(T const &var, U pos)
{ return !!(var & (1 << pos)); }

constexpr unsigned short
operator"" _millis(const unsigned long long x) {
  return x;
}
constexpr unsigned short
operator"" _secs(const unsigned long long x) {
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
static Effects effects;

static const constexpr BwFrames(6) mouth PROGMEM = {
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


/*
 * The maximum delay is ~65_secs. It overflows otherwise.
 * The shortest delay must be smaller than 100_millis,
 * otherwise the onboard WDT kills us.
 */
#define MAX_DELAY       (USHRT_MAX)
#define MIN_DELAY       (100_millis)

#define DISPLAY_DELAY   (80_millis)    // 12.5 fps
#define SAMPLES         (3)
#define SAMPLE_DELAY    ((DISPLAY_DELAY * 3) / (2 * SAMPLES))
#define BORED_MIN_DELAY (5_secs)
#define BORED_MAX_DELAY (25_secs)

static void boring()
{
  animations.choose();
  effects.choose();

#ifdef VOICEMASK_DEBUG
  Serial.print( F("I am bored, show me a ") );
  Serial.print(animations.name());
  Serial.print( F(", then apply ") );
  Serial.println(effects.name());
#endif
  bored.detach();
}

static unsigned SetState(unsigned percent)
{
  unsigned idx, inc;

  enum {
    Talk_range_min = 10,
    Talk_range_max = 85,
    Talk_range = Talk_range_max - Talk_range_min,
  };

  switch (percent) {
  case 0 ... Talk_range_min:
    idx = 0;
    break;
  case Talk_range_min + 1 ... Talk_range_max:
    /* decibel scale */
    idx = 1;
    inc = Talk_range / 2;
    percent -= Talk_range_min;
    while (percent > inc) {
      idx++;
      percent -= inc;
      inc /= 2;
      if (idx == count_of(mouth) - 2)
        break;
    }
    break;
  default:
    idx = count_of(mouth) - 1;
    break;
  }

  return idx;
}

static void IRAM_ATTR isr()
{
  bored.detach();
  bored.once_ms(0, boring);
}

void ClearFrame()
{ strip.ClearTo(0); }

void ShowPixel(unsigned col, unsigned row, const RgbColor &c)
{ strip.SetPixelColor(topo.Map(col, row), c); }

RgbColor const GetPixel(unsigned col, unsigned row)
{ return strip.GetPixelColor(topo.Map(col, row)); }

void PGM_ShowFrame(ColorFrames(1) const &frame)
{
  ColorFrames(1) f;

  memcpy_P(f, &frame, sizeof(f));
  for (unsigned row = 0; row < ROWS; row++) {
    for (unsigned col = 0; col < COLUMNS; col++) {
      RgbColor const c = f[row][col];
      ShowPixel(col, row, c);
    }
  }
}

void PGM_ShowFrame(BwFrames(1) const &frame, const RgbColor &c)
{
  BwFrames(1) f;

  memcpy_P(f, &frame, sizeof(f));
  for (unsigned row = 0; row < ROWS; row++) {
    uint8_t this_row = f[row];
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
  PGM_ShowFrame(mouth[idx], white);
}

void ShowMouthSmiling()
{ ShowMouth(0); }

static Running_avg<SAMPLES, unsigned> percent;

static void sound_sample()
{
#if defined(TESTMODE) && (TESTMODE == VOICE)
  percent += 50 * sin(millis() / 1000) + 50;
#else
  percent += MAX4466.scaled_loudness<99>();
#endif
}

static void display_update()
{
  unsigned idx;

  idx = SetState(percent);

#ifdef VOICEMASK_DEBUG
  Serial.print(F("MAX4466: ")); Serial.print(MAX4466);
  Serial.print(F(" --> percent (avg): ")); Serial.printf_P(PSTR("%3u"), unsigned(percent));
  if (animations.active()) {
    Serial.print(F(" --> anim idx: ")); Serial.println(animations.frame_nr());
  } else {
    Serial.print(F(" --> idx: ")); Serial.println(idx);
  }
#endif

  if (animations.active()) {
    animations.frame();
  } else {
    ShowMouth(idx);

    effects.apply(percent);

    if (idx == 0) {
      if (!bored.active()) {
        uint32_t delay_range = BORED_MAX_DELAY - BORED_MIN_DELAY;
        bored.once_ms(BORED_MIN_DELAY + random(delay_range), boring);
      }
    } else {
      bored.detach();
    }
  }

  strip.Show();
}

static void wifi_modem_sleep()
{
  if (wifi_fpm_get_sleep_type() == MODEM_SLEEP_T &&
      wifi_get_opmode() == NULL_MODE)
    return;

  wifi_set_opmode_current(NULL_MODE);
  yield();
  wifi_fpm_set_sleep_type(MODEM_SLEEP_T);
  yield();
  wifi_fpm_open();
  wifi_fpm_do_sleep(0xFFFFFFF);
  yield();

#ifdef VOICEMASK_DEBUG
  if (wifi_get_opmode() != NULL_MODE ||
      wifi_fpm_get_sleep_type() != MODEM_SLEEP_T) {
    Serial.println(F("Cannot turn off Wifi!\n"));
    delay(10_secs);
  }
#endif
}

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  init_serial();

  strip.Begin();
  strip.SetBrightness(85);
  strip.ClearTo(0);
  strip.Show();

  wifi_modem_sleep();
  MAX4466.setup(A0);

#ifdef VOICEMASK_DEBUG
  Serial.print( F("Display delay: ") ); Serial.println(DISPLAY_DELAY);
  Serial.print( F("Sample delay: ") ); Serial.println(SAMPLE_DELAY);
  Serial.println();
#endif
  static_assert(SAMPLE_DELAY > 0, "SAMPLE_DELAY breaks Ticker; needs to be >0.");
  static_assert(SAMPLE_DELAY < MIN_DELAY, "SAMPLE_DELAY is too high; WDT will kill us.");
  static_assert(SAMPLE_DELAY <= DISPLAY_DELAY, "DISPLAY_DELAY is too low.");
  static_assert(DISPLAY_DELAY < BORED_MIN_DELAY, "display vs. bored mismatch.");
  static_assert(BORED_MIN_DELAY <= BORED_MAX_DELAY, "bored delays mismatch.");
  static_assert(BORED_MAX_DELAY <= MAX_DELAY, "BORED_MAX_DELAY is too high; overflows.");


  sampler.attach_ms(SAMPLE_DELAY, sound_sample);
  display.attach_ms(DISPLAY_DELAY, display_update);

  pinMode(D3 /* Flash button */, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(D3), isr, FALLING);

  animations.init();
  effects.init();

  digitalWrite(LED_BUILTIN, HIGH);
}

void loop()
{
  halt();
}
