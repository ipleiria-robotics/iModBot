// In this example we'll cover all the autoDrive() function usage.
// The board used for this tutorial was ESP32 Devkit V1

// let's define the pins to wich the sensors are connected
#define farLeft 25
#define left 33
#define middle 32
#define right 35
#define farRight 34
#define CLP 39
#define near 36

#include <robotOnLine.h>

robotOnLine robot; // Create a "robot" object.

void setup()
{
  Serial.begin(115200); // This is the serial speed for ESP boards.

  robot.begin();  // Configure the pins
  
  robot.beginAutoDrive(); // Initialize the auto driving function
  
  Serial.println("-> Pins set!");

  byte dist = 10; // Distance (in cm) at wich the robot stops.
  robot.setStopDistance(dist);
  Serial.print("-> Stop distance set to ");
  Serial.println(dist);
}

void loop()
{
  int info = 0; // variable to store the find information returned by the autoDrive() function
  int command = 0; // variable to remenber which command should be executed



  info = robot.autoDrive(0); //here we copy the returned value to the "info" variable

  /* By default you have to send something to the function autoDrive( ), these are accepted numbers:
      0 -> Send 0 or any other number not listed if you don't want to send an instruction.
      1 -> Send 1 to rotate the robot to the right until it finds a line.
      2 -> Send 2 to rotate the robot to the left until it finds a line.
      3 -> Send 3 to tell the robot to go forward.
      4 -> Send 4 to tell the robot to reverse, be aware the robot may slightly turn due to the rear wheel.
  */



  if (info == 1) //the autoDrive() function returns 1 if multiple lines are found
  {
    switch ( command )
    {
      case 0: // if the variable command is 0 we execute the first order
        //code for the 1st time multiple lines are found
        robot.autoDrive(1); //turn Right
        command++;  // in the end we increment the variable, this is equivalent to "command = command +1;"
        break;

      case 1:
        //code for the 2nd time multiple lines are found
        robot.autoDrive(2); //turn left
        command++;
        break;

      case 2:
        //code for the 3rd time multiple lines are found
        robot.autoDrive(3); //go ahead
        command++;
        break;

      case 3:
        //code for the 4th time multiple lines are found
        //okay, this time let's say the robot had two lines found, one forward and one to the side
        //let's check to which side the line is and tell the robot that way
        if ( digitalRead(left) || digitalRead(farLeft) )
        {
          robot.autoDrive(2); //turn left
        }
        else // if( digitalRead(right) || digitalRead(farRight) )
        { // we could check if there is a line to the right but if no line was found in the left side, and there
          // are multiple lines found, then there can only be another line to the right.
          robot.autoDrive(1); //turn right
        }
        command++;
        break;

      case 4:
        //code for the 5th time multiple lines are found

        command++;  //set this variable to 0 if you want the previous commands to be repeated
        break;

      default:
        break;
    }
  }

  if (info == 2) //the autoDrive() function returns 2 if an obstacle is found
  {
    Serial.println("Something is on the way!");
  }

  if (info == 3) //the autoDrive() function returns 2 if no lines are found at the moment
  {
    Serial.println("No lines found, is the robot off track? Perhaps it's turning right now.");
  }
}
