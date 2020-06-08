
/* Revision 2 */

#include "iModBot.h"

/*
  INPUTS ONLY
  (No internall pulls)  
    GPIO34
    GPIO35
    GPIO36
    GPIO37
    GPIO38
    GPIO39

*/
//#define DEBUG 1
#define _hopTime 8   // miliseconds to wait before increment/decrement the PWM value
#define _decelPWM 10 // PWM decrease amount during deceleration
#define _accelPWM 10 // PWM increase during acceleration

#define PWM_Frequency 5000 // this variable is used to define the time period, up to 312.5kHz
#define _IN1_PWM_channel 0 // this variable is used to select the channel number
#define _IN2_PWM_channel 1 // there are 15 channels, choose between 0 to 15 (not related to the pins)
#define _IN3_PWM_channel 2 //
#define _IN4_PWM_channel 3 //
#define PWM_RESOUTION 8    // this will define the resolution of the signal which is 8 in this case

#define PWM 1               //by defining "PWM" we'll be using the PWM function to control the motors
#define doSpeedAdjustment 1 // enables the functions to match the wheels speed
#define encoderOffset 1     // used to match the wheels speed, this "1" is how much (encoder pulses) a wheel can be ahead or behind the other

//used in the timer ISR associated function
#define stepsPerTurn 40
#define wheelDia 67 //mm

// dutty cycles to control the various motor speeds
byte _fastSpeed = 255;
byte _averageSpeed = 225;
byte _slowSpeed = 200;

bool _disableUltrasonicSensor = 0;
bool _disableNearSensor = 0;
bool _disableCLPSensor = 0;

//all the processing to match the wheels speed is done inside the autoDrive() function
extern bool _maneuver = 1;        // remenbers if the robot did any maneuver
bool _lastEncoderOffsetMatch = 0; //remenbers if lasst time the wheels speed matched
bool _fixDirAlways = 1;           //will constantly attempt to match wheels speed

// for speed adjustment
byte _encoderAhead = 0; //4Most Significant Bits -> Left, 4Less SB -> right
bool _encoderOsc = 0;   //warns if the motor speed can't match
uint _eso = 2;          //encoder step offset

volatile int8_t iModBot::_speedAdjR = 0;
volatile int8_t iModBot::_speedAdjL = 0;

byte _stopDistance = 9; // distance in cm at wich the robot will stop

byte _LLS1 = 25;
volatile byte iModBot::_LS2 = 33;
volatile byte iModBot::_MS3 = 32;
volatile byte iModBot::_RS4 = 35;
byte _RRS5 = 34;
volatile byte iModBot::_CLP = 39;
byte _Near = 36;

volatile byte iModBot::_wheelL = 26;
volatile byte iModBot::_wheelR = 27;

volatile byte iModBot::_mov = 0;
volatile byte iModBot::_rPWM = 0;
volatile byte iModBot::_lPWM = 0;
volatile byte iModBot::_minSpeed = 10;
volatile bool iModBot::_bootFlag = 1;
volatile bool iModBot::_canAdjustRPS = 1;

volatile uint iModBot::_speedRPS = 0;
volatile uint iModBot::leftRPS = 0;
volatile uint iModBot::rightRPS = 0;
volatile bool iModBot::_RPSenabled = 0;
volatile  bool iModBot::_pwmAdjustDisabled = 0;
volatile  bool iModBot::_canAdjustPWM = 0;

//motor pins IN1&IN2 right engine
byte _IN1 = 4; //IO15 and 5 are HIGH during boot.
byte _IN2 = 16;
//IN3&IN4 left engine
byte _IN3 = 17;
byte _IN4 = 18;
//IR interruption sensor pins, 20 wholes
/* byte _wheelL = 26;
byte _wheelR = 27; */

byte _trig = 14;
byte _echo = 12;

extern volatile byte _LLP = 0; // tells which sensor "saw" the line last time 1 to 5
/*  0 no line found.
    1 line of left
    2 line on middle
    3 line on right
    4 multiple lines found
    5 obstacle found
    11 Motors will turn left after the _invDel ms
    13 Motors will turn right after the _invDel ms
    14 Just missed a crossroad, reversing..
*/
extern volatile unsigned long _timeOut = 0;

uint _dif = 700;
uint _invDel = 100; //delay to wait before inverting motors polarity (e.g. making both motors move in the oposite direction)
//volatile uint _Right = 0, _Left = 0;

E32_PC0 PC0;
E32_PC1 PC1;

// Timer setup
hw_timer_t *timer = NULL;
volatile SemaphoreHandle_t timerSemaphore;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

int16_t iModBot::getRightEncoderCount()
{                        // returns the number of pulse's of the right encoder
  return PC1.getCount(); // pin 27
}

int16_t iModBot::getLeftEncoderCount()
{                        // returns the number of pulse's of the left encoder
  return PC0.getCount(); // pin 26
}

void iModBot::clearEncoderCount()
{ //set's the number of counted encoder pulse's back to zero
  PC0.clearCount();
  PC1.clearCount();
}

void iModBot::editWheelPin(uint8_t _wheeL, uint8_t _wheeR)
{
  _wheelL = _wheeL;
  _wheelR = _wheeR;
}

void iModBot::editMotorPin(uint8_t _in1, uint8_t _in2, uint8_t _in3, uint8_t _in4)
{
  _IN1 = _in1;
  _IN2 = _in2;
  _IN3 = _in3;
  _IN4 = _in4;
}

void iModBot::editUltrasonicPin(uint8_t _tri, uint8_t _ech)
{
  _trig = _tri;
  _echo = _ech;
}

void iModBot::editSensorPin(uint8_t _SS1, uint8_t _SS2, uint8_t _SS3, uint8_t _SS4, uint8_t _SS5, uint8_t __CLP, uint8_t __Near)
{
  _LLS1 = _SS1;
  _LS2 = _SS2;
  _MS3 = _SS3;
  _RS4 = _SS4;
  _RRS5 = _SS5;
  _Near = __Near;
  _CLP = __CLP;
}

void iModBot::_config()
{
  //For the front Board with 6xIR sensors
  pinMode(_LLS1, INPUT);
  pinMode(_LS2, INPUT);
  pinMode(_MS3, INPUT);
  pinMode(_RS4, INPUT);
  pinMode(_RRS5, INPUT);
  pinMode(_CLP, INPUT);
  pinMode(_Near, INPUT);

  //for the motor conroll
  pinMode(_IN1, OUTPUT);
  pinMode(_IN2, OUTPUT);
  pinMode(_IN3, OUTPUT);
  pinMode(_IN4, OUTPUT);

  //for the wheels IR sensors
  pinMode(_wheelL, INPUT);
  pinMode(_wheelR, INPUT);

  //for the ultrasonic sensor
  pinMode(_trig, OUTPUT);
  pinMode(_echo, INPUT);
}

