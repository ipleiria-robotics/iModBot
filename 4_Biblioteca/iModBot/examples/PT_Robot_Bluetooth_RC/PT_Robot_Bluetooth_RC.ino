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

  ESP_BT.println("info"); // Enviar uma mensagem

  ESP_BT.write('a');  // Enviar apenas um caracter

  char msg =  ESP_BT.read();  // Receber um caracter

  // Agora é apenas uma questão de jogar com as funções do bluetooth.

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
