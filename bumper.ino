/*This is an example program to demonstrate a responsive coding pattern.
 * The idea here is to read a number of inputs, buttons in this case, and 
 * change the behavior of some outputs, LEDs this time, depending on the
 * states of those inputs and outputs.   
 */

////////////////////
//Included Libraries
////////////////////
/*Libraries are files with reusable code that
 * solves a common problem. They contain a class
 * (or multiple classes) which define objects.
 * Objects have data and methods to use that data.
*/
#include <Bounce2.h> //https://github.com/thomasfredericks/Bounce2/wiki
#include <elapsedMillis.h> //https://www.pjrc.com/teensy/td_timing_elaspedMillis.html
//////////////////////
//Compiler Definitions
//////////////////////
#define HAZ      2;       // the number of the HAZARAD SIGNAL pin
#define REV      3;       // the number of the REVERSE SIGNAL pin
#define LED1     5;       // the number of the LED pin
#define LED2     6;       // the number of the LED pin
#define LED3     7;       // the number of the LED pin
#define LED4     8;       // the number of the LED pin
#define CAM      9;       // the number of the CAMERA pin
#define BLUE     10;      // the number of the BLUE pin
#define GREEN    11;      // the number of the GREEN pin
#define RED      12;      // the number of the RED pin

//////////////////////
//Objects
//////////////////////
Bounce haz = Bounce(); //instatiate two objects to handle
Bounce rev = Bounce(); //button debouncing
elapsedMillis counter1; //These are special variables (longs, I think) that
elapsedMillis counter2; //count up from zero by one each millisecond.

/////////////////////
//Global Variables
/////////////////////
/*variables have a property called scope, which determines
 * where in a program they can be accessed. Global variable 
 * can be read or accessed by any function.
 * For more: https://www.arduino.cc/reference/en/language/variables/variable-scope--qualifiers/scope/
 */
int led1State  = LOW;    //These track if the "output" is on or off
int led2State  = LOW;
int led3State  = LOW;
int led4State  = LOW;
int camState   = LOW;
int blueState  = LOW;
int greenState = LOW;
int redState   = LOW;
int state1 = 1;         //These globals will be used as a switch to 
int state2 = 0;         //control blocks of code.

void setup() {
   // Setup the button with an internal pull-up :
  pinMode(REV,INPUT_PULLUP);
  pinMode(HAZ,INPUT_PULLUP);
  
  // After setting up the button, setup the Bounce instance :
  button1.attach(REV);  //objectname.method(argument);
  button1.interval(50);     //use the library documentatoin to find out
  button2.attach(HAZ);  //what the methods are and what arguments
  button2.interval(50);     //are required.
  
  // Setup the LED :
  pinMode(LED1,OUTPUT);
  digitalWrite(LED1,led1State);
  pinMode(LED2,OUTPUT);
  digitalWrite(LED2,led2State);
  pinMode(LED3,OUTPUT);
  digitalWrite(LED3,led3State);
  pinMode(LED4,OUTPUT);
  digitalWrite(LED4,led4State);
  pinMode(CAM,OUTPUT);
  digitalWrite(CAM,camState);
  pinMode(BLUE,OUTPUT);
  digitalWrite(BLUE,blueState);
  pinMode(GREEN,OUTPUT);
  digitalWrite(GREEN,greenState);
  pinMode(RED,OUTPUT);
  digitalWrite(RED,redState);
}

void updateButtons(){ //this function checks the inputs for changes
                      //because this function does not return any
                      //data it is defined with the keyword 'void'.
  rev.update();   //the .update method reads the button and looks for 
  haz.update();   //a significant change in state. Must call once per loop
}

void behaviorSet1(int arg){ //code that changes outputs
  static int s = 0;  //static means the value of the local variable can persist bewteen 
                     //calls to the function it is inside.
  if(arg != 0){ //'arg' is a local variable, it only exists in 'behaviorSet1'                   
               //only run if arg is not 0 (enable behaviorSet1)
    switch(s){ 
      //LED1 
      case 0: 
        if(button1.fell()){
          counter1 = 0;     //set elapsedMillis timer to 0
          state2 = 1;       //use global variable to enable behaviorSet2
          led1State = HIGH; //set the STATE high, the pin itself will change in the update funcion
          s++;              //same as s = s+1;
          }
        break;
      case 1:
        if(rev.fell()){ //has the button been pressed again?
          s++; //
        }
        if(counter1 > 1000){      //check if one second has elapsed
          counter1 = 0;           //reset timer
          led1State = !led1State;   //Toggle Led  
        }
        break; //if neither 'if' is true do nothing. 
      case 2:
      if(rev.fell()){    //has the button been pressed again?
          led1State = LOW;  //Turn the LED off
          state2 = 0;       //Disable behaivorSet2
          s=0;              //Go back to the first case that waits for
        }                   //a button press
        if(counter1 > 500){      //check if half a second has elapsed
          counter1 = 0;           //reset timer
          led1State = !led1State;   //Toggle Led (faster this time)
        }
        break;
    } //close switch-case
  } //close if statement
}  //close function

void behaviorSet2(int arg){ //The name 'arg' can be repeated in a different function
  static int s = 0; //same as before
  static int rate = 1; //this time we will vary the blink rate
  
  if(arg != 0){
    switch(s){             
      case 0:
        if(haz.fell()){
          counter2 = 0;     //set elapsedMillis timer to 0
          led2State = HIGH; //set the STATE high, the pin itself will change in the update funcion
          s++;              //same as s = s+1;
          }
        break;
      case 1:
        if(haz.rose()){  //has the button been released?
          led2State = LOW;  //Turn the LED off
          rate = 1;         //reset rate
          s=0;              //Go back to the first case that waits for
        }                   //a button press
        else if(counter2 > rate){  //time to blink?
          counter2 = 0;           //reset timer
          rate++;
          led2State = !led2State;   //Toggle Led (slower and slower)
        }
        break;
    } //close swtich case;
  } //close if statement
  else{
    s = 0; 
    rate = 1;  
  }
}

void writeOutputs(){
  digitalWrite(LED1,led1State);
  digitalWrite(LED2,led2State);
}

void loop() {
 updateButtons;
 behaviorSet1(state1); //blink after toggling
 behaviorSet2(state2); //blink at decreasing rate if holding button 2 AND LED1 is blinking
 writeOutputs;
}