byte iModBot::distance()
{ // returns the distance (in cm) measured by the ultrasonic sensor
  digitalWrite(_trig, LOW);
  delayMicroseconds(2);

  digitalWrite(_trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(_trig, LOW);

  uint32_t duration = pulseIn(_echo, HIGH);

  uint dist = int(duration * 0.034f / 2.f);
  if (dist > 255)
  {
    return 255;
  }
  else
  {
    return dist;
  }
}

void iModBot::setStopDistance(byte d)
{
  _stopDistance = d;
}

void iModBot::disableCLP()
{
  _disableCLPSensor = 1;
}
void iModBot::disableNear()
{
  _disableNearSensor = 1;
}
void iModBot::disableUltrasonic()
{
  _disableUltrasonicSensor = 1;
}

byte iModBot::checkSensors()
{
  byte data = 0;
  byte d[8] = {0, 0, 0, 0, 0, 0, 0};
  // 0 (impact switch), 1 (far left) , 2 (left) , 3 (middle),
  //code for line following part
  d[6] = !digitalRead(_LLS1);
  d[5] = !digitalRead(_LS2);
  d[4] = !digitalRead(_MS3); // if d[4] is 1, it means there is a line
  d[3] = !digitalRead(_RS4);
  d[2] = !digitalRead(_RRS5);
  d[1] = digitalRead(_CLP);
  d[0] = digitalRead(_Near);

  for (size_t i = 0; i < 7; i++)
  {
    data |= (d[i] << (i + 1));
  }
  return data;
}

void IRAM_ATTR _think()
{
  #ifdef DEBUG
  Serial.print("*");
  #endif

  noInterrupts();

  _maneuver = 1;  //To tell if the robot did any manouver, may be used in autoDrive()

  if (digitalRead(iModBot::_CLP)) // If front switch is pressed
  { //stop all wheels
    ledcWrite(_IN1_PWM_channel, 0);
    ledcWrite(_IN2_PWM_channel, 0);
    ledcWrite(_IN3_PWM_channel, 0);
    ledcWrite(_IN4_PWM_channel, 0);
    _LLP = 5; // to pass information to the autoDrive function
    iModBot::_mov = 0;  // to remenber the direction of each wheel, stopped in this case
    iModBot::_rPWM = 0; // to remenber the PWM value set to the right wheel
    iModBot::_lPWM = 0; // to remenber the PWM value set to the left wheel
    return;
  }
  bool a = 0, b = 0, c = 0;
  a = !digitalRead(iModBot::_LS2);
  b = !digitalRead(iModBot::_MS3);
  c = !digitalRead(iModBot::_RS4);
  // a,b or c is 1 if a line is found.
  if (((a && b) || (b && c) || (a && c)) && (_LLP != 4))
  {
    if (a)
    { //checking if this is a false alarm, may happen during curves
      if (digitalRead(_LLS1))
      { //if no line found on far left, false alarm
        return;
      }
    }
    else if (c)
    {
      if (digitalRead(_RRS5))
      { //if no line found on far right, false alarm
        return;
      }
    }
    ledcWrite(_IN1_PWM_channel, 0);
    ledcWrite(_IN2_PWM_channel, 0);
    ledcWrite(_IN3_PWM_channel, 0);
    ledcWrite(_IN4_PWM_channel, 0);
    iModBot::_mov = 0;
    iModBot::_rPWM = 0;
    iModBot::_lPWM = 0;
    _LLP = 4;
  }
  else if ((!a && b && !c) && (_LLP != 2))
  {
    ledcWrite(_IN1_PWM_channel, 0);
    ledcWrite(_IN2_PWM_channel, 0);
    ledcWrite(_IN3_PWM_channel, 0);
    ledcWrite(_IN4_PWM_channel, 0);
    if (iModBot::_mov != 0x00)
    {
      delay(50);
    }

    /* short c = PC0.getCount();
    short d = PC1.getCount();
    for (byte i = 0; i < 230; i+=15)
    {
      if(c == PC0.getCount())
        ledcWrite(_IN2_PWM_channel, i);

      if(d == PC1.getCount())
        ledcWrite(_IN3_PWM_channel, i);

      delay(400);
    } */

    short a = PC0.getCount(); //get pulse count while stopped
    short b = PC1.getCount();

    ledcWrite(_IN2_PWM_channel, abs(255 - iModBot::_speedAdjR));
    ledcWrite(_IN3_PWM_channel, abs(255 - iModBot::_speedAdjL));

    while (a == PC0.getCount() && b == PC1.getCount())
      ; // proceed to reduce speed once both wheels move

    /*
      Since whe're dealing with the minimun PWM Duty needed we can't adjust the wheel
      speed by decreasing it's PWM, instead we will increase the oposing wheel's PWM.
    */
    //verificar se o ajuste de velocidade não ultropassa o valor máximo de 255
    if (((iModBot::_minSpeed + abs(iModBot::_speedAdjL)) < 255) && ((iModBot::_minSpeed + abs(iModBot::_speedAdjR)) < 255))
    {
      ledcWrite(_IN2_PWM_channel, abs(iModBot::_minSpeed + abs(iModBot::_speedAdjL))); //R Wheel
      ledcWrite(_IN3_PWM_channel, abs(iModBot::_minSpeed + abs(iModBot::_speedAdjR))); //L Wheel
      iModBot::_lPWM = abs(iModBot::_minSpeed + abs(iModBot::_speedAdjR));
      iModBot::_rPWM = abs(iModBot::_minSpeed + abs(iModBot::_speedAdjL));
    }
    else
    {
      ledcWrite(_IN2_PWM_channel, iModBot::_minSpeed);
      ledcWrite(_IN3_PWM_channel, iModBot::_minSpeed);
      iModBot::_lPWM = iModBot::_minSpeed;
      iModBot::_rPWM = iModBot::_minSpeed;
    }

    iModBot::_mov = 0x11;

    _maneuver = 0;
    _LLP = 2;
  }
  else if ((a && !b && !c) && ((_LLP != 1) && (_LLP != 11)))
  {
    if (_LLP == 3 || _LLP == 13 || _LLP == 0)
    { //if motors were rotating right
      ledcWrite(_IN1_PWM_channel, 0);
      ledcWrite(_IN3_PWM_channel, 0);
      delay(50);
#ifdef DEBUG
      Serial.print("Halt!");
#endif
    }
    else
    {
      ledcWrite(_IN1_PWM_channel, 0);
      ledcWrite(_IN2_PWM_channel, 0);
      ledcWrite(_IN3_PWM_channel, 0);
      ledcWrite(_IN4_PWM_channel, 0);
    }

    if (iModBot::_mov != 0x00)
    {
      delay(50);
    }

    short a = PC0.getCount(); //get pulse count while stopped
    short b = PC1.getCount();

    ledcWrite(_IN2_PWM_channel, abs(255 - iModBot::_speedAdjR));
    ledcWrite(_IN4_PWM_channel, abs(255 - iModBot::_speedAdjL));

    while (a == PC0.getCount() || b == PC1.getCount())
      ;

    //verificar se o ajuste de velocidade não ultropassa o valor máximo de 255
    if (((iModBot::_minSpeed + abs(iModBot::_speedAdjL)) < 255) && ((iModBot::_minSpeed + abs(iModBot::_speedAdjR)) < 255))
    {
      ledcWrite(_IN2_PWM_channel, iModBot::_minSpeed + abs(iModBot::_speedAdjL));
      ledcWrite(_IN4_PWM_channel, iModBot::_minSpeed + abs(iModBot::_speedAdjR));
      iModBot::_lPWM = iModBot::_minSpeed + abs(iModBot::_speedAdjR);
      iModBot::_rPWM = iModBot::_minSpeed + abs(iModBot::_speedAdjL);
    }
    else
    {
      ledcWrite(_IN2_PWM_channel, iModBot::_minSpeed);
      ledcWrite(_IN4_PWM_channel, iModBot::_minSpeed);
      iModBot::_lPWM = iModBot::_minSpeed;
      iModBot::_rPWM = iModBot::_minSpeed;
    }

    iModBot::_mov = 0x21;
    _LLP = 1;

    /* else
    {
      //right engine forward
      ledcWrite(_IN1_PWM_channel, 0);
      ledcWrite(_IN2_PWM_channel, _averageSpeed); // fast
      //left engine reverse
      ledcWrite(_IN3_PWM_channel, 0);
      ledcWrite(_IN4_PWM_channel, _slowSpeed); // slow
      iModBot::_mov = 0;
      _LLP = 1;
    } */
  }
  else if ((!a && !b && c) && ((_LLP != 3) && (_LLP != 13)))
  {
    if (_LLP == 1 || _LLP == 11 || _LLP == 0)
    {
      ledcWrite(_IN2_PWM_channel, 0);
      ledcWrite(_IN4_PWM_channel, 0);
      delay(50);
#ifdef DEBUG
      Serial.print("Halt!");
#endif
    }
    else
    {
      ledcWrite(_IN1_PWM_channel, 0);
      ledcWrite(_IN2_PWM_channel, 0);
      ledcWrite(_IN3_PWM_channel, 0);
      ledcWrite(_IN4_PWM_channel, 0);
    }

    if (iModBot::_mov != 0x00)
    {
      delay(50);
    }

    short a = PC0.getCount();
    short b = PC1.getCount();

    ledcWrite(_IN1_PWM_channel, abs(255 - iModBot::_speedAdjR));
    ledcWrite(_IN3_PWM_channel, abs(255 - iModBot::_speedAdjL));

    while (a == PC0.getCount() || b == PC1.getCount())
      ;

    //verificar se o ajuste de velocidade não ultropassa o valor máximo de 255
    if (((iModBot::_minSpeed + abs(iModBot::_speedAdjL)) < 255) && ((iModBot::_minSpeed + abs(iModBot::_speedAdjR)) < 255))
    {
      ledcWrite(_IN1_PWM_channel, iModBot::_minSpeed + abs(iModBot::_speedAdjL));
      ledcWrite(_IN3_PWM_channel, iModBot::_minSpeed + abs(iModBot::_speedAdjR));
      iModBot::_lPWM = iModBot::_minSpeed + abs(iModBot::_speedAdjR);
      iModBot::_rPWM = iModBot::_minSpeed + abs(iModBot::_speedAdjL);
    }
    else
    {
      ledcWrite(_IN1_PWM_channel, iModBot::_minSpeed);
      ledcWrite(_IN3_PWM_channel, iModBot::_minSpeed);
      iModBot::_lPWM = iModBot::_minSpeed;
      iModBot::_rPWM = iModBot::_minSpeed;
    }

    iModBot::_mov = 0x12;
    _LLP = 3;
    /* else
    {
      //right engine reverse
      ledcWrite(_IN1_PWM_channel, _slowSpeed); // slow
      ledcWrite(_IN2_PWM_channel, 0);
      //left engine forward
      ledcWrite(_IN3_PWM_channel, _averageSpeed); // fast
      ledcWrite(_IN4_PWM_channel, 0);
      iModBot::_mov = 0; iModBot::_rPWM=0; iModBot::_lPWM=0;
      _LLP = 3;
    } */
  }
  else if ((!a && !b && !c) && (_LLP != 0))
  {
    if (((_LLP == 1) || (_LLP == 11)) && (!digitalRead(_LLS1)))
    {
      ;
    }
    else if (((_LLP == 3) || (_LLP == 13)) && (!digitalRead(_RRS5)))
    {
      ;
    }
    else
    {
      _LLP = 0;
      _timeOut = millis();
    }
  }
  interrupts();
#ifdef DEBUG
  Serial.println(_LLP);
#endif
}

void iModBot::beginAutoDrive()
{
  //Just in case the timer ISR was previously activated
  _canAdjustRPS=0;
  _canAdjustPWM=1;

  noInterrupts();
  if (_bootFlag == 1)
  { // Find lowest usable PWM dutty signal
    _bootFlag = _checkMinSpeed();
  }
  Serial.print(_minSpeed);
  interrupts();
  //attachInterrupt(digitalPinToInterrupt(_LLS1), _think, CHANGE);
  attachInterrupt(digitalPinToInterrupt(_LS2), _think, CHANGE);
  attachInterrupt(digitalPinToInterrupt(_MS3), _think, CHANGE);
  attachInterrupt(digitalPinToInterrupt(_RS4), _think, CHANGE);
  //attachInterrupt(digitalPinToInterrupt(_RRS5), _think, CHANGE);
  attachInterrupt(digitalPinToInterrupt(_CLP), _think, RISING);
  //begin();
}

void iModBot::endAutoDrive()
{
  detachInterrupt(digitalPinToInterrupt(_LS2));
  detachInterrupt(digitalPinToInterrupt(_MS3));
  detachInterrupt(digitalPinToInterrupt(_RS4));
  detachInterrupt(digitalPinToInterrupt(_CLP));
}

byte iModBot::autoDrive(byte order)
{
  byte returnFlag = 0;
  //static byte _lLLP=_LLP;
#ifdef doSpeedAdjustment
  //adjust speed, #scheduled for removal or repositioning
  {
    //Serial.print(_maneuver);
    if ((_maneuver == 0) && (_fixDirAlways) && (!_encoderOsc) && (!_RPSenabled))
    { //if the robot drove a straight line, no manouvers made.
      //Serial.print(".");
      int16_t _lV = 0, _rV = 0;
      _lV = PC0.getCount();
      _rV = PC1.getCount();
      if ((_lV > _rV + _eso) || (_lV < _rV - _eso) || (_rV > _lV + _eso) || (_rV < _lV - _eso))
      { //do encoder values match? if they don't
        byte buffEncoderAhead = _encoderAhead;
        if ((_lV - _rV) > 0)
        {
          //left encoder is going faster
          byte buffer = (buffEncoderAhead & 0xF0) >> 4;     // copy "left" data into a buffer
          buffer++;                                         // increment the buffer
          buffEncoderAhead &= 0x0F;                         //erase "left" data
          _encoderAhead = (buffer << 4) | buffEncoderAhead; //save new "left" data

          _speedAdjL++;
          //Serial.println("<<< faster");
        }
        else if ((_rV - _lV) > 0)
        {
          //right encoder is going faster
          byte buffer = (buffEncoderAhead & 0x0F);            //copy "right" data into a buffer
          buffer++;                                           // increment the buffer
          buffEncoderAhead &= 0xF0;                           //erase "right" data
          _encoderAhead = buffEncoderAhead | (buffer & 0x0F); //save new "right" data

          _speedAdjR++;
          //Serial.println(">>> faster");
        }

        if (!_encoderOsc)
        { //verificar se os motores estão  a oscilar
          buffEncoderAhead = _encoderAhead;
          if (((buffEncoderAhead & 0xF0) > 0) && ((buffEncoderAhead & 0x0F) > 0)) //if the speed has been adjusted in both wheels
          {
            if (((buffEncoderAhead & 0xF0) >> 4) == ((buffEncoderAhead & 0x0F) + 1))
            {
              _encoderOsc = 1;
              _speedAdjR = 0;
              _speedAdjL = 0;
              //Serial.println("speed oscilation");
            }
            else if (((buffEncoderAhead & 0xF0) >> 4) == ((buffEncoderAhead & 0x0F) - 1))
            {
              _encoderOsc = 1;
              _speedAdjR = 0;
              _speedAdjL = 0;
              //Serial.println(">>speed oscilation<<");
            }
          }
        }
      }
      else
      { //if they match, let's keep that in mind
        _lastEncoderOffsetMatch = 1;
        //Serial.println("speed does match");
      }
    }
  }
#endif

  /* if(_lLLP!=_LLP) // Make robot move based on _LLP variable
  {
    if(_LLP==1){
      rotateLeft(_averageSpeed);
       _lLLP=_LLP;
    }else if(_LLP==2){
      forward(_fastSpeed);
       _lLLP=_LLP;
    }else if(_LLP==3){
      rotateRight(_averageSpeed);      
       _lLLP=_LLP;
    }else{  //não há instruções para mover o robô
      _lLLP=_LLP;
    }
  } */

  static byte dist = 0;
  byte _dist = distance();
  static unsigned long _distCheckStamp = 0;

  if (millis() - _distCheckStamp > 500)
  {
    dist = _dist;
    _distCheckStamp = millis();
    clearEncoderCount();
  }

  if (_dist < dist)
  {
    dist = _dist;
  }

  byte d[8] = {0, 0, 0, 0, 0, 0, 0}; // 0 (impact switch), 1 (far left) , 2 (left) , 3 (middle),

  if (!_disableCLPSensor)
  {
    d[0] = digitalRead(_CLP); // 0V when nothing found
  }
  else
  {
    d[0] = 0;
  }

  if (!_disableNearSensor)
  {
    d[6] = digitalRead(_Near); // 0V when nothing found
  }
  else
  {
    d[6] = 0;
  }

  if (!_disableUltrasonicSensor && (dist <= _stopDistance))
  {
    stopMotors();
    returnFlag = 2;
    _timeOut = 0;
    _LLP = 5;
  }
  else if ((d[6]) || (d[0]))
  {
    stopMotors();
    returnFlag = 2;
    _timeOut = 0;
    _LLP = 5;
  }
  else if ((_LLP == 5) && ((!_disableUltrasonicSensor) && (dist > _stopDistance))) //The robot has stopped, interrupts ain't trigger with it stopped, let's check if it can advance now
  {
    //Ultrasonic CLP and near sensor have already been checked at this point
    bool a = 0, b = 0, c = 0;
    a = !digitalRead(_LS2);
    b = !digitalRead(_MS3);
    c = !digitalRead(_RS4);
    // a,b or c is 1 if a line is found.
    if (((a && b) || (b && c) || (a && c)) && (_LLP != 4))
    {
      if (a)
      { //checking if this is a false alarm, may happen during curves
        if (!digitalRead(_LLS1))
        { //if  line found on far left, false alarm
          stopMotors();
          _LLP = 4;
        }
      }
      else if (c)
      {
        if (!digitalRead(_RRS5))
        { //if  line found on far right, false alarm
          stopMotors();
          _LLP = 4;
        }
      }
    }
    else if ((!a && b && !c) && (_LLP != 2))
    {
      forward(_fastSpeed);
      _maneuver = 0;
      _LLP = 2;
    }
    else if ((a && !b && !c) && ((_LLP != 1) && (_LLP != 11)))
    {
      rotateLeft(_fastSpeed);
      _LLP = 1;
    }
    else if ((!a && !b && c) && ((_LLP != 3) && (_LLP != 13)))
    {
      rotateRight(_fastSpeed);
      _LLP = 3;
    }
    else if ((!a && !b && !c) && (_LLP != 0))
    {
      _LLP = 0;
    }
  }

  //the next function will stop the robot after x time without a line found.
  if ((_LLP == 0 || _LLP == 14) && (_timeOut != 0)) //if no lines were found on last check and timeout isn't zero
  {
    if (millis() - _timeOut > _dif)
    {
      stopMotors();
      _timeOut = 0;
    }
  }
  /* else if ((_LLP == 13) && (_timeOut != 0))
  {
    if (millis() - _timeOut > _invDel)
    {
      rotateRight(_fastSpeed);
      _timeOut = 0;
    }
  }
  else if ((_LLP == 11) && (_timeOut != 0))
  {
    if (millis() - _timeOut > _invDel)
    {
      rotateLeft(_fastSpeed);
      _timeOut = 0;
    }
  } */

  //The decision making of the robot is done in the _think function interruption
  //The _LLP variable returns information from it.
  switch (_LLP)
  {
  /*  _LLP values:
        0 no line found.
        1 line of left
        2 line on middle
        3 line on right
        4 multiple lines found
        5 obstacle found

        returnFlag values:
        0 - nothing2report
        1 - multiple lines found
        2 - obstacle found
        3 - no lines found
    */
  case 0:
    returnFlag = 3;
    break;

  case 4:
    returnFlag = 1;
    break;

  case 5:
    returnFlag = 2;
    break;

  default:
    returnFlag = 0;
    break;
  }

  //process received instructions
  if ((returnFlag == 1) || (returnFlag == 2))
  { //info you can send: 1 turn right'line, 2 turn left'line, 3 forward, 4 reverse
    switch (order)
    {
    case 1:
      noInterrupts();
      rotateRight(_fastSpeed);
      while (digitalRead(_MS3) && _ec())
        ;
      forward(_fastSpeed);
      interrupts();
      break;

    case 2:
      noInterrupts();
      rotateLeft(_fastSpeed);
      while (digitalRead(_MS3) && _ec())
        ;
      forward(_fastSpeed);
      interrupts();
      break;

    case 3:
      if (returnFlag != 2) //Check if there isn't an obstacle ahead
      {
        forward(_fastSpeed);
      }
      break;

    case 4:
      reverse(_fastSpeed);
      break;

    default:
      break;
    }
  }

  //Clear teh encoders value in the end
  //If the encoders value aren't cleared the code may fail to adjust wheels speed

  //clearEncoderCount();

  return returnFlag;
  /*
  0 - nothing2report
  1 - multiple lines found
  2 - obstacle found
  3 - no lines found
  */
}

bool iModBot::_ec()
{
  if ((!_disableCLPSensor) && (digitalRead(_CLP) == 1)) // a low level means a obstacle
  {
    return 0;
  }
  else if ((!_disableNearSensor) && (digitalRead(_Near) == 1))
  {
    return 0;
  }
  else if (!_disableUltrasonicSensor)
  {
    byte d = distance();
    if (d <= _stopDistance)
      return 0;
  }
  return 1;
}

void iModBot::noLineDelay(uint _k)
{ //After no lines are detected, how much time should it take to stop?
  _dif = _k;
}

void iModBot::rotateLeft(byte speed)
{

  if (_mov == 0x21 && (!_canAdjustRPS))
  {
    decelerate(speed);
  }
  else if (_mov != 0)
  {
    decelerate(0);
  }

  _mov = 0x21;

  accelerate(speed);

  _rPWM = speed;
  _lPWM = speed;
}

void iModBot::rotateRight(byte speed)
{

  if (_mov == 0x12 && (!_canAdjustRPS))
  {
    decelerate(speed);
  }
  else if (_mov != 0)
  {
    decelerate(0);
  }

  /* ledcWrite(_IN1_PWM_channel, speed); //R
  ledcWrite(_IN2_PWM_channel, 0); //R
  ledcWrite(_IN3_PWM_channel, speed); //L
  ledcWrite(_IN4_PWM_channel, 0); //L */

  _mov = 0x12;

  accelerate(speed);

  _rPWM = speed;
  _lPWM = speed;
}

void iModBot::stopMotors()
{

  if (_mov != 0)
    decelerate(0);

  _canAdjustRPS = 0; _canAdjustPWM = 0;
  ledcWrite(_IN1_PWM_channel, 0);
  ledcWrite(_IN2_PWM_channel, 0);
  ledcWrite(_IN3_PWM_channel, 0);
  ledcWrite(_IN4_PWM_channel, 0);
  _mov = 0;
}

void iModBot::forward(byte speed)
{

  if (_mov == 0x11 && (!_canAdjustRPS))
  { // If we still go forward and we're not using RPS
    decelerate(speed);
  }
  else if (_mov != 0)
  {
    decelerate(0);
  }

  //accelerate
  /* ledcWrite(_IN1_PWM_channel, 0); //R
  ledcWrite(_IN2_PWM_channel, abs(speed - _speedAdjR)); //R
  ledcWrite(_IN3_PWM_channel, abs(speed - _speedAdjL)); //L
  ledcWrite(_IN4_PWM_channel, 0); //L */

  _mov = 0x11;

  accelerate(speed);

  _rPWM = speed;
  _lPWM = speed;

  _canAdjustPWM = 1;
  _canAdjustRPS = 0;
}

void iModBot::reverse(byte speed)
{
  if (_mov != 0)
  {
    decelerate(0);
  }
  else if (_mov == 0x22 && (!_canAdjustRPS))
  {
    decelerate(speed);
  }

  //accelerate
  /* ledcWrite(_IN1_PWM_channel, abs(speed - _speedAdjR)); //R
  ledcWrite(_IN2_PWM_channel, 0); //R
  ledcWrite(_IN3_PWM_channel, 0); //L
  ledcWrite(_IN4_PWM_channel, abs(speed - _speedAdjL)); //L */

  _mov = 0x22;

  accelerate(speed);

  _rPWM = speed;
  _lPWM = speed;

  _canAdjustPWM = 1;
  _canAdjustRPS = 0;
}

void iModBot::leftWheel(short dutty)
{
  byte _nPWM = 0;

  byte lb = _mov;
  lb &= 0x0F; //clear left data
  if (dutty > 0)
  {
    lb |= 0x10; //store new left data
  }
  else if (dutty == 0)
  {
    _nPWM = 0;
    lb |= 0x00; //store new left data
  }
  else
  {
    lb |= 0x20; //store new left data
  }

  dutty = abs(dutty);
  _nPWM = dutty & 0x00FF;
  if (lb == _mov)
  {                           //if the direction wasn't changed
    decelerateSide(0, _nPWM); //no need to stop
  }
  else
  {                       //if the direction was changed
    decelerateSide(0, 0); //wheel need to stop first
  }

  _mov = lb; //store to global variable

  accelerateSide(0, _nPWM);
}

void iModBot::rightWheel(short dutty)
{
  byte _nPWM = 0;

  byte lb = _mov;
  lb &= 0xF0; //clear right data
  if (dutty > 0)
  {
    lb |= 0x01; //store new right data
  }
  else if (dutty == 0)
  {
    _nPWM = 0;
    lb |= 0x00; //store new right data
  }
  else
  {
    lb |= 0x02; //store new right data
  }

  dutty = abs(dutty);
  _nPWM = dutty & 0x00FF; // "Resize variable"
  if (lb == _mov)
  {                           //if the direction wasn't changed
    decelerateSide(1, _nPWM); //no need to stop
  }
  else
  {                       //if the direction was changed
    decelerateSide(1, 0); //wheel need to stop first
  }

  _mov = lb; //store to global variable

  accelerateSide(1, _nPWM);
}

void iModBot::forwardRPS(float rps)
{
  moveAt(1, 1, rps);
}
void iModBot::reverseRPS(float rps)
{
  moveAt(3, 1, rps);
}
void iModBot::rotateRightRPS(float rps)
{
  moveAt(2, 1, rps);
}
void iModBot::rotateLeftRPS(float rps)
{
  moveAt(4, 1, rps);
}

void iModBot::forwardRPM(float rpm)
{
  moveAt(1, 2, rpm);
}
void iModBot::reverseRPM(float rpm)
{
  moveAt(3, 2, rpm);
}
void iModBot::rotateRightRPM(float rpm)
{
  moveAt(2, 2, rpm);
}
void iModBot::rotateLeftRPM(float rpm)
{
  moveAt(4, 2, rpm);
}

void iModBot::forwardMS(float mps)
{
  moveAt(1, 3, mps);
}
void iModBot::reverseMS(float mps)
{
  moveAt(3, 3, mps);
}
void iModBot::rotateRightMS(float mps)
{
  moveAt(2, 3, mps);
}
void iModBot::rotateLeftMS(float mps)
{
  moveAt(4, 3, mps);
}

void iModBot::moveAt(byte dir, byte unit, float data)
{
  _canAdjustRPS=0;
  // dir = direction, unit = speed unit, data
  noInterrupts();
  switch (unit)
  {
  case 1: //RPS *100
    _speedRPS = int(data * 100);
    break;

  case 2: //RPM to RPS *100
    _speedRPS = int((data / 60) * 100);
    break;

  case 3: // M/S to RPS *100
    // (((PI*0.067)/60.0)*((leftRPS/100.0)*60.0));
    _speedRPS = ((((60 / (PI * 0.067)) * data) / 60) * 100);
    break;

  default:
    return;
    break;
  }
  interrupts();

  switch (dir)
  {
  case 1:
    forward(_minSpeed);
    break;

  case 2:
    rotateRight(_minSpeed);
    break;

  case 3:
    reverse(_minSpeed);
    break;

  case 4:
    rotateLeft(_minSpeed);
    break;

  default:
    stopMotors();
    break;
  }

  // Set the flag that allows speed adjustment
  _canAdjustRPS = 1;
  _canAdjustPWM = 0;
}

float iModBot::getLeftRPS()
{
  return (leftRPS / 100.0);
}
float iModBot::getRightRPS()
{
  return (rightRPS / 100.0);
}
float iModBot::getLeftRPM()
{
  return ((leftRPS / 100.0) * 60);
}
float iModBot::getRightRPM()
{
  return ((rightRPS / 100.0) * 60);
}
float iModBot::getLeftMS()
{
  return (((PI * 0.067) / 60.0) * ((leftRPS / 100.0) * 60.0));
}
float iModBot::getRightMS()
{
  return (((PI * 0.067) / 60.0) * ((rightRPS / 100.0) * 60.0));
}

void iModBot::accelerate(byte _nPWM)
{
  if (_lPWM > _nPWM && _rPWM > _nPWM)
  {
    return;
  }

  while (1)
  {
    switch (_mov)
    {
    case 0x11:
      ledcWrite(_IN1_PWM_channel, 0);     //R
      ledcWrite(_IN2_PWM_channel, _rPWM); //R
      ledcWrite(_IN3_PWM_channel, _lPWM); //L
      ledcWrite(_IN4_PWM_channel, 0);     //L
      break;

    case 0x22:
      ledcWrite(_IN1_PWM_channel, _rPWM); //R
      ledcWrite(_IN2_PWM_channel, 0);     //R
      ledcWrite(_IN3_PWM_channel, 0);     //L
      ledcWrite(_IN4_PWM_channel, _lPWM); //L
      break;

    case 0x12:
      ledcWrite(_IN1_PWM_channel, _rPWM); //R
      ledcWrite(_IN2_PWM_channel, 0);     //R
      ledcWrite(_IN3_PWM_channel, _lPWM); //L
      ledcWrite(_IN4_PWM_channel, 0);     //L
      break;

    case 0x21:
      ledcWrite(_IN1_PWM_channel, 0);     //R
      ledcWrite(_IN2_PWM_channel, _rPWM); //R
      ledcWrite(_IN3_PWM_channel, 0);     //L
      ledcWrite(_IN4_PWM_channel, _lPWM); //L
      break;

    default:
      break;
    }

    delay(_hopTime); //tempo entre saltos

    if (_rPWM == _nPWM && _lPWM == _nPWM)
    {
      return;
    }

    if ((_rPWM + _accelPWM) > _nPWM)
    {
      _rPWM = _nPWM;
    }
    else
    {
      _rPWM += _accelPWM;
    }
    if ((_lPWM + _accelPWM) > _nPWM)
    {
      _lPWM = _nPWM;
    }
    else
    {
      _lPWM += _accelPWM;
    }
  }
}
void iModBot::accelerateSide(bool _rightSide, byte _nPWM)
{
  if (_lPWM > _nPWM && _rPWM > _nPWM)
  {
    return;
  }

  byte lc = _mov;
  if (_rightSide)
  {
    lc &= 0x0F; //clear left data from copy
    while (1)
    {
      switch (lc)
      {
      case 0x01:
        ledcWrite(_IN1_PWM_channel, 0);     //R
        ledcWrite(_IN2_PWM_channel, _rPWM); //R
        break;

      case 0x02:
        ledcWrite(_IN1_PWM_channel, _rPWM); //R
        ledcWrite(_IN2_PWM_channel, 0);     //R
        break;

      default:
        break;
      }

      delay(_hopTime);

      if (_rPWM == _nPWM)
      {
        return;
      }

      if ((_rPWM + _accelPWM) > _nPWM)
      {
        _rPWM = _nPWM;
      }
      else
      {
        _rPWM += _accelPWM;
      }
    }
  }
  else
  {
    lc &= 0xF0; //clear right data from copy
    while (1)
    {
      switch (lc)
      {
      case 0x10:
        ledcWrite(_IN3_PWM_channel, _lPWM); //L
        ledcWrite(_IN4_PWM_channel, 0);     //L
        break;

      case 0x20:
        ledcWrite(_IN3_PWM_channel, 0);     //L
        ledcWrite(_IN4_PWM_channel, _lPWM); //L
        break;

      default:
        break;
      }

      delay(_hopTime);

      if (_lPWM == _nPWM)
      {
        return;
      }

      if ((_lPWM + _accelPWM) > _nPWM)
      {
        _lPWM = _nPWM;
      }
      else
      {
        _lPWM += _accelPWM;
      }
    }
  }
}

void iModBot::decelerate(byte _nPWM)
{
  if (_rPWM < _nPWM && _lPWM < _nPWM)
  {
    return;
  }

  while (1)
  {
    if ((_rPWM - _decelPWM) < _nPWM)
    {
      _rPWM = _nPWM;
    }
    else
    {
      _rPWM -= _decelPWM;
    }
    if ((_lPWM - _decelPWM) < _nPWM)
    {
      _lPWM = _nPWM;
    }
    else
    {
      _lPWM -= _decelPWM;
    }

    switch (_mov)
    {
    case 0x11:
      ledcWrite(_IN1_PWM_channel, 0);     //R
      ledcWrite(_IN2_PWM_channel, _rPWM); //R
      ledcWrite(_IN3_PWM_channel, _lPWM); //L
      ledcWrite(_IN4_PWM_channel, 0);     //L
      break;

    case 0x22:
      ledcWrite(_IN1_PWM_channel, _rPWM); //R
      ledcWrite(_IN2_PWM_channel, 0);     //R
      ledcWrite(_IN3_PWM_channel, 0);     //L
      ledcWrite(_IN4_PWM_channel, _lPWM); //L
      break;

    case 0x12:
      ledcWrite(_IN1_PWM_channel, _rPWM); //R
      ledcWrite(_IN2_PWM_channel, 0);     //R
      ledcWrite(_IN3_PWM_channel, _lPWM); //L
      ledcWrite(_IN4_PWM_channel, 0);     //L
      break;

    case 0x21:
      ledcWrite(_IN1_PWM_channel, 0);     //R
      ledcWrite(_IN2_PWM_channel, _rPWM); //R
      ledcWrite(_IN3_PWM_channel, 0);     //L
      ledcWrite(_IN4_PWM_channel, _lPWM); //L
      break;

    default:
      break;
    }

    delay(_hopTime); //tempo entre saltos

    if (_rPWM == _nPWM && _lPWM == _nPWM)
    {
      return;
    }
  }
}
void iModBot::decelerateSide(bool _rightSide, byte _nPWM)
{
  byte lc = _mov;
  if (_rightSide)
  {
    lc &= 0x0F; //clear left data from copy
    while (1)
    {
      if ((_rPWM - _decelPWM) < _nPWM)
      {
        _rPWM = _nPWM;
      }
      else
      {
        _rPWM -= _decelPWM;
      }

      switch (lc)
      {
      case 0x01:
        ledcWrite(_IN1_PWM_channel, 0);     //R
        ledcWrite(_IN2_PWM_channel, _rPWM); //R
        break;

      case 0x02:
        ledcWrite(_IN1_PWM_channel, _rPWM); //R
        ledcWrite(_IN2_PWM_channel, 0);     //R
        break;

      default:
        break;
      }

      delay(_hopTime);

      if (_rPWM == _nPWM)
      {
        return;
      }
    }
  }
  else
  {
    lc &= 0xF0; //clear right data from copy
    while (1)
    {
      if ((_lPWM - _decelPWM) < _nPWM)
      {
        _lPWM = _nPWM;
      }
      else
      {
        _lPWM -= _decelPWM;
      }

      switch (lc)
      {
      case 0x10:
        ledcWrite(_IN3_PWM_channel, _lPWM); //L
        ledcWrite(_IN4_PWM_channel, 0);     //L
        break;

      case 0x20:
        ledcWrite(_IN3_PWM_channel, 0);     //L
        ledcWrite(_IN4_PWM_channel, _lPWM); //L
        break;

      default:
        break;
      }

      delay(_hopTime);

      if (_lPWM == _nPWM)
      {
        return;
      }
    }
  }
}

void IRAM_ATTR onTimer()
{ // Increment the counter and set the time of ISR

  portENTER_CRITICAL_ISR(&timerMux);

  static byte feed = 0;
  if (iModBot::_RPSenabled)
  {
    // Since other parts of the library may disable the interrupts
    // two variables will be used to determine the enlapsed time
    static unsigned long lastCheck = 0;   //tempo 
    unsigned long enlapsedTime = millis() - lastCheck;


    uint Rs = 0, Ls = 0; // Speed of right and left wheel in RPS *100 (multiplied by 100 to avoid using float type variables)
    static unsigned short prevLp = 0, prevRp = 0; // measured pulses last time this ISR ran 

    // Using eight extra "unsigned short" variablues (four to each side) it's possible to accelerate the timer down to 100ms 
    // Just so the code always has info from the last 500ms, perhaps create an array.
    unsigned short Lp = 0, Rp = 0; // pulsos decorridos desde ultima medição

    //detect if encoder values have been cleared
    if ((iModBot::getLeftEncoderCount() < prevLp) || (iModBot::getRightEncoderCount() < prevRp))
    {
      Lp = iModBot::getLeftEncoderCount();
      Rp = iModBot::getRightEncoderCount();
    }
    else  // If not, read the gap
    {
      Lp = iModBot::getLeftEncoderCount() - prevLp;
      Rp = iModBot::getRightEncoderCount() - prevRp;
    }

    prevLp = iModBot::getLeftEncoderCount();
    prevRp = iModBot::getRightEncoderCount();
    lastCheck = millis();

    /*
    * Obter as rotações em RPS
    * Fórmula:
    *  f = n * v(rps)
    *  v(rps) = f/v
    *  f = pulsos/tempo
    *  n = numero de ranhuras
    *  
    *  É necessário também verificar se o tempo decorrido não é zero.
    *  
    */
    if ((enlapsedTime != 0))
    {
      Ls = ((Lp / (enlapsedTime / 1000.0)) / 40.0) * 100; //    Ls = (Lp / 40.0 ) / ( enlapsedTime / 1000.0 ) ;
      Rs = ((Rp / (enlapsedTime / 1000.0)) / 40.0) * 100;
    }

    Serial.print("Left: ");
    Serial.print(Ls);
    Serial.print("\t");
    Serial.print("Right: ");
    Serial.println(Rs);

    iModBot::leftRPS = Ls;  // Copy measured speed to class variable
    iModBot::rightRPS = Rs;

    byte __rPWM = 0, __lPWM = 0;  // Declare local variables to adjust PWM
    if (iModBot::_canAdjustRPS ) // If it is allowed to adjust the speed
    {
      __rPWM = iModBot::_rPWM;  
      __lPWM = iModBot::_lPWM;

      // To detect small speed variations and do small speed adjustments 
      byte llb = feed, rlb = (feed & 0b00000111);
      llb = llb >> 3;
      llb &= 0b00000111;
      
      if (Ls < iModBot::_speedRPS)  //If Left wheel speed (Ls)
      {   //decide wether the next PWM step should be 5 or 1
        if (((((llb & 1) == 0) || ((llb & 2) == 0) || ((llb & 4) == 0)) && ((feed & 0x80) == 0x80)) || (__lPWM > 250))
        { //last three cycles we've increased speed, so now let's decrease slowly
          __lPWM = (__lPWM < 255) ? __lPWM + 1 : __lPWM; // Prevent overflow
        }
        else
        {
          __lPWM = (__lPWM < 251) ? __lPWM + 5 : __lPWM; // Prevent overflow
        }
        iModBot::_lPWM = __lPWM;  //save changes
        llb = llb << 1;
        llb |= 0x01;  // Memorize we've increased the PWM value this time
      }
      else if (Ls > iModBot::_speedRPS)
      { //decide wether the next PWM step should be 5 or 1
        if ((((llb & 1) == 1) || ((llb & 2) == 2) || ((llb & 4) == 4)) && ((feed & 0x80) == 0x80))
        { //last three cycles we've increased speed, so now let's decrease slowly
          __lPWM = (__lPWM > 1) ? __lPWM - 1 : __lPWM; // Prevent underflow
        }
        else
        {
          __lPWM = (__lPWM > 5) ? __lPWM - 5 : __lPWM; // Prevent underflow
        }
        iModBot::_lPWM = __lPWM;  //save changes
        llb = llb << 1;
        llb &= 0xFE; // Memorize we've decreased the PWM value this time
      }

      if (Rs < iModBot::_speedRPS)
      { //decide wether the next PWM step should be 5 or 1
        if (((((rlb & 1) == 0) || ((rlb & 2) == 0) || ((rlb & 4) == 0)) && ((feed & 0x80) == 0x80)) || (__rPWM > 250))
        { //last three cycles we've increased speed, so now let's decrease slowly
          __rPWM = (__rPWM < 255) ? __rPWM + 1 : __rPWM; // prevent overflow
        }
        else
        {
          __rPWM = (__rPWM < 251) ? __rPWM + 5 : __rPWM; // prevent overflow
        }
        iModBot::_rPWM = __rPWM;
        rlb = rlb << 1;
        rlb |= 0x01;
      }
      else if (Rs > iModBot::_speedRPS)
      {
        if ((((rlb & 1) == 1) || ((rlb & 2) == 2) || ((rlb & 4) == 4)) && ((feed & 0x80) == 0x80))
        { //last three cycles we've increased speed, so now let's decrease slowly
          __rPWM = (__rPWM > 1) ? __rPWM - 1 : __rPWM; // prevent underflow
        }
        else
        {
          __rPWM = (__rPWM > 5) ? __rPWM - 5 : __rPWM; // prevent underflow
        }
        iModBot::_rPWM = __rPWM;
        rlb = rlb << 1;
        rlb &= 0xFE;
      }
      //Serial.println(llb, BIN);
      feed &= 0;
      rlb &= 0b00000111;
      llb &= 0b00000111;
      feed |= (llb << 3);
      feed &= 0b10111000; //just to make sure the last 3 LSB are 0
      feed |= rlb;
      feed |= 0b10000000;

      switch (iModBot::_mov)
      {
      case 0x11:
        ledcWrite(_IN1_PWM_channel, 0);      //R
        ledcWrite(_IN2_PWM_channel, __rPWM); //R
        ledcWrite(_IN3_PWM_channel, __lPWM); //L
        ledcWrite(_IN4_PWM_channel, 0);      //L
        break;

      case 0x22:
        ledcWrite(_IN1_PWM_channel, __rPWM); //R
        ledcWrite(_IN2_PWM_channel, 0);      //R
        ledcWrite(_IN3_PWM_channel, 0);      //L
        ledcWrite(_IN4_PWM_channel, __lPWM); //L
        break;

      case 0x12:
        ledcWrite(_IN1_PWM_channel, __rPWM); //R
        ledcWrite(_IN2_PWM_channel, 0);      //R
        ledcWrite(_IN3_PWM_channel, __lPWM); //L
        ledcWrite(_IN4_PWM_channel, 0);      //L
        break;

      case 0x21:
        ledcWrite(_IN1_PWM_channel, 0);      //R
        ledcWrite(_IN2_PWM_channel, __rPWM); //R
        ledcWrite(_IN3_PWM_channel, 0);      //L
        ledcWrite(_IN4_PWM_channel, __lPWM); //L
        break;

      default:
        break;
      }
    }
    else if( (!iModBot::_pwmAdjustDisabled) && (iModBot::_canAdjustPWM) )
    {
      static byte fasterWheel = 0; //Which wheel is faster? 0 means none
      const byte offset = 1; // PWM offset, allows one wheel to go faster by the specified PWM amount. 

      if(fasterWheel==0){
        if(Lp > (Rp + offset) ){ //Left wheel is fater
          fasterWheel=1; //left
        }else if(Rp > (Lp + offset) ){ //Right wheel is faster
          fasterWheel=2; //Right
        }
      }
      else
      {
        __rPWM=iModBot::_rPWM;
        __lPWM=iModBot::_lPWM;
        
        Ls = ((Lp / (enlapsedTime / 1000.0)) / 40.0) * 100; 
        Rs = ((Rp / (enlapsedTime / 1000.0)) / 40.0) * 100;
        
        if(Ls > Rs){  // right now, Left wheel is faster
          if(fasterWheel==1){ // if left wheel was the fastest one already
            __lPWM = (__lPWM > 1) ? __lPWM - 1 : __lPWM; // Prevent underflow
            iModBot::_lPWM=__lPWM;
          }else{  // if right wheel was the fastest one, but now it's not
            __rPWM = (__rPWM < 255) ? __rPWM + 1 : __rPWM; // Prevent overflow
            iModBot::_rPWM=__rPWM;
          }
        }else if(Rs > Ls){ // right now Right wheel is faster
          if(fasterWheel==1){ // if left wheel was the fastest one, but now it's not
            __lPWM = (__lPWM < 255) ? __lPWM + 1 : __lPWM; // Prevent overflow
            iModBot::_lPWM=__lPWM;
          }else{  // if right wheel was the fastest one
            __rPWM = (__rPWM > 1) ? __rPWM - 1 : __rPWM; // Prevent underflow
            iModBot::_rPWM=__rPWM;
          }
        }
        /* Serial.print(fasterWheel);
        Serial.print("\t");
        Serial.print("L: ");
        Serial.print(__lPWM);
        Serial.print("\t");
        Serial.print("R: ");
        Serial.println(__rPWM); */

        switch (iModBot::_mov)
        {
        case 0x11:
          ledcWrite(_IN1_PWM_channel, 0);      //R
          ledcWrite(_IN2_PWM_channel, __rPWM); //R
          ledcWrite(_IN3_PWM_channel, __lPWM); //L
          ledcWrite(_IN4_PWM_channel, 0);      //L
          break;

        case 0x22:
          ledcWrite(_IN1_PWM_channel, __rPWM); //R
          ledcWrite(_IN2_PWM_channel, 0);      //R
          ledcWrite(_IN3_PWM_channel, 0);      //L
          ledcWrite(_IN4_PWM_channel, __lPWM); //L
          break;

        case 0x12:
          ledcWrite(_IN1_PWM_channel, __rPWM); //R
          ledcWrite(_IN2_PWM_channel, 0);      //R
          ledcWrite(_IN3_PWM_channel, __lPWM); //L
          ledcWrite(_IN4_PWM_channel, 0);      //L
          break;

        case 0x21:
          ledcWrite(_IN1_PWM_channel, 0);      //R
          ledcWrite(_IN2_PWM_channel, __rPWM); //R
          ledcWrite(_IN3_PWM_channel, 0);      //L
          ledcWrite(_IN4_PWM_channel, __lPWM); //L
          break;

        default:
          break;
        }
      }
      
    }

  }
  /* else if( (!iModBot::_pwmAdjustDisabled) && (iModBot::_canAdjustPWM) ) // --------
  {
    uint Rs = 0, Ls = 0; // velocidade em RPS de cada roda

    const byte offset = 1; // PWM offset, allows one wheel to go faster by the specified PWM amount. 
    static byte fasterWheel = 0; //Which wheel is faster? 0 means none
    int16_t PWMoffset = 0; //PWM to be added or subtracted

    static unsigned short prevLp = 0, prevRp = 0; //pulsos medidos no fim da ultima medição 
    unsigned short Lp = 0, Rp = 0; // pulsos decorridos desde ultima medição

    //detect if encoder values have been cleared
    if ((iModBot::getLeftEncoderCount() < prevLp) || (iModBot::getRightEncoderCount() < prevRp))
    {
      Lp = iModBot::getLeftEncoderCount();
      Rp = iModBot::getRightEncoderCount();
    }
    else  // If not, read the gap
    {
      Lp = iModBot::getLeftEncoderCount() - prevLp;
      Rp = iModBot::getRightEncoderCount() - prevRp;
    }

    if(fasterWheel==0){
      if(Lp > (Rp + offset) ){ //Left wheel is fater
        fasterWheel=1; //left
      }else if(Rp > (Lp + offset) ){ //Right wheel is faster
        fasterWheel=2; //Right
      }
    }
    else
    {
      byte __rPWM = 0, __lPWM = 0;  // Declare local variables to adjust PWM
      __rPWM=iModBot::_rPWM;
      __lPWM=iModBot::_lPWM;
      const uint16_t timerInterval = 500; //value in miliseconds (ms)
      Ls = ((Lp / (timerInterval / 1000.0)) / 40.0) * 100; 
      Rs = ((Rp / (timerInterval / 1000.0)) / 40.0) * 100;
      
      if(Ls > Rs){  // right now Left wheel is faster
        if(fasterWheel==1){ // if left wheel was the fastest one already
          __lPWM = (__lPWM > 1) ? __lPWM - 1 : __lPWM; // Prevent underflow
          iModBot::_lPWM=__lPWM;
        }else{  // if right wheel was the fastest one, but now it's not
          __rPWM = (__rPWM < 255) ? __rPWM + 1 : __rPWM; // Prevent overflow
          iModBot::_rPWM=__rPWM;
        }
      }else if(Rs > Ls){ // right now Right wheel is faster
        if(fasterWheel==1){ // if left wheel was the fastest one, but now it's not
          __lPWM = (__lPWM < 255) ? __lPWM + 1 : __lPWM; // Prevent overflow
          iModBot::_lPWM=__lPWM;
        }else{  // if right wheel was the fastest one
          __rPWM = (__rPWM > 1) ? __rPWM - 1 : __rPWM; // Prevent underflow
          iModBot::_rPWM=__rPWM;
        }
      }

      switch (iModBot::_mov)
      {
      case 0x11:
        ledcWrite(_IN1_PWM_channel, 0);      //R
        ledcWrite(_IN2_PWM_channel, __rPWM); //R
        ledcWrite(_IN3_PWM_channel, __lPWM); //L
        ledcWrite(_IN4_PWM_channel, 0);      //L
        break;

      case 0x22:
        ledcWrite(_IN1_PWM_channel, __rPWM); //R
        ledcWrite(_IN2_PWM_channel, 0);      //R
        ledcWrite(_IN3_PWM_channel, 0);      //L
        ledcWrite(_IN4_PWM_channel, __lPWM); //L
        break;

      case 0x12:
        ledcWrite(_IN1_PWM_channel, __rPWM); //R
        ledcWrite(_IN2_PWM_channel, 0);      //R
        ledcWrite(_IN3_PWM_channel, __lPWM); //L
        ledcWrite(_IN4_PWM_channel, 0);      //L
        break;

      case 0x21:
        ledcWrite(_IN1_PWM_channel, 0);      //R
        ledcWrite(_IN2_PWM_channel, __rPWM); //R
        ledcWrite(_IN3_PWM_channel, 0);      //L
        ledcWrite(_IN4_PWM_channel, __lPWM); //L
        break;

      default:
        break;
      }
    }
  } */
  else //if iModBot::_canAdjustPWM... do PWM adjustment on faster wheel
  {
    feed = 0;
  }

  portEXIT_CRITICAL_ISR(&timerMux);
  
#ifdef DEBUG
  Serial.print("Left: ");
  Serial.print(Ls);
  Serial.print("\t");
  Serial.print("Right: ");
  Serial.println(Rs);

  Serial.print("[");
  Serial.print(iModBot::_speedRPS);
  Serial.print(" ");
  Serial.print(iModBot::_rPWM);
  Serial.print(" ");
  Serial.print(iModBot::_canAdjustRPS);
  Serial.print("] ");
#endif
}

void iModBot::begin()
{
  //configure the pins
  _config();

  // Initialize pulse counter modules
  PC0.begin(_wheelL);
  PC1.begin(_wheelR);

  //setup PWM for motor pins
  ledcSetup(_IN1_PWM_channel, PWM_Frequency, PWM_RESOUTION);
  ledcAttachPin(_IN1, _IN1_PWM_channel);

  ledcSetup(_IN2_PWM_channel, PWM_Frequency, PWM_RESOUTION);
  ledcAttachPin(_IN2, _IN2_PWM_channel);

  ledcSetup(_IN3_PWM_channel, PWM_Frequency, PWM_RESOUTION);
  ledcAttachPin(_IN3, _IN3_PWM_channel);

  ledcSetup(_IN4_PWM_channel, PWM_Frequency, PWM_RESOUTION);
  ledcAttachPin(_IN4, _IN4_PWM_channel);

  ledcWrite(_IN1_PWM_channel, 0);
  ledcWrite(_IN2_PWM_channel, 0);
  ledcWrite(_IN3_PWM_channel, 0);
  ledcWrite(_IN4_PWM_channel, 0);
}
bool iModBot::_checkMinSpeed()
{
  unsigned long ts = millis();
  forward(_minSpeed);
  while (1)
  {
    if (PC0.getCount() != 0 && PC1.getCount() != 0)
    {
      stopMotors();
      if (_minSpeed <= 250)
      {
        _minSpeed += 10;
      }
      return 0;
    }
    if (millis() - ts > 400)
    {
      if (_minSpeed == 255)
      {
        return 1;
      }
      ts = millis();
      _minSpeed += 10;
      _minSpeed = (_minSpeed > 255) ? 255 : _minSpeed;
      PC0.clearCount();
      PC1.clearCount();
      forward(_minSpeed);
    }
  }
}
void iModBot::enSpeedAdj()
{
  /*  Timer Configuration   */

  // Use 1st timer of 4 (counted from zero).
  // Set 80 divider for prescaler (see ESP32 Technical Reference Manual for more
  // info).
  timer = timerBegin(1, 80, true);

  // Attach onTimer function to our timer.
  timerAttachInterrupt(timer, &onTimer, true);

  // Set alarm to call onTimer function every 500 ms (value in microseconds).
  // Repeat the alarm (third parameter)
  timerAlarmWrite(timer, 500000, true);

  // Start an alarm
  timerAlarmEnable(timer);

  noInterrupts();
  _RPSenabled = 1;
  if (_bootFlag == 1)
  { // Find lowest usable PWM dutty signal
    _bootFlag = _checkMinSpeed();
  }
  interrupts();
}

void iModBot::turnLeft(uint16_t deg)
{
  stopMotors();
  if (_bootFlag == 1)
  { // Find lowest usable PWM dutty signal
    _bootFlag = _checkMinSpeed();
  }
  if (deg == 0)
  {
    deg = 90;
  }
  if (deg < 361)
  {
    clearEncoderCount();
    //40 count = 1 turn = 180 deg
    byte steps = ((deg * 80) / 360);
    //rotateLeft(200);

    rightWheel(150);
    while (PC1.getCount() < steps)
      ;
    ledcWrite(_IN1_PWM_channel, 0);
    ledcWrite(_IN2_PWM_channel, 0);
    _mov &= 0xF0;
    _rPWM = 0;
    leftWheel(-150);
    while (PC0.getCount() < steps)
      ;
    ledcWrite(_IN3_PWM_channel, 0);
    ledcWrite(_IN4_PWM_channel, 0);
    _mov = 0x00;
    _lPWM = 0;
    //stopMotors();
  }
}

void iModBot::turnRight(uint16_t deg)
{
  stopMotors();
  if (_bootFlag == 1)
  { // Find lowest usable PWM dutty signal
    _bootFlag = _checkMinSpeed();
  }
  if (deg == 0)
  {
    deg = 90;
  }
  if (deg < 361)
  {
    clearEncoderCount();
    //40 count = 1 turn = 180 deg
    byte steps = ((deg * 80) / 360);

    //rotateRight(200);

    leftWheel(150);
    while (PC0.getCount() < steps)
      ;
    ledcWrite(_IN3_PWM_channel, 0);
    ledcWrite(_IN4_PWM_channel, 0);
    _mov &= 0x0F;
    _lPWM = 0;
    rightWheel(-150);
    while (PC1.getCount() < steps)
      ;
    ledcWrite(_IN1_PWM_channel, 0);
    ledcWrite(_IN2_PWM_channel, 0);
    _mov = 0x00;
    _rPWM = 0;
    //stopMotors();
  }
}

void iModBot::setSpeeds(byte _f, byte _a, byte _s)
{
  _fastSpeed = _f;
  _averageSpeed = _a;
  _slowSpeed = _s;
}

bool iModBot::readCLP()
{
  return digitalRead(iModBot::_CLP);
}
bool iModBot::readNear()
{
  return digitalRead(_Near);
}
bool iModBot::readS1()
{
  return digitalRead(_LLS1);
}
bool iModBot::readS2()
{
  return digitalRead(iModBot::_LS2);
}
bool iModBot::readS3()
{
  return digitalRead(iModBot::_MS3);
}
bool iModBot::readS4()
{
  return digitalRead(iModBot::_RS4);
}
bool iModBot::readS5()
{
  return digitalRead(_RRS5);
}
