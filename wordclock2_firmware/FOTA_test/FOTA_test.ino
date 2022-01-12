#include <WiFi.h>
#include <esp32fota.h>
#include <WiFiProv.h>

#define FIRMWARE_VERSION 1

esp32FOTA esp32FOTA("FOTA_TEST", FIRMWARE_VERSION);

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

void setup() {
    Serial.begin(115200);

    esp32FOTA.checkURL = "https://ezekiel-da.github.io/wordclock2/FOTA_TEST.json";
    
    WiFi.onEvent(SysProvEvent);
    WiFiProv.beginProvision(WIFI_PROV_SCHEME_BLE, WIFI_PROV_SCHEME_HANDLER_FREE_BTDM, WIFI_PROV_SECURITY_1, "bisous", "PROV_clock");

    Serial.print("FOTA TESTER V "); Serial.println(FIRMWARE_VERSION);

    while (WiFi.status() != WL_CONNECTED) {
      delay(10);
    }
    Serial.println("Wifi connected");
}

void loop() {
    
      bool updatedNeeded = esp32FOTA.execHTTPcheck();
      if (updatedNeeded)
      {
        esp32FOTA.execOTA();
      }

      delay(30000);
    
}
