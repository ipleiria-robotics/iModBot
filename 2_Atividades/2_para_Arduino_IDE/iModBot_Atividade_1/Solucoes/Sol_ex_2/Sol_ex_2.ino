#include <robotOnLine.h>

robotOnLine robot;

#define hopTime 10  // tempo que o robô aguarda até alterar a velocidade de novo

void setup() 
{
  robot.begin();
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() 
{
  // Velocidade controlada por PWM, resolução de 8 bit
  // Pode variar de 0 a 255

    // Rodar para a esquerda

  //acelerar
  for(int i=0; i<255; i = i+5)
  {
    robot.rotateLeft(i);
    delay(hopTime);
  }
  delay(10);
  //desacelerar
  for(int i=255; i>0; i = i-5)
  {
    robot.rotateLeft(i);
    delay(hopTime);
  }
  
  delay(500);

    //Rodar para a direita

  //acelerar
  for(int i=0; i<255; i = i+5)
  {
    robot.rotateRight(i);
    delay(hopTime);
  }
  delay(10);
  //desacelerar
  for(int i=255; i>0; i = i-5)
  {
    robot.rotateRight(i);
    delay(hopTime);
  }
  
  delay(500);

    //Avançar

  //acelerar
  for(int i=0; i<255; i = i+5)
  {
    robot.forward(i);
    delay(hopTime);
  }
  delay(10);
  //desacelerar
  for(int i=255; i>0; i = i-5)
  {
    robot.forward(i);
    delay(hopTime);
  }
  
  delay(500);

    //Recuar

  //acelerar
  for(int i=0; i<255; i = i+5)
  {
    robot.reverse(i);
    delay(hopTime);
  }
  delay(10);
  //desacelerar
  for(int i=255; i>0; i = i-5)
  {
    robot.reverse(i);
    delay(hopTime);
  }
  
  delay(500);

  while(1); // parar execução do programa
  
}
