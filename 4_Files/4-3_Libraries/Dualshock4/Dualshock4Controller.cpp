#include "Dualshock4Controller.h"

// #define DEBUG 1
// #define TIMINGS_ACCEL 1
// #define TIMINGS_DECEL 1

Dualshock4Controller::Dualshock4Controller(uint8_t motor_pin_1,
                                           uint8_t motor_pin_2,
                                           uint8_t motor_pin_3,
                                           uint8_t motor_pin_4,
                                           uint8_t trig_pin,
                                           uint8_t echo_pin,
                                           uint8_t led_pin,
                                           uint8_t resolution,
                                           uint16_t frequency,
                                           uint8_t rampIntervalAcel,
                                           uint8_t rampIntervalDecel,
                                           uint8_t max_speed,
                                           uint8_t min_speed,
                                           uint8_t rotate_speed,
                                           uint8_t DEADZONE,
                                           float steeringSensivity)
{
  this->motor_pin_1 = motor_pin_1;
  this->motor_pin_2 = motor_pin_2;
  this->motor_pin_3 = motor_pin_3;
  this->motor_pin_4 = motor_pin_4;
  this->trig_pin = trig_pin;
  this->echo_pin = echo_pin;
  this->led_pin = led_pin;
  this->resolution = resolution;
  this->frequency = frequency;
  this->rampIntervalAcel = rampIntervalAcel;
  this->rampIntervalDecel = rampIntervalDecel;
  this->max_speed = max_speed;
  this->min_speed = min_speed;
  this->rotate_speed = rotate_speed;
  this->DEADZONE = DEADZONE;
  this->steeringSensivity = steeringSensivity;
  Dualshock4Controller::init();

#ifdef DEBUG
  Serial.begin(115200);
  pins_settings();
#endif
}

Dualshock4Controller::~Dualshock4Controller()
{
}

void Dualshock4Controller::pins_settings()
{
  Serial.println(led_pin);
  Serial.println(motor_pin_1);
  Serial.println(motor_pin_2);
  Serial.println(motor_pin_3);
  Serial.println(motor_pin_4);
  Serial.println(trig_pin);
  Serial.println(echo_pin);
  Serial.println(frequency);
  Serial.println(resolution);
  Serial.println(rampIntervalAcel);
  Serial.println(rampIntervalDecel);
  Serial.println(max_speed);
  Serial.println(min_speed);
  Serial.println(DEADZONE);
  Serial.println(steeringSensivity);
}

void Dualshock4Controller::init()
{
  pinMode(led_pin, OUTPUT);            // LED Built-in ESP32-DevKitC-V1
  pinMode(motor_pin_1, OUTPUT);        // Right -
  pinMode(motor_pin_2, OUTPUT);        // Right +
  pinMode(motor_pin_3, OUTPUT);        // Left -
  pinMode(motor_pin_4, OUTPUT);        // Left +
  pinMode(trig_pin, OUTPUT);           // HC-SR04 Trigger
  pinMode(echo_pin, INPUT);            // HC-SR04 Echo
  ledcSetup(0, frequency, resolution); // (PWM Channel 0, Frequency, Resolution)
  ledcAttachPin(motor_pin_1, 0);       // (Pin, PWM Channel)
  ledcSetup(1, frequency, resolution); // (PWM Channel 1, Frequency, Resolution)
  ledcAttachPin(motor_pin_2, 1);       // (Pin, PWM Channel)
  ledcSetup(2, frequency, resolution); // (PWM Channel 2, Frequency, Resolution)
  ledcAttachPin(motor_pin_3, 2);       // (Pin, PWM Channel)
  ledcSetup(3, frequency, resolution); // (PWM Channel 3, Frequency, Resolution)
  ledcAttachPin(motor_pin_4, 3);       // (Pin, PWM Channel)
}

void Dualshock4Controller::drive()
{
  if (!disableUltrasonicSensor)
  {
    distanceSR04();
    if (distance < stopDistance)
    {
      speed = 0;
      flushVar();
      move();
      reverse_start = true;
    }
    while (reverse_start == true)
    {
      speed = min_speed;
      state = 2;
      move();
      distanceSR04();
      if (distance > stopDistance)
      {
        reverse_start = false;
        speed = 0;
        flushVar();
        move();
      }
    }
  }

  if (PS4.Cross()) // Drive
  {
    button.cross = true;
    if (state == 2 && speed > idle)
    {
      can_accelerate = false;
      speed--;
    }
    else if (speed <= idle)
      can_accelerate = true;
    if (can_accelerate)
      state = 1;
    startDecelerate = false;
    startAccelerate = true;
  }
  else if (!PS4.Cross() && button.cross == true)
  {
    button.cross = false;
    startDecelerate = true;
    startAccelerate = false;
    can_accelerate = true;
    if (speed != max_speed)
      flushTimings();
  }

  if (PS4.Square() && (!PS4.Cross() || !PS4.R2())) // Reverse
  {
    button.square = true;
    if (state == 1 && speed > idle)
    {
      can_accelerate = false;
      speed--;
    }
    else if (speed <= idle)
      can_accelerate = true;
    if (can_accelerate)
      state = 2;
    startDecelerate = false;
    startAccelerate = true;
  }
  else if (!PS4.Square() && button.square == true)
  {
    button.square = false;
    startDecelerate = true;
    startAccelerate = false;
    can_accelerate = true;
    if (speed != max_speed)
      flushTimings();
  }

  if (PS4.R2Value()) // Drive with analog R2
  {
    button.r2 = true;
    if (state == 2 && speed > idle)
    {
      can_accelerate = false;
      speed--;
    }
    else if (speed <= idle)
      can_accelerate = true;
    if (can_accelerate)
      state = 1;
    analogSpeed = PS4.R2Value();
    startDecelerate = false;
    startAccelerateAnalog = true;
  }
  else if (!PS4.R2() && button.r2 == true)
  {
    button.r2 = false;
    startDecelerate = true;
    startAccelerateAnalog = false;
    can_accelerate = true;
    if (speed != max_speed)
      flushTimings();
  }

  if (PS4.L2Value() && (!PS4.Cross() || !PS4.R2Value())) // Reverse with analog L2
  {
    button.l2 = true;
    if (state == 1 && speed > idle)
    {
      can_accelerate = false;
      speed--;
    }
    else if (speed <= idle)
      can_accelerate = true;
    if (can_accelerate)
      state = 2;
    analogSpeed = PS4.L2Value();
    startDecelerate = false;
    startAccelerateAnalog = true;
  }
  else if (!PS4.L2() && button.l2 == true)
  {
    button.l2 = false;
    startDecelerate = true;
    startAccelerateAnalog = false;
    can_accelerate = true;
    if (speed != max_speed)
      flushTimings();
  }

  if (startAccelerateAnalog)
    accelerateAnalog();
  if (startAccelerate)
    accelerate();
  if (startDecelerate)
    decelerate();

  steering();
  speed_left_motor = speed - (steer_left / (float)steeringSensivity);
  speed_right_motor = speed - (steer_right / (float)steeringSensivity);
  move();

#ifdef DEBUG
  // Serial.printf("Speed: %d\t steer_left: %d\t steer_right: %d\n\r", speed, steer_left, steer_right);
  Serial.printf("Left motor: %d\t Right motor: %d\t Driving state: %d\t Accelerate: %d\t Decelerate: %d\n\r", speed_left_motor, speed_right_motor, state, startAccelerate, startDecelerate);
#endif
}

