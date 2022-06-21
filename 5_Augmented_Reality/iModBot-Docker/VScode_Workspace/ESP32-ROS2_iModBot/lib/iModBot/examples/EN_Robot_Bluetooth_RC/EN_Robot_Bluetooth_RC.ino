#include "BluetoothSerial.h"

BluetoothSerial ESP_BT;

#include <iModBot.h>

iModBot robot;

void setup()
{
  Serial.begin(115200);

  ESP_BT.begin("iModBot");

  robot.begin();

  ESP_BT.println("Hello"); // Send a message

  ESP_BT.write('!');  // Send just a character

}

void loop()
{
  char msg =  ESP_BT.read();  // Receive information

  if (msg > 32 && msg <126){  // To make sure we only work with certain characters (from the ASCII table)

    switch (msg) {
    case '1':
      robot.forward(255);
      break;
    case '2':
      robot.rotateRight(255);
      break;
    case '3':
      robot.rotateLeft(255);
      break;
    case '4':
      robot.reverse(255);
      break;

    default:
      robot.stopMotors();
      break;
    }
  }
  delay(100);
}
