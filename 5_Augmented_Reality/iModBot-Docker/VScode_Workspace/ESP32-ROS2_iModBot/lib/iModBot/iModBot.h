
/* Revision 3 */

#ifndef iModBot_h
#define iModBot_h

#include "Arduino.h"
#include <FunctionalInterrupt.h>
#include "E32_PC0.h"  //to controll ESP32 pulse counter module (unit 0)
#include "E32_PC1.h"  //to controll ESP32 pulse counter module (unit 1)

// to do: remove extern declaration and move the autoDrive(byte) function to the public of iModBot class, test changes afterwards.
extern byte autoDrive(byte);  //to check the sensors and do autoDrive related tasks

// ISR functions can't be associated with a class, they are asynchronous. They can still interact with class functions and variables by using the scope resolution operator and other tools.
void IRAM_ATTR _think(); // private function to handle interrupts triggered by the IR sensors, used by the auto driving functionality
void IRAM_ATTR onTimer(); // 500 ms timer used to adjust the wheel's speed, activated by the user by calling "enSpeedAdj()" function.

class iModBot
{
  public:
    /*  ATTENTION */
    /* There are several variables declared public but none of them should be set by the user.
       They were declared public just so the ISR functions could access them.
       User can read this variables but it is advised not to change them.
    */

    //functions to edit pin configuration, must be called before begin()
    void editWheelPin(uint8_t , uint8_t );
    void editMotorPin(uint8_t , uint8_t , uint8_t , uint8_t );
    void editUltrasonicPin(uint8_t , uint8_t );
    void editSensorPin(uint8_t , uint8_t , uint8_t , uint8_t , uint8_t , uint8_t , uint8_t );

    void setStopDistance(byte );  // distance read by the ultrasonic sensor at which the robot will stop, valid only for the incorporated line following feature

    void endAutoDrive();  // detach the external interruptions that call the _think() function
    void beginAutoDrive();  // attaches the interruptions that call the _think() function
    void begin(); // Does the pin, PWM and pulse counter module configuration

    byte distance();  // returns a byte holding the measured distance (in centimeter) by the HC-SR04 Ultrasonic sensor
    void turnLeft(uint16_t ); //turns n degrees
    void turnRight(uint16_t );  //turns n degrees
    
    //info you can send: 1 do half turn, 2 turn left'line, 3 turn right'line, 4 forward, 5 reverse
    byte autoDrive(byte); //returns 1 when multiple lines or obstacle found, returns 2 if obstacle is found
    void steerLeft(byte ); //turns very slightly without stopping or reversing any of the wheels
    void steerRight(byte );  //turns very slightly without stopping or reversing any of the wheels
    void rotateLeft(byte ); //keeps turning at n Dutty
    void rotateRight(byte );  //keeps turning at n Dutty
    void stopMotors();  //self explanatory
    void forward(byte );  //self explanatory
    void reverse(byte );  //self explanatory
    void leftWheel(short ); //controls the left wheel, positive values make it move forward and vice-versa, a 0 stops the motors. Ranges from -255 to 255
    void rightWheel(short );  //controls the right wheel, positive values make it move forward and vice-versa, a 0 stops the motors. Ranges from -255 to 255

    //rev2 added
    void decelerate(byte );
    void decelerateSide(bool ,byte );
    void accelerate(byte );
    void accelerateSide(bool ,byte );

    //for sensors
    void clearEncoderCount(); //self explanatory
    static int16_t getLeftEncoderCount();  //self explanatory
    static int16_t getRightEncoderCount(); //self explanatory
    byte checkSensors();
    void disableCLP();  // disables the checking of the CLP switch of the autodrive functionality
    void disableNear(); // disables the checking of the Near sensor of the autodrive functionality
    void disableUltrasonic(); // disables the checking of the ultrasonic sensor of the autodrive functionality
    void setSpeeds(byte ,byte ,byte); // sets the fastest, medium and lower speeds used by the various functions, including the autodrive functionality
    void noLineDelay(uint );  // it's the delay (in ms) of how much time the robot will keep moving after acknowledging that there isn't a line to follow, very useful since the line may be between two sensors at a moment.
    
    bool readCLP();
    bool readNear();
    bool readS1();
    bool readS2();
    bool readS3();
    bool readS4();
    bool readS5();

    volatile static byte _LS2;  //
    volatile static byte _MS3;
    volatile static byte _RS4;
    volatile static byte _CLP;
    volatile static int8_t _speedAdjR;  //
    volatile static int8_t _speedAdjL;  //

    volatile static byte _wheelL; //Left wheel encoder pin
    volatile static byte _wheelR; //Right wheel encoder pin
    
    //added in rev2 
    void forwardRPS(float );
    void reverseRPS(float );
    void rotateRightRPS(float );
    void rotateLeftRPS(float );

    void forwardRPM(float );
    void reverseRPM(float );
    void rotateRightRPM(float );
    void rotateLeftRPM(float );

    void forwardMS(float );
    void reverseMS(float );
    void rotateRightMS(float );
    void rotateLeftMS(float );

    void enSpeedAdj();  //enables to use RPS, RPM and m/s based functions as well as matching the wheel's speed (through an ISR)

    float getLeftRPS();
    float getRightRPS();
    float getLeftRPM();
    float getRightRPM();
    float getLeftMS();
    float getRightMS();
    
    void moveAt(byte, byte , float ); 

    //added in rev2 
    // take note, at the time this library was developed ESP32 didn't support the use of float type variables inside timer ISR (08/06/2020).
    volatile static byte _mov;  // Saves the direction of both wheels in Hexadecimal, upper 4 bits are for left wheel and lower 4 are right wheel's, 0x1 forward, 0x0 stopped, 0x2 backwards
    volatile static byte _rPWM; // Saves the current PWM value set to the right wheel
    volatile static byte _lPWM; // Saves the current PWM value set to the left wheel
    volatile static byte _minSpeed; // Minimun PWM duty required to move both wheels
    volatile static bool _bootFlag; // Self explanatory
    volatile static uint _speedRPS; // Speed in "RPS * 100" set by user, multiplied by 100 to avoid using float type variables inside ISR
    volatile static bool _canAdjustRPS; // flag to allow RPS adjustment, enabled (1) disabled (0) by the library at certain events
    volatile static bool _RPSenabled; // flag to signal there is an ISR in charge of matching wheel's speed 
    volatile static bool _pwmAdjustDisabled;  // flag that disables (when set to 1) the matching of the wheel's speed inside the ISR (when not using the RPS, RPM and m/s based functions)
    volatile static bool _canAdjustPWM; // flag to allow the matching of the wheel's speed, enabled (1) disabled (0) by the library at certain events

    volatile static uint leftRPS; // speed of the left wheel in RPS*100
    volatile static uint rightRPS;  // speed of the Right wheel in RPS*100

  private:

    bool _checkMinSpeed();  // Check the minimun duty-cycle needed to turn both wheels
    void _config();   // set's the needed input and output pins.
    bool _ec(); // extra-check, private function used by auto driving functionality

    //Next three variables are self explanatory, they are changed by the functions mentioned before
    bool _disableUltrasonicSensor = 0;  
    bool _disableNearSensor = 0;  
    bool _disableCLPSensor = 0; 

    bool _maneuver = 1; // to know if the robot did a maneuver, used to adjust the wheels speed in the auto driving functionality
    bool _lastEncoderOffsetMatch = 0;
    bool _fixDirAlways = 1;
    byte _encoderAhead = 0;
    bool _encoderOsc = 0;
    
    byte _LLS1 = 25;
    byte _RRS5 = 34;
    byte _Near = 36;

    //motor pins IN1&IN2 right engine
    byte _IN1 = 4; 
    byte _IN2 = 16;
    //IN3&IN4 left engine
    byte _IN3 = 17;
    byte _IN4 = 18;
    //IR optical interruption sensor pins, 20 wholes
    /* byte _wheelL = 26;
    byte _wheelR = 27; */

    byte _trig = 14;
    byte _echo = 12;

    byte _lastLinePos=0; // tells which sensor "saw" the line last time 1 to 5
    byte _stopDistance = 9;

    byte _fastSpeed = 255;
    byte _averageSpeed = 200;
    byte _slowSpeed = 180;

    uint _dif=750;  // time it will take to stop the robot after no line is found
    uint _invDel=100; //delay to wait before inverting motors polarity

};




#endif