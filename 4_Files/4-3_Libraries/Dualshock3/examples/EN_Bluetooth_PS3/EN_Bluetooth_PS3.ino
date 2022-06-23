#include <Dualshock3Controller.h>

#define EN_1 4                 // Motor 1 -
#define EN_2 16                // Motor 1 +
#define EN_3 17                // Motor 2 -
#define EN_4 18                // Motor 2 +
#define trig 12                // Trigger HC-SR04
#define echo 14                // Echo HC-SR04
#define builtin_led 2          // Built-in LED in ESP32
#define resolution 8           // Resolution PWN
#define frequency 5000         // Frequency PWM
#define ramp_acceleration 0    // Accelerate by 1 (speed) every 'x' ms [MIN: 0 ms] [MAX: 255 ms]
#define ramp_deceleration 0    // Decelerate by 1 (speed) every 'x' ms [MIN: 0 ms] [MAX: 255 ms]
#define maximum_speed 255      // Max speed PWM [MIN: 0] [MAX: 255]
#define minimum_speed 100      // Min speed PWM [MIN: 0] [MAX: 255]
#define rotate_speed 180       // Rotating speed PWM [MIN: 0] [MAX: 255]
#define deadzone 10            // Deadzone analog stick [MIN: 0] [MAX: 127]
#define steering_sensivity 2.0 // The lower this number is, the more sensitive the car is to cornering [MIN: 1.0]

Dualshock3Controller DS3(EN_1, EN_2, EN_3, EN_4, trig, echo, builtin_led, resolution, frequency,
                         ramp_acceleration, ramp_deceleration, maximum_speed, minimum_speed, rotate_speed,
                         deadzone, steering_sensivity);

void setup()
{
  Ps3.begin("01:01:01:01:01:01"); // PS3 Controller MAC Address. This can be changed with SixAxisPairTool
  while (!Ps3.isConnected()) {}   // Loops here until PS3 controller is connected
}

void loop()
{
  DS3.drive();    // Function to drive the robot with Cross, Square, R2 and L2. D-pad and left analog to steer
  DS3.features(); // Features to enable or disable Ultrassonic sensor. Triangle
}