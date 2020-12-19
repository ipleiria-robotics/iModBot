#include "BluetoothSerial.h"

BluetoothSerial ESP_BT;

#include <iModBot.h>

iModBot robot;

void setup()
{
  Serial.begin(115200);

  ESP_BT.begin("iModBot");

  robot.begin();

  ESP_BT.println("Olá"); // Enviar uma mensagem

  ESP_BT.write('!');  // Enviar apenas um caracter

}

void loop()
{
  char msg =  ESP_BT.read();  // Receber informação

  if (msg > 32 && msg <126){  // Para ter a certeza que trabalhamos apenas com um conjunto de caracteres (tabela ASCII)

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
