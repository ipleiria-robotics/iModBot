/*
 * Neste exemplo vamos usar a função d econdução automática da biblioteca "robotOnLine" juntamento com
 * o exemplo "Blink"
 */

#include <iModBot.h>

// aqui criamos um objeto chamado "robot"
iModBot robot; 
/*
 * Podem substituir "robot" por outra palavra, depois quando chamarem uma função da biblioteca
 * terão de escrever:
 * palavra.função();
 * em vez de:
 * robot.função();
 */

unsigned long enlapsedTime=0;
 
void setup() 
{
  //Esta função configura os pinos necessários para o projeto.
  robot.begin();	// Configura os pinos.
  
  robot.beginAutoDrive(); 	// Inicia a função de condução autónoma.
  // vejam o tutorial presente no gitHub para mais informação
  //

  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() 
{
  // Na próxima linha é executada a função responsável por controlar o robô automáticamente 
  int info = robot.autoDrive(0);
  /* Esta função vai devolver um valor, este é 0 se não houver nada a reportar,
   *  1 se forem encontradas várias linhas, 2 se o robô encontrar um obstáculo 
   *  e 3 se não forem encontradas nenhumas linhas.
   *  
   *  Quanto mais depressa esta função for chamada outra vez mais eficaz será o robô.
   */


   /* [ delay(); ?? ]
    *  Como temos de chamar a função autoDrive() o mais cedo possível
    *  tivémos que remover as funções delay(), mas não se preocupem,
    *  ainda é possível controlar o tempo do piscar do LED usando a função
    *  millis().
    *  mais informação sobre a função millis() [https://www.arduino.cc/reference/en/language/functions/time/millis/]
    */

  
  if( millis() > enlapsedTime+2000 )  // A primeira vez que esta linha for processada esta condição será verdadeira
  {
    digitalWrite(LED_BUILTIN, HIGH);  //ligamos o LED
    enlapsedTime=millis();            //aqui guardamos o tempo atual
  }
  else if( millis() > enlapsedTime+1000 ) //quando passar um segundo (1000 ms) desde que guardámos o tempo:
  {
    digitalWrite(LED_BUILTIN, LOW);   //desligamos o LED, tomem nota que este esteve ligado um segundo desde que o primeiro IF foi processado 
  }
  // Quando passar outro segundo a primeira condição IF será verdadeira de novo e o LED será ligado.
  // Uma vez mais, o LED ficou desligado durante um segundo. Este ciclo continuará indefinidamente.
}
