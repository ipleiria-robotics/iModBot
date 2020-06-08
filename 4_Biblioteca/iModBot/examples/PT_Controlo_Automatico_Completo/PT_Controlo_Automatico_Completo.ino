// neste exemplo vamos explicar como usar a função autoDrive()
// A placa usada para este turorial foi ESP32 Devkit V1

// Vamos definir os pinos aos quais os sensores foram conectados
#define farLeft 25
#define left 33
#define middle 32
#define right 35
#define farRight 34
#define CLP 39
#define near 36

#include <robotOnLine.h>

robotOnLine robot; // Criar um objeto chamado "robot"

void setup()
{
  Serial.begin(115200); // 115200 é a velocidade do monitor série para as placas ESP
	
  robot.begin();	// Configura os pinos.
  
  robot.beginAutoDrive();	//	Inicia a função de condução autónoma.
  
  Serial.println("-> Pinos configurados!");

  byte dist = 10; // Distância (em cm) à qual o robô pára.
  robot.setStopDistance(dist);
  Serial.print("-> Distância de paragem definida: ");
  Serial.println(dist);
}

void loop()
{
  int info = 0; // Variável para guardar o numero devolvido pela função autoDrive().
  int command = 0; // variável para lembrar que comandos já foram executados.



  info = robot.autoDrive(0); // Aqui copiamos o valor devolvido pela função para a variável info.

  /* Sempre que usarem a função autoDrive() têm de enviar um valor, estes são as instruções aceites:
      0 -> Enviar 0 ou qualquer outro número (até 255) não listado se não quiser dar instruções ao robô.
      1 -> Enviar 1 para colocar o robô a rodar para a direita até encontrar uma linha.
      2 -> Enviar 2 para colocar o robô a rodar para a esquerda até encontrar uma linha.
      3 -> Enviar 3 para dizer ao robô para seguir em frente.
      4 -> Enviar 4 para dizer ao robô para recuar, tenha em atenção que o robô pode-se desalinhar ligeiramente devido à roda traseira.
  */



  if (info == 1) // A função autoDrive() devolve o valor 1 se forem encontradas multiplas linhas
  {
    switch ( command )
    {
      case 0: // Se a variável command for 0, executa-se a primeira ordem
        // Código para a 1ª vez que multiplas linhas forem encontradas.
        robot.autoDrive(1); // Virar à direita.
        command++;  // No fim incrementamos a variável, is é equivalente a ter "command = command +1;"
        break;

      case 1:
        // Código para a 2ª vez que multiplas linhas forem encontradas.
        robot.autoDrive(2); // Virar à esquerda.
        command++;
        break;

      case 2:
        // Código para a 3ª vez que multiplas linhas forem encontradas.
        robot.autoDrive(3); // Seguir em frente.
        command++;
        break;

      case 3:
        // Código para a 4ª vez que multiplas linhas forem encontradas.
        // Okay, digamos que o robô encontrou uma linha em frente e uma de lado, mas não sabemos que lado        
        // Vamos verificar de que lado a linha se encontra e dizer ao robô para seguir esse caminho
        if ( digitalRead(left) || digitalRead(farLeft) )
        {
          robot.autoDrive(2); // Virar à esquerda.
        }
        else // if( digitalRead(right) || digitalRead(farRight) )
        { // Podiamos verificar se havia uma linha à direita, mas se sabemos que há 
          // multiplas linhas e nenhuma se encontra à esquerda, só nos resta uma opção.
          robot.autoDrive(1); // Virar à direita.
        }
        command++;
        break;

      case 4:
        // Código para a 5ª vez que multiplas linhas forem encontradas.

        command++;  //Se esta variável for colocada a 0 os comandos anteriores serão repetidos.
        break;

      default:
        break;
    }
  }

  if (info == 2) // A função autoDrive() devolve o valor 2 se for encontrado algum obstáculo.
  {
    Serial.println("Está algo no caminho!");
  }

  if (info == 3) // A função autoDrive() devolve o valor 3 se não forem encontradas linhas.
  {
    Serial.println("Não foram encontradas linhas, o robô está na pista?");
    Serial.println("É possível que estaja apenas a virar de momento.");
  }
}
