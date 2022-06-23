#include <ArduinoOTA.h>

#include <iModBot.h>

iModBot robot; 

const char* ssid = "hotSpotName"; // Nome do ponto WiFi
const char* password = "password"; // Password do ponto WiFi
void setup()
{
  Serial.begin(115200);
  
  Serial.println("A ligar..");
  
  WiFi.disconnect(true);  //disconnect form wifi to set new wifi connection
  WiFi.mode(WIFI_STA); //init wifi mode

  WiFi.begin(ssid, password);
  
  while (WiFi.waitForConnectResult() != WL_CONNECTED)
  {
    Serial.println(" Erro de ligação! A reiniciar...");
    delay(2000);
    ESP.restart();
  }
  
  Serial.println("WiFi conectado, a configurar OTA..");

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
        Serial.printf("Progresso: %u%%\r", (progress / (total / 100)));
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

  Serial.println("Pronto!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Port: ");
  Serial.println(3232);

  robot.begin();

}

void loop()
{
  //A função ArduinoOTA.handle(); deve ser corrida de tempo a tempo para verificar se há um novo programa a ser recebido por WiFi
  ArduinoOTA.handle();

  //Escreva o código aqui
}
