/*
 * In this example we'll implement the robotOnLine library self driving function to the blink library
 */

#include <robotOnLine.h>

//here we create a "robot" object
robotOnLine robot; 
/*
 * You can replace "robot" with some other word if you want, then each
 * time you call a function from the library you will have to write:
 * yourWord.functionName(); 
 * instead of :
 * robot.functionName();
 */

unsigned long enlapsedTime=0;
 
void setup() 
{
  //this function configures the IO needed for the project
  robot.begin();
  robot.beginAutoDrive(); 
  //check the gitHub tutorial for more information
  //

  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() 
{
  //here we call the function that makes the robot follow the line automatically
  int info = robot.autoDrive(0);
  /* this function will return a value, 0 if there is nothing to report,
   *  1 if the robot found multiple lines, 2 if the robot found an obstacle
   *  3 if no line is found.
   *  
   *  The sooner this function is called again the better
   */

   /* We have to call the autoDrive function as soon as possible so we 
    * had to remove the delay functions, fear not, you can still delay
    * the blinking of the LED using the millis() function.
    * more information about the millis() function [https://www.arduino.cc/reference/en/language/functions/time/millis/]
    */

  
  if( millis() > enlapsedTime+2000 )  // the first time this line is run this condition will be true
  {
    digitalWrite(LED_BUILTIN, HIGH);  //turn ON the LED
    enlapsedTime=millis();            //here we copy the present time
  }
  else if( millis() > enlapsedTime+1000 ) //after a second (1000 ms) the LED will turn OFF, so it stayed ON during a second.
  {
    digitalWrite(LED_BUILTIN, LOW);   
  }
  // When another second passes the first IF condition will be true and the LED will be turned ON.
  // Once again, it stayed OFF during a second. This cycle will keep going.
}