void Dualshock4Controller::features()
{
  if (PS4.Triangle() && button.triangle == false)
    button.triangle = true;
  else if (!PS4.Triangle() && button.triangle == true)
  {
    button.triangle = false;
    disableUltrasonicSensor = !disableUltrasonicSensor;
  }

  if (PS4.Circle() && button.circle == false)
    button.circle = true;
  else if (!PS4.Circle() && button.circle == true)
  {
    button.circle = false;
    // do something
  }

  if (PS4.Up() && button.up == false)
    button.up = true;
  else if (!PS4.Up() && button.up == true)
  {
    button.up = false;
    uint8_t battery = map(PS4.Battery(), 0, 10, 0, 100);

    /*========================================================================================*/
    /*                                       BATTERY                                          */
    /*========================================================================================*/
    /*  For a better reading, instead of using gradient colors, it was used fixed colors such as:
        RED: battery below 20%
        ORANGE: battery above 20% and below 40%
        YELLOW: battery above 40% and below 60%
        GREEN: battery above 60% and below 80%
        BLUE: battery above 80%
        WHITE: battery is charging                                                            */
    /*========================================================================================*/

    switch (PS4.Charging())
    {
    case true:
      DualShock4_Led_WHITE();
      break;
    case false:
      if (battery <= 20)
        DualShock4_Led_RED();
      else if (battery > 21 && battery <= 40)
        DualShock4_Led_ORANGE();
      else if (battery > 41 && battery <= 60)
        DualShock4_Led_YELLOW();
      else if (battery > 61 && battery <= 80)
        DualShock4_Led_GREEN();
      else
        DualShock4_Led_BLUE();
      break;
    }
    PS4.sendToController();
    delay(1000);
    PS4.setLed(red, green, blue);
    PS4.sendToController();
  }

  if (PS4.Down() && button.down == false)
    button.down = true;
  else if (!PS4.Down() && button.down == true)
  {
    button.down = false;
    // do something
  }
}
void Dualshock4Controller::steering()
{
  if (PS4.Right()) // Steer or rotate right
  {
    if (!PS4.Cross() && !PS4.Square() && !PS4.R2() && !PS4.L2() && speed < idle)
    {
      button.right = true;
      state = 4;
      move();
    }
    else
    {
      old_speed = speed;
      steer_right = speed;
    }
  }
  else if (!PS4.Right() && button.right == true)
  {
    button.right = false;
    steer_right += old_speed / (float)steeringSensivity;
    flushVar();
    move();
  }

  if (PS4.Left()) // Steer or rotate left
  {
    if (!PS4.Cross() && !PS4.Square() && !PS4.R2() && !PS4.L2() && speed < idle)
    {
      button.left = true;
      state = 3;
      move();
    }
    else
    {
      old_speed = speed;
      steer_left = speed;
    }
  }
  else if (!PS4.Left() && button.left == true)
  {
    button.left = false;
    steer_left += old_speed / (float)steeringSensivity;
    flushVar();
    move();
  }

  if (abs(PS4.LStickX()) + abs(PS4.LStickY()) > 2) // Steer with left analog stick
  {
    left_analog_x = PS4.LStickX(); // PS4 analog stick comes in int8_t [-128, 127], where x=0 is idle position
    if (left_analog_x < -DEADZONE)
    {
      steer_right = 0;
      steer_left = map(left_analog_x, 0, -128, 0, speed); // From 0 to -128 (LEFT), reduce PWM according to received byte in left wheel to steer left
    }
    else if (left_analog_x > DEADZONE)
    {
      steer_left = 0;
      steer_right = map(left_analog_x, 0, 127, 0, speed); // From 0 to 127 (RIGHT), reduce PWM according to received byte in right wheel to steer right
    }
    else if ((steer_right != 0 || steer_left != 0) && (left_analog_x > -DEADZONE && left_analog_x < DEADZONE) && (!PS4.data.button.left && !PS4.data.button.right)) // If trash values are stores in steer_right or steer_left, and the left analog is in rest position, and both left and right dpads are up, clean vars
    {
      steer_left = 0;
      steer_right = 0;
    }
  }
}

