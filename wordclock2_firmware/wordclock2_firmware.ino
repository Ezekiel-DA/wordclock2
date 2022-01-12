#include <WiFi.h>
#include <esp32fota.h>
#include <WiFiProv.h>
#include <ezTime.h>
#include <FastLED.h>

#include "clockface.h"

#define FIRMWARE_VERSION 3

esp32FOTA esp32FOTA("wordclock2-base-firmware", FIRMWARE_VERSION);
Timezone tz;

static uint16_t rainbowLoopGlobalSpeed = 100;
static uint8_t hue = 0;

TaskHandle_t startupEffectTaskHandle = 0;
TaskHandle_t FOTAIndicatorTaskHandle = 0;

void SysProvEvent(arduino_event_t *sys_event)
{
    switch (sys_event->event_id) {
    case ARDUINO_EVENT_WIFI_STA_GOT_IP:
        Serial.print("\nConnected IP address : ");
        Serial.println(IPAddress(sys_event->event_info.got_ip.ip_info.ip.addr));
        break;
    case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
        Serial.println("\nDisconnected. Connecting to the AP again... ");
        break;
    case ARDUINO_EVENT_PROV_START:
        Serial.println("\nProvisioning started\nGive Credentials of your access point using \" Android app \"");
        break;
    case ARDUINO_EVENT_PROV_CRED_RECV: { 
        Serial.println("\nReceived Wi-Fi credentials");
        Serial.print("\tSSID : ");
        Serial.println((const char *) sys_event->event_info.prov_cred_recv.ssid);
        Serial.print("\tPassword : ");
        Serial.println((char const *) sys_event->event_info.prov_cred_recv.password);
        break;
    }
    case ARDUINO_EVENT_PROV_CRED_FAIL: { 
        Serial.println("\nProvisioning failed!\nPlease reset to factory and retry provisioning\n");
        if(sys_event->event_info.prov_fail_reason == WIFI_PROV_STA_AUTH_ERROR) 
            Serial.println("\nWi-Fi AP password incorrect");
        else
            Serial.println("\nWi-Fi AP not found....Add API \" nvs_flash_erase() \" before beginProvision()");        
        break;
    }
    case ARDUINO_EVENT_PROV_CRED_SUCCESS:
        Serial.println("\nProvisioning Successful");
        break;
    case ARDUINO_EVENT_PROV_END:
        Serial.println("\nProvisioning Ends");
        break;
    default:
        break;
    }
}

void rainbowLoop(void*) {
  while (1) {
    clearClockFace();
    for (uint8_t y = 0; y < 10; ++y) {
      for (uint8_t x = 0; x < 13; ++x) {
        _leds[XY(x, y)] = CHSV(hue, 255, 255);
        //FastLED.show(); // don't do this in a task
        delay(rainbowLoopGlobalSpeed);
        hue += 5;
      }
      //hue += 100;
    }
  }
}

void FOTAIndicatorAnimation(void*) {
  while(1) {
    minute1();
    delay(500);
    minute1(false);
    delay(500);
  }
}

void setup() {
    Serial.begin(115200);

    pinMode(TOP_LEFT_LED_PIN, OUTPUT);
    pinMode(TOP_RIGHT_LED_PIN, OUTPUT);
    pinMode(BOTTOM_LEFT_LED_PIN, OUTPUT);
    pinMode(BOTTOM_RIGHT_LED_PIN, OUTPUT);
    FastLED.addLeds<WS2812B, WORD_LEDS_PIN, GRB>(_leds, 130); // don't add FastLED's "typical LED" correction; it makes the whites pink-ish
    FastLED.setBrightness(150);
    clearClockFace();
    FastLED.show();

    esp32FOTA.checkURL = "https://ezekiel-da.github.io/wordclock2/deployment.json";

    WiFi.onEvent(SysProvEvent);
    WiFiProv.beginProvision(WIFI_PROV_SCHEME_BLE, WIFI_PROV_SCHEME_HANDLER_FREE_BTDM, WIFI_PROV_SECURITY_1, "bisous", "PROV_clock");

    Serial.println("Word Clock II - Word Clock Lives");
    Serial.print("Running firmware version "); Serial.println(FIRMWARE_VERSION);

    xTaskCreate(rainbowLoop, "startupEffect", 10000, NULL, 1, &startupEffectTaskHandle);

    rainbowLoopGlobalSpeed = 300;

    while (WiFi.status() != WL_CONNECTED) {
      events();
      FastLED.show();
      delay(10);
    }

    rainbowLoopGlobalSpeed = 100;

    while (timeStatus() != timeSet) {
      events();
      FastLED.show();
      delay(10);
    }
    
    vTaskDelete(startupEffectTaskHandle);
    delay(1000);
    
    clearClockFace();
    FastLED.show();
    
    delay(500);
    Serial.println("UTC: " + UTC.dateTime());

    tz.setLocation("Europe/Paris");
}

