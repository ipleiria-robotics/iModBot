#include "BluetoothSerial.h"

BluetoothSerial ESP_BT;

#include <robotOnLine.h>

robotOnLine robot;

void setup()
{
  Serial.begin(115200);

  ESP_BT.begin("Edurobot");

  robot.begin();

}

void loop()
{

  ESP_BT.println("info"); //send a message

  ESP_BT.write('a');  // send single character

  char msg =  ESP_BT.read();  // receive information

  // Now it's a matter of playing with the bluetooth functions

  switch (msg) {
    case 'a':
      robot.forward(255);
      break;
    case 'b':
      robot.rotateRight(255);
      break;
    case 'c':
      robot.rotateLeft(255);
      break;
    case 'd':
      robot.reverse(255);
      break;

    default:
      robot.stopMotors();
      break;
  }
}