void Dualshock4Controller::accelerate()
{
  uint32_t currentTime = millis();
  if (rampIntervalAcel > 0)
  {
    if ((currentTime - previousTimeAcel) >= rampIntervalAcel)
    {
      previousTimeAcel = currentTime;
      if (can_accelerate)
      {
        if (speed < min_speed)
          speed = min_speed;
        else if (speed != max_speed)
          speed++;
      }
    }
  }
  else
    speed = 255;

#ifdef TIMINGS_ACCEL
  Serial.printf("Time: %d\t Previous time: %d\t Ramp: %d\t Speed: %d\n\r", currentTime, previousTimeAcel, rampIntervalAcel, speed);
#endif
}
void Dualshock4Controller::decelerate()
{
  uint32_t currentTime = millis(); // start counter for ramp generator
  if (rampIntervalDecel > 0)
  {
    if ((currentTime - previousTimeDecel) >= rampIntervalDecel)
    {
      previousTimeDecel = currentTime;
      if (speed > idle) // keep decreasing speed until it reaches the minimum required to move
        speed--;
      else if (speed <= idle) // When speed at its minimum, stop the robot
      {
        speed = 0;
        state = 0;
        startDecelerate = false;
        can_accelerate = true;
      }
    }
  }
  else
    speed = 0;
}
void Dualshock4Controller::accelerateAnalog()
{
  uint32_t currentTime = millis(); // start counter for ramp generator
  if (rampIntervalDecel > 0)
  {
    if ((currentTime - previousTimeAcel) >= rampIntervalAcel) // when time passed is equal or greater than our definition of "rampIntervalAcel"
    {
      previousTimeAcel = currentTime; // add rampIntervalAcel to previousTimeAcel for next condition pass
      if (can_accelerate)
      {
        if (speed < min_speed)
          speed = min_speed;
        else if (speed < analogSpeed)
          speed++;
        else if (speed > analogSpeed)
          speed--;
      }
    }
  }
  else
    speed = analogSpeed;
}

void Dualshock4Controller::move()
{
  switch (state)
  {
  case 0:            // Stop (coast)
    ledcWrite(0, 0); // R
    ledcWrite(1, 0); // R
    ledcWrite(2, 0); // L
    ledcWrite(3, 0); // L
    break;
  case 1:                            // Forward
    ledcWrite(0, 0);                 // R
    ledcWrite(1, speed_left_motor);  // R
    ledcWrite(2, speed_right_motor); // L
    ledcWrite(3, 0);                 // L
    break;
  case 2:                            // Reverse
    ledcWrite(0, speed_left_motor);  // R
    ledcWrite(1, 0);                 // R
    ledcWrite(2, 0);                 // L
    ledcWrite(3, speed_right_motor); // L
    break;
  case 3:                       // Rotate right
    ledcWrite(0, rotate_speed); // R
    ledcWrite(1, 0);            // R
    ledcWrite(2, rotate_speed); // L
    ledcWrite(3, 0);            // L
    break;
  case 4:                       // Rotate left
    ledcWrite(0, 0);            // R
    ledcWrite(1, rotate_speed); // R
    ledcWrite(2, 0);            // L
    ledcWrite(3, rotate_speed); // L
    break;
  }
}

void Dualshock4Controller::flushVar()
{
  steer_right = 0;
  steer_left = 0;
  state = 0;
}
void Dualshock4Controller::flushTimings()
{
  previousTimeDecel = 0;
  previousTimeAcel = 0;
}

void Dualshock4Controller::distanceSR04()
{
  digitalWrite(trig_pin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trig_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_pin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echo_pin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2;        // Speed of sound wave divided by 2
  distance = constrain(distance, 2, 255); // Limit distance to 2.55m
}

void Dualshock4Controller::DualShock4_Led_RED()
{
  PS4.setLed(255, 0, 0);
}
void Dualshock4Controller::DualShock4_Led_ORANGE()
{
  PS4.setLed(255, 140, 0);
}
void Dualshock4Controller::DualShock4_Led_GREEN()
{
  PS4.setLed(0, 255, 0);
}
void Dualshock4Controller::DualShock4_Led_YELLOW()
{
  PS4.setLed(255, 255, 0);
}
void Dualshock4Controller::DualShock4_Led_PINK()
{
  PS4.setLed(255, 192, 203);
}
void Dualshock4Controller::DualShock4_Led_PURPLE()
{
  PS4.setLed(255, 0, 255);
}
void Dualshock4Controller::DualShock4_Led_AQUA()
{
  PS4.setLed(0, 255, 255);
}
void Dualshock4Controller::DualShock4_Led_WHITE()
{
  PS4.setLed(255, 255, 255);
}
void Dualshock4Controller::DualShock4_Led_BLUE()
{
  PS4.setLed(0, 0, 255);
}