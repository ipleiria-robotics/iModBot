/*  In this example we'll have a look at most functions provided by the RobotOnLine library.
    Functions related tot he auto driving feature are not described here.
*/

#include <iModBot.h>  //This line is a must-have

iModBot robot;  //This line is a must-Have as well, here the word "robot" can be any other
//word (that doesn't make conflict with the Arduino Language) at your choice.



void setup()
{
  /*  The " editWheelPin(uint8_t , uint8_t ); " function allows you to edit the pins where you connect the optical encoders.
      If you use this function you must call it (i.e write it) before you call the begin(); function
  */
  //  ( Left Encoder pin, Right Encoder pin )
  robot.editWheelPin(26 , 27);  // This are the default pins the library will use

  //  - - - - - - - - - - - - -

  /*  The " editMotorPin(uint8_t , uint8_t , uint8_t , uint8_t ); " function can be called after the begin() function, but it's advised to call it before.
      It allows you to change the pins in charge of activating each motor.
      At the time this tutorial was written we were using a board with the L293D IC, and the pins labelled IN1, IN2, IN3, IN4
  */
  //                (IN1,IN2,IN3, IN4)
  robot.editMotorPin( 2 , 4 , 16, 17 ); // This are the default pins the library will use

  //  - - - - - - - - - - - - -

  /*  The " editUltrasonicPin(uint8_t , uint8_t ); " function can be called after the begin() function, but it's advised to call it before.
      It allows you to change the pins that connect to the HC-SR04 (ultrasonic sensor).
  */
  //                 (trig pin, echo pin );
  robot.editUltrasonicPin(  14, 12);  // This are the default pins the library will use

  //  - - - - - - - - - - - - -

  /*  The " editSensorPin(uint8_t , uint8_t , uint8_t , uint8_t , uint8_t , uint8_t , uint8_t ); " function
       can be called after the begin() function, but it's advised to call it before.
      It allows you to change the pins that connect to the board with the infrared sensors.
      At the time this tutorial was written we were using a board with the 74HC14 IC,
      and it had the pins labelled S1, S2, S3, S4, S5, CLP, Near.
  */
  //                 (  S1, S2, S3, S4, S5,CLP,Near)
  robot.editSensorPin(  25, 33, 32, 35, 34, 39, 36);  // This are the default pins the library will use

  //  - - - - - - - - - - - - -

  /*  If you will be using the autoDrive() function then " setStopDistance(byte ); " function allows you to change the distance (in cm)
       (measured by the ultrasonic sensor) at wich the robot will stop moving.
  */
  //                   (distance)
  robot.setStopDistance(5 ); //This is the default value used by the library

  //  - - - - - - - - - - - - -

  /*  The " begin(); " function will configure the pins and interruptions needed for the library.
      The PWM channels 0,1,2,3 of the ESP32 are used by the library with a resolution of 8 bit (0 to 255)
  */
  robot.begin();  // Don't forget to call this one

  //  - - - - - - - - - - - - -

  /*  The "setSpeeds(byte, byte, byte)" function allows you to tell the robot what
      dutty cycle corresponds to the fast, medium and low speed.
  */
  robot.setSpeeds(255, 200, 180);

  //  - - - - - - - - - - - - -

  Serial.begin(115200);
}

void loop()
{
  byte dist = robot.distance(); //this functions returns the distance (in cm) measured by the HC-SR04, ranges from 0 to 255 cm
  Serial.print("Distance measured is: ");
  Serial.print(dist);
  Serial.println("cm");

  //  - - - - - - - - - - - - -

  /*  The functions "turnLeft(uint16_t);" and "turnRight(uint16_t);" allow you to tell the robot to turn n degrees
      to the right or left (depending wich function you call).
      If you send a 0 it will attempt to turn 90 degrees. If you send a value higher than 360º it will attempt to turn only 360.

  */
  robot.turnLeft(0);
  robot.turnRight(0);

  //  - - - - - - - - - - - - -

  /*  The functions "steerLeft(byte );" and "steerRight(byte );" allow you to tell the robot to turn.
      to the right or left (depending wich function you call).
      Each of this functions will attempt steer the robot by decreasing the speed of the oposing wheel.
  */
  robot.steerLeft(255);   //You should send the max speed (0 to 255) at wich you want your robot to go.
  robot.steerRight(255);

  //  - - - - - - - - - - - - -

  /*  The functions " rotateLeft(byte ); " and " rotateRight(byte ); " allow you to make the robot rotate
      to the right or left (depending on wich functions you call) at n speed, the value you enter (from 0 to 255)
      defines the speed at wich the wheels will turn.
      The robot will keep turning until you instruct it not to.
  */
  robot.rotateLeft(255);  //  Introduce a value from 1 to 255, low dutty cycles are less likely to make the wheel actually turn
  robot.rotateRight(150); //  If you insert 0 the robot won't move.

  //  - - - - - - - - - - - - -

  /*  The "stopMotors();" function is self explanatory, it simply stops both motors/wheels
  */
  robot.stopMotors();

  //  - - - - - - - - - - - - -

  /*  The "forward(byte );" function allows you to instruct the robot to move forward at n speed;
      You can send a value of 1 up to 255, a 0 will  just stop the motors.
  */
  robot.forward(255);

  //  - - - - - - - - - - - - -

  /*  The "reverse(byte );" function allows you to instruct the robot to move backwards at n speed;
      You can send a value of 1 up to 255, a 0 will  just stop the motors.
  */
  robot.reverse(255);

  //  - - - - - - - - - - - - -

  /*  The "leftWheel(short );" and "rightWheel(short );" function allows you to control the left or right wheel individually.
      If you send a negative value the wheel will move backwards, a positive value will make the wheel move forward.
      A 0 will stop the wheel. It accepts values of -255 up to 255.
  */
  robot.leftWheel(-255);
  robot.rightWheel(-255);

  //  - - - - - - - - - - - - -

  /*  The "getLeftEncoderCount();" and "getRightEncoderCount();" functions allow you to get the number of pulses from the left or right encoder.
      It returns a two byte number of type short.«
  */
  short leftEncoder = robot.getLeftEncoderCount();
  short rightEncoder = robot.getRightEncoderCount();

  /*  The function "clearEncoderCount();" set's the number of pulses to 0.
      So when the robot start moving it will start incrementing up from zero.
      The pulse count is resetted automatically when it reaches 30 000.
  */
  robot.clearEncoderCount();

  //  - - - - - - - - - - - - -

  /*  The "checkSensors();" function check's the sensors from the board on the front.
      This function may be a bit hard to understand and it basically just does a "digitalRead();" of the pins
      coming from the board conecting the ESP32.

      This functions returns a byte that should be interpreted in binary.
      The bits, from right to left mean the following:
      bit 0:  Is 1 if the infrared sensor (Near) senses something nearby.
      bit 1:  Is 1 if the CLP switch is pressed, this usually happens if the robot impacts with something.
      bit 2:  Is 1 if the far right sensor detected a line.
      bit 3:  Is 1 if the right sensor detected a line.
      bit 4:  Is 1 if the middle sensor detected a line.
      bit 5:  Is 1 if the left sensor detected a line.
      bit 6:  Is 1 if the far left sensor detected a line.
  */
  byte info = robot.checkSensors();

  if ( ( info & 0b00000001 ) != 0) { //bit 0
    Serial.println(" Something was detected by the Near sensor. ");
  }
  if ( ( info & 0b00000010 ) != 0) { //bit 1
    Serial.println(" Something was detected by the CLP switch. ");
  }
  if ( ( info & 0b00000100 ) != 0) { //bit 2
    Serial.println(" A line was found by the far right sensor. ");
  }
  if ( ( info & 0b00001000 ) != 0) { //bit 3
    Serial.println(" A line was found by the right sensor. ");
  }
  if ( ( info & 0b00010000 ) != 0) { //bit 4
    Serial.println(" A line was found by the middle sensor. ");
  }
  if ( ( info & 0b00100000 ) != 0) { //bit 5
    Serial.println(" A line was found by the left sensor. ");
  }
  if ( ( info & 0b01000000 ) != 0) { //bit 6
    Serial.println(" A line was found by the far left sensor. ");
  }

  //  - - - - - - - - - - - - -

  /* 
   * To ease the reading of the pins from the infrared module the following
   * functions can be used.
   * if the function returns "1" it means the pin state is "HIGH" and vice versa.
   */
   
  bool pinState=0;
  
  pinState = robot.readCLP();

  pinState = robot.readNear();
  
  pinState = robot.readS1();
  
  pinState = robot.readS2();
  
  pinState = robot.readS3();
  
  pinState = robot.readS4();
  
  pinState = robot.readS5();
    
  //  - - - - - - - - - - - - -

  // In order to use functions related to rotationbs per second/minute and meters per second
  // it is required that the function "enSpeedAdj()" is called once before.

  robot.enSpeedAdj();

  // To make the robot move forward at the specified speed in RPS
   
  robot.forwardRPS(2.1);

  // To make the robot move backwards at the specified speed in RPS
   
  robot.reverseRPS(1.8);

  // To make the robot rotate to the right (Clock-Wise) at the specified speed in RPS
   
  robot.rotateRightRPS(2.3);

  // To make the robot rotate left (Counter Clock-Wise) at the specified speed in RPS
   
  robot.rotateLeftRPS(2.4);
  
  //  - - - - - - - - - - - - -

  // To make the robot move forward at the specified speed in RPM
   
  robot.forwardRPM(2.1);

  // To make the robot move backwards at the specified speed in RPM
   
  robot.reverseRPM(1.8);

  // To make the robot rotate to the right (Clock-Wise) at the specified speed in RPM
   
  robot.rotateRightRPM(2.3);

  // To make the robot rotate left (Counter Clock-Wise) at the specified speed in RPM
   
  robot.rotateLeftRPM(2.4);
  
  //  - - - - - - - - - - - - -

  // To make the robot move forward at the specified speed in m/s
   
  robot.forwardMS(2.1);

  // To make the robot move backwards at the specified speed in m/s
   
  robot.reverseMS(1.8);

  // To make the robot rotate to the right (Clock-Wise) at the specified speed in m/s
   
  robot.rotateRightMS(2.3);

  // To make the robot rotate left (Counter Clock-Wise) at the specified speed in m/s
   
  robot.rotateLeftMS(2.4);
  
  //  - - - - - - - - - - - - -

  // To read the speed of a specific wheel in the specified unit:

  float velocidade=0;

  velocidade = robot.getLeftRPS();

  velocidade = robot.getRightRPS();
  
  velocidade = robot.getLeftRPM();

  velocidade = robot.getRightRPM();

  velocidade = robot.getLeftMS();

  velocidade = robot.getRightMS();
}
