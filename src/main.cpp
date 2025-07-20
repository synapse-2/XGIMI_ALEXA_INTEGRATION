#include <Arduino.h>

// New feature! Overclocking WS2812
// #define FASTLED_OVERCLOCK 1.2 // 20% overclock ~ 960 khz.
#include <FastLED.h>




// Define the LED_BUILTIN pin for the ESP32
// This is typically GPIO 48 on many ESP32 boards, but can vary by board.

#define LED_BUILTINIO GPIO_NUM_48
// Define the number of pixels in the NeoPixel strip
#define NUMPIXELS 1
CRGB leds[NUMPIXELS];




void setup()
{
  Serial.begin(115200);
  while (!Serial)
    ; // wait for serial attach

  Serial.println();
  Serial.println("Initializing...");
  Serial.flush();

  Serial.println("Example configured to blink GPIO LED!");

  Serial.println();
  Serial.println("Running...");

  printf("Hello world!\n");

  /* Print chip information */
  esp_chip_info_t chip_info;
  uint32_t flash_size;
  esp_chip_info(&chip_info);
  printf("This is %s chip with %d CPU core(s), %s%s%s%s, ",
         CONFIG_IDF_TARGET,
         chip_info.cores,
         (chip_info.features & CHIP_FEATURE_WIFI_BGN) ? "WiFi/" : "",
         (chip_info.features & CHIP_FEATURE_BT) ? "BT" : "",
         (chip_info.features & CHIP_FEATURE_BLE) ? "BLE" : "",
         (chip_info.features & CHIP_FEATURE_IEEE802154) ? ", 802.15.4 (Zigbee/Thread)" : "");

  unsigned major_rev = chip_info.revision / 100;
  unsigned minor_rev = chip_info.revision % 100;
  printf("silicon revision v%d.%d, ", major_rev, minor_rev);
  if (esp_flash_get_size(NULL, &flash_size) != ESP_OK)
  {
    printf("Get flash size failed");
    return;
  }

  printf("%" PRIu32 "MB %s flash\n", flash_size / (uint32_t)(1024 * 1024),
         (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

  printf("Minimum free heap size: %" PRIu32 " bytes\n", esp_get_minimum_free_heap_size());

  FastLED.addLeds<NEOPIXEL, LED_BUILTINIO>(leds, NUMPIXELS);
}

void loop()
{

  delay(1000);

    leds[0] = CRGB::;
     FastLED.show(); delay(30);
    leds[0] = CRGB::Black; 
    FastLED.show(); 
    
    delay(30);
}
