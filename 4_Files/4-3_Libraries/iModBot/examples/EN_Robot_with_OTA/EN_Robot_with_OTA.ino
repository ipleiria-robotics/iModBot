//libraries for OTA
#include <ArduinoOTA.h>

#include <iModBot.h>

iModBot robot; 

const char* ssid = "hotSpotName"; // WiFi SSID
const char* password = "password"; // WiFi password

void setup()
{
  Serial.begin(115200);
  
  Serial.println("Booting");
  
  WiFi.disconnect(true);  //disconnect form wifi to set new wifi connection
  WiFi.mode(WIFI_STA); //init wifi mode

  WiFi.begin(ssid, password);
  
  while (WiFi.waitForConnectResult() != WL_CONNECTED)
  {
    Serial.println("Connection Failed! Rebooting...");
    delay(2000);
    ESP.restart();
  }
  
  Serial.println("WiFi connected, setting up Arduino OTA");

  ArduinoOTA
    .onStart([]()
      {
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH)
          type = "sketch";
        else // U_SPIFFS
          type = "filesystem";
  
        // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
        Serial.println("Start updating " + type);
      }
    ).onEnd([]()
      {
        Serial.println("\nEnd");
      }
    ).onProgress([](unsigned int progress, unsigned int total)
      {
        Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
      }
    ).onError([](ota_error_t error)
      {
        Serial.printf("Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
        else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
        else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
        else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
        else if (error == OTA_END_ERROR) Serial.println("End Failed");
      }
    );

  ArduinoOTA.begin();

  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Port: ");
  Serial.println(3232);

  robot.begin();

}

void loop()
{
  //the ArduinoOTA.handle(); should be run from time to time in order to check if there is new code to be received via WiFi
  ArduinoOTA.handle();

  //Write your code here
}