void loop() {
    events(); // for EZTime's NTP sync etc
   
    uint8_t hour = tz.hourFormat12();
    uint8_t minute = tz.minute();
    Serial.println("Local time: " + tz.dateTime());

     static uint16_t prevFOTACheck = millis();
    uint16_t now = millis();
    if ((uint16_t)(now - prevFOTACheck) >= 5)
    {
      bool updatedNeeded = esp32FOTA.execHTTPcheck();
      if (updatedNeeded)
      {
        xTaskCreate(FOTAIndicatorAnimation, "FOTAIndicatorAnimation", 10000, NULL, 1, &FOTAIndicatorTaskHandle);
        esp32FOTA.execOTA();
        vTaskDelete(FOTAIndicatorTaskHandle);
      }
       
      prevFOTACheck = now;
    }
    
    clearClockFace();
    it();
    is();

      switch (minute % 5) { // NB: all fall through is by design; if 4 mins, 4,3,2 and 1 need to be lit, if 3 mins, 3,2 and 1, etc.
          case 4:
            minute4();
          case 3:
            minute3();
          case 2:
            minute2();
          case 1:
            minute1();
        }

    if (minute < 5)
        oclock();
    else if (minute >= 5 && minute < 10) {
        fiveM();
        minutes();
        past();
    } else if (minute >= 10 && minute < 15) {
        tenM();
        minutes();
        past();
    } else if (minute >= 15 && minute < 20) {
        quarter();
        past();
    } else if (minute >= 20 && minute < 25) {
        twenty();
        minutes();
        past();
    } else if (minute >= 25 && minute < 30) {
        twenty();
        fiveM();
        minutes();
        past();
    } else if (minute >= 30 && minute < 35) {
        half();
        past();
    } else if (minute >= 35 && minute < 40) {
        twenty();
        fiveM();
        minutes();
        to();
    } else if (minute >= 40 && minute < 45) {
        twenty();
        minutes();
        to();
    } else if (minute >= 45 && minute < 50) {
        quarter();
        to();
    } else if (minute >= 50 && minute < 55) {
        tenM();
        minutes();
        to();
    } else if (minute >= 55) {
        fiveM();
        minutes();
        to();
    }

    switch (hour) {
        case 0:
        case 12:
            minute <= 34 ? twelve() : one();
            break;
        case 1:
        case 13:
            minute <= 34 ? one() : two();
            break;
        case 2:
        case 14:
            minute <= 34 ? two() : three();
            break;
        case 3:
        case 15:
            minute <= 34 ? three() : four();
            break;
        case 4:
        case 16:
            minute <= 34 ? four() : five();
            break;
        case 5:
        case 17:
            minute <= 34 ? five() : six();
            break;
        case 6:
        case 18:
            minute <= 34 ? six() : seven();
            break;
        case 7:
        case 19:
            minute <= 34 ? seven() : eight();
            break;
        case 8:
        case 20:
            minute <= 34 ? eight() : nine();
            break;
        case 9:
        case 21:
            minute <= 34 ? nine() : ten();
            break;
        case 10:
        case 22:
            minute <= 34 ? ten() : eleven();
            break;
        case 11:
        case 23:
            minute <= 34 ? eleven() : twelve();
            break;
    }

    FastLED.show();

    delay(1000);
}
