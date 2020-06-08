/*  Neste exemplo serão exemplificadas maioria das funções da biblioteca RobotOnLine.
    As funções relacionadas com a funcionalidade de condução automática não serão descritas neste exemplo.
*/

#include <robotOnLine.h>  // É necessário incluir a biblioteca

robotOnLine robot;  // É sempre necessário criar esta linha, a palavra "robot" pode ser outra
//qualquer (desde que não faça interferência com a linguagem arduino).



void setup()
{
  /*  A função " editWheelPin(uint8_t , uint8_t ); " permite alterar os pinos onde são ligados os sensores òpticos.
      Se esta função for usada, a mesma deve ser chamada antes da função begin();
  */
  //  ( pino do encoder da roda esquerda, pino do encoder da roda direita)
  robot.editWheelPin(26 , 27);  // Estes são os pinos que a biblioteca usa por defeito.

  //  - - - - - - - - - - - - -

  /*  A função " editMotorPin(uint8_t , uint8_t , uint8_t , uint8_t ); " pode ser chamada depois da função begin(); mas não é aconselhado.
      Permite alterar onde são ligadas as conecções para controlar os motores.
      Na altura em que este exemplo foi criado foi usada uma placa com o CI L293D, e os nomes dos pinos da mesma eram IN1, IN2, IN3, IN4.
  */
  //                (IN1,IN2,IN3, IN4)
  robot.editMotorPin( 2 , 4 , 16, 17 ); // Estes são os pinos que a biblioteca usa por defeito.

  //  - - - - - - - - - - - - -

  /*  A função " editUltrasonicPin(uint8_t , uint8_t ); " pode ser chamada depois da função begin(); mas não é aconselhado.
      Esta função permite alterar os pinos que conectam  ao sensor ultrassónico HC-SR04.
  */
  //                 (trig pin, echo pin );
  robot.editUltrasonicPin(  14, 12);  // Estes são os pinos que a biblioteca usa por defeito.

  //  - - - - - - - - - - - - -

  /*  A função " editSensorPin(uint8_t , uint8_t , uint8_t , uint8_t , uint8_t , uint8_t , uint8_t ); "
      pode ser chamada depois da função begin(); mas não é aconselhado.
      Permite alterar os pinos onde são estabelecidas as ligações à placa frontal que possui vários sensores de infravermelho.
      Na altura em que este exemplo foi escrito estava a ser usada uma placa com o CI 74HC14,
      e os pinos S1, S2, S3, S4, S5, CLP, Near.
  */
  //                 (  S1, S2, S3, S4, S5,CLP,Near)
  robot.editSensorPin(  25, 33, 32, 35, 34, 39, 36);  // Estes são os pinos que a biblioteca usa por defeito.

  //  - - - - - - - - - - - - -

  /*  A unção " begin(); " irá configurar todos os pinos e interrupções em uso pela biblioteca.
      Os canais de PWM 0,1,2,3 do ESP32 serão usados pela biblioteca com uma resolução de 8 bit (0 to 255).
  */
  robot.begin();  // Esta função é essencial.

  //  - - - - - - - - - - - - -

  /*  A função "setSpeeds(byte, byte, byte)" permite dizer ao robô qual o dutty cycle
      que deverá corresponder às velocidades máxima, média e mínima.
  */
  robot.setSpeeds(255, 200, 180);

  //  - - - - - - - - - - - - -

  Serial.begin(115200);
}

void loop()
{
  byte dist = robot.distance(); // Esta função devolve a distância (em cm) medida pelo sensor HC-SR04, O valor devolvido varia de 0 to 255 cm
  Serial.print("A distância medida é de: ");
  Serial.print(dist);
  Serial.println("cm");

  //  - - - - - - - - - - - - -

  /*  As funções "turnLeft(uint16_t);" e "turnRight(uint16_t);" permitem dizer ao robô para rodar x graus
      para a direita ou esquerda (dependendo de qual das funções é chamada).
      Se for enviado um 0 o robô irá tentar rodar 90 graus. Se for enviado um valor acima de 360 o robô irá apenas rodar 360 graus.

  */
  robot.turnLeft(0);
  robot.turnRight(0);

  //  - - - - - - - - - - - - -

  /*  As funções "steerLeft(byte );" e "steerRight(byte );" permitem dizer ao robô para virar para a 
      direita ou esquerda (dependendo de qual das funções é chamada).
      Cada uma destas funções irá fazer o robô virar ao reduzir a velocidade da roda.
  */
  robot.steerLeft(255);   // Deves especificar a velocidade máxima (1 a 255) à qual o robô deve circular.
  robot.steerRight(255);

  //  - - - - - - - - - - - - -

  /*  As funções " rotateLeft(byte ); " e " rotateRight(byte ); " permitem dizer ao robô para começar a rodar 
      para a direita ou esquerda (dependendo de qual das funções é chamada) a x velocidade. 
      A velocidade especificada (de 0 a 255) irá indicar a velocidade à qual o robô deverá rodar.
      O robô continuará a rodar até que receba novas instruções.
  */
  robot.rotateLeft(255);  //  Introduzir um valor de 1 to 255, ciclos de trabalho baixos (numeros baixos) poderão não ser suficientes para o
  robot.rotateRight(150); //  robô conseguir virar. Um 0 não fará o robô rodar.

  //  - - - - - - - - - - - - -

  /*  A função "stopMotors();" para ambos os motores.
  */
  robot.stopMotors();

  //  - - - - - - - - - - - - -

  /*  A função "forward(byte );" permite dizer ao robô para avançar a x velocidade.
      Pode ser enviado um valor de 1 a 255, um 0 fará os motores pararem.
  */
  robot.forward(255);

  //  - - - - - - - - - - - - -

  /*  A função "reverse(byte );" permite dizer ao robô para recuar a x velocidade.
      Pode ser enviado um valor de 1 a 255, um 0 fará os motores pararem.
  */
  robot.reverse(255);

  //  - - - - - - - - - - - - -

  /*  As funções "leftWheel(short );" e "rightWheel(short );" permitem controlar a roda direita e/ou esquerda individualmente.
      Um valor negativo faz a rodar andar para trás, um valor positivo faz a mesma andar para a frente.
      Pode ser enviado um valor de -255 a 255, um 0 fará o motor parar.
  */
  robot.leftWheel(-255);
  robot.rightWheel(-255);

  //  - - - - - - - - - - - - -

  /*  As funções "getLeftEncoderCount();" e "getRightEncoderCount();" devolvem o numero de pulsos contados pelo encoder direito e/ou esquerdo.
      Estas funções devolvem um número de dois bytes do tipo short. 
  */
  short leftEncoder = robot.getLeftEncoderCount();
  short rightEncoder = robot.getRightEncoderCount();

  /*  A função "clearEncoderCount();" coloca o número de pulsos a 0.
      Quando o robô começar a andar a contagem será iniciada a partir de 0.
      A contagem de pulsos é colocada a 0 automaticamente quando a contagem alcança 30 000.
  */
  robot.clearEncoderCount();

  //  - - - - - - - - - - - - -

  /*  A função "checkSensors();" verifica os sensores da placa frontal do robô.
      Esta função é um pouco complicada de usar e apenas faz um "digitalRead" a cada pino conectado à
      placa frontal.
      Esta função devolve um byte (8 bits) que deve ser interpretada em binário.
      Cada bit, da direita para a esquerda, significam o seguinte:
      bit 0:  É 1 se o sensor "Near" detetar algo.
      bit 1:  É 1 se o botão CLP for pressionado, isto normalmente acontece quando o robô colide com algo.
      bit 2:  É 1 se o sensor mais à direita detetar uma linha.
      bit 3:  É 1 se o sensor se o sensor da direita detetar uma linha.
      bit 4:  É 1 se o sensor do meio detetar uma linha.
      bit 5:  É 1 se o sensor da esquerda detetar uma linha.
      bit 6:  É 1 se o sensor mais à esquerda detetar uma linha.
  */
  byte info = robot.checkSensors();

  if ( ( info & 0b00000001 ) != 0) { //bit 0
    Serial.println(" Alguma coisa  foi detetada pelo sensor Near. ");
  }
  if ( ( info & 0b00000010 ) != 0) { //bit 1
    Serial.println(" Alguma coisa  foi detetada pelo botão CLP. ");
  }
  if ( ( info & 0b00000100 ) != 0) { //bit 2
    Serial.println(" Uma linha foi detetada pelo sensor mais à dirita. ");
  }
  if ( ( info & 0b00001000 ) != 0) { //bit 3
   Serial.println(" Uma linha foi detetada pelo sensor da direita. ");
  }
  if ( ( info & 0b00010000 ) != 0) { //bit 4
    Serial.println(" Uma linha foi detetada pelo sensor do meio. ");
  }
  if ( ( info & 0b00100000 ) != 0) { //bit 5
    Serial.println(" Uma linha foi detetada pelo sensor da esquerda. ");
  }
  if ( ( info & 0b01000000 ) != 0) { //bit 6
    Serial.println(" Uma linha foi detetada pelo sensor mais à esquerda. ");
  }

  //  - - - - - - - - - - - - -

  /*
   * Para facilitar a leitura infividual de cada pino do módulo de sensores infravermelho
   * foram criadas as seguintes funções.
   * Se o estado do pino for "1" o seu estado é "HIGH" e vice-versa.
   */
   
  bool pinState=0;
  
  pinState = robot.readCLP();

  pinState = robot.readNear();
  
  pinState = robot.readS1();
  
  pinState = robot.readS2();
  
  pinState = robot.readS3();
  
  pinState = robot.readS4();
  
  pinState = robot.readS5();
  
}
