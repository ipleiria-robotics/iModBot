#ifndef DUALSHOCK4CONTROLLER_H
#define DUALSHOCK4CONTROLLER_H

#include <PS4Controller.h>
#include <Arduino.h>

// #define DEBUG 1

class Dualshock4Controller
{
private:
    uint8_t motor_pin_1, motor_pin_2, motor_pin_3, motor_pin_4; // Motor pins
    uint8_t trig_pin, echo_pin;                                 // HC-SR04 pins
    uint8_t led_pin;                                            // Built-in LED
    uint8_t DEADZONE;                                           // Deadzone for analog sticks
    uint8_t rampIntervalAcel = 0;                               // Increase speed var by 1 every 'x' ms
    uint8_t rampIntervalDecel = 0;                              // Decrease speed var by 1 every 'x' ms
    uint8_t max_speed;                                          // Maximum PWM speed
    uint8_t min_speed;                                          // Minimum PWM speed at which robot can move
    uint8_t rotate_speed;                                       // Speed at which robot rotates
    uint8_t resolution;                                         // Resolution for PWM
    uint16_t frequency;                                         // Frequency for PWM
    float steeringSensivity;                                    // Closer to 0 means more steering applied

public:
    uint8_t state; // Controls motor states. 5 in total
                   /*
                     State:
                     0 - Stop
                     1 - Forward
                     2 - Reverse
                     3 - Rotate left
                     4 - Rotate right
                   */

    bool can_accelerate = true;                          // Ready to accelerate
    bool reverse_start = false;                          // Start reverse HC-SR04
    bool startDecelerate = false;                        // Allows the robot to decelerate
    bool startAccelerate = false;                        // Allows the robot to accelerate
    bool startAccelerateAnalog = false;                  // Accelerate but for analogs
    bool drivingStyle = false;                           // Switch between driving styles
    bool disableUltrasonicSensor = true;                 // Set "true" to disable this module. Set "false" to enable this module.
    uint32_t previousTimeAcel = 0;                       // Variables for ramp generator
    uint32_t previousTimeDecel = 0;                      // Variables for ramp generator
    uint8_t speed = 0;                                   // Data that is used in formulas for the motors
    uint8_t old_speed = 0;                               // Save old speed to steering
    uint8_t idle = 80;                                   // Current smoother
    uint8_t steer_right = 0, steer_left = 0;             // Variables for steering
    uint8_t speed_left_motor = 0, speed_right_motor = 0; // Variables for motors PWM
    uint8_t analogSpeed;                                 // Converted mapping data from analog triggers
    int8_t left_analog_x;                                // Raw data from left analog stick in X-axis
    uint8_t r2, l2;                                      // Raw data from analog triggers
    uint8_t battery;                                     // Save controller battery level
    uint8_t red = 0, green = 255, blue = 255;            // Default LED in controller. User can change this
    uint8_t distance;                                    // HC-SR04 distance acquired from object
    int32_t duration;                                    // HC-SR04 wave travel duration
    uint8_t stopDistance = 10;                           // Distance at which robot will stop and perform given actions

    typedef struct event_up
    {
        bool cross : 1;
        bool square : 1;
        bool triangle : 1;
        bool circle : 1;

        bool up : 1;
        bool down : 1;
        bool left : 1;
        bool right : 1;

        bool r1 : 1;
        bool r2 : 1;
        bool r3 : 1;
        bool l1 : 1;
        bool l2 : 1;
        bool l3 : 1;

        bool share : 1;
        bool options : 1;
        bool touchpad : 1;
    };
    event_up button;

    Dualshock4Controller(uint8_t motor_pin_1,
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
                         float steeringSensivity);
    ~Dualshock4Controller();

    void init();
    void pins_settings();

    void drive();
    void features();
    void accelerate();
    void decelerate();
    void accelerateAnalog();
    void move();
    void steering();
    void flushVar();
    void flushTimings();
    void distanceSR04();

    void DualShock4_Led_RED();
    void DualShock4_Led_ORANGE();
    void DualShock4_Led_GREEN();
    void DualShock4_Led_YELLOW();
    void DualShock4_Led_PINK();
    void DualShock4_Led_PURPLE();
    void DualShock4_Led_AQUA();
    void DualShock4_Led_WHITE();
    void DualShock4_Led_BLUE();
};

#endif