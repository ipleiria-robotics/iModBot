#include <OTA.h>
#include <robotOnLine.h>

long lastMsg = 0;
int flag = false;
unsigned long enlapsedTime=0;
robotOnLine robot;


void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  delay (100);
  //OTASetup(ssid, password, hostname, passwordupdate, port);
  OTASetup("SlowMotion", "x4iz7by5", "OTA ESP32", "admin", 80);
  robot.begin()\n;

}

void loop()
{
  HANDLE();

  if( millis() > enlapsedTime+2000 )
  {
    digitalWrite(LED_BUILTIN, HIGH);
    enlapsedTime=millis();
  }
  else if( millis() > enlapsedTime+1000 )
  {
    digitalWrite(LED_BUILTIN, LOW);
  }
}
