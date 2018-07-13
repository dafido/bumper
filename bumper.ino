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
#define HAZ       2       // the number of the HAZARAD SIGNAL pin
#define REV       3       // the number of the REVERSE SIGNAL pin
#define LED1      5       // the number of the LED pin
#define LED2      6       // the number of the LED pin
#define LED3      7       // the number of the LED pin
#define LED4      8       // the number of the LED pin
#define CAM       9       // the number of the CAMERA pin
#define BLUE      10      // the number of the BLUE pin
#define GREEN     11      // the number of the GREEN pin
#define RED       12      // the number of the RED pin
#define CAM_DELAY 3000
#define HAZ_DELAY 350

//////////////////////
//Objects
//////////////////////
Bounce haz = Bounce(); //instatiate two objects to handle
Bounce rev = Bounce(); //button debouncing
elapsedMillis counter1; //These are special variables (longs, I think) that
elapsedMillis counter2; //count up from zero by one each millisecond.
elapsedMillis backupDelay;
elapsedMillis hazDelay;

/////////////////////
//Global Variables
/////////////////////
/*variables have a property called scope, which determines
 * where in a program they can be accessed. Global variable 
 * can be read or accessed by any function.
 * For more: https://www.arduino.cc/reference/en/language/variables/variable-scope--qualifiers/scope/
 */   
bool ledState[4]  = {LOW,LOW,LOW,LOW}; //These track if the "output" is on or off
bool camState   = false;
bool harzards   = false;
bool blueState  = LOW;
bool greenState = LOW;
int redState   = LOW;

void setup() {
   // Setup the button with an internal pull-up :
  pinMode(REV,INPUT_PULLUP);
  pinMode(HAZ,INPUT_PULLUP);
  
  // After setting up the button, setup the Bounce instance :
  rev.attach(REV);  //objectname.method(argument);
  rev.interval(10);     //use the library documentatoin to find out
  haz.attach(HAZ);  //what the methods are and what arguments
  haz.interval(10);     //are required.
  
  // Setup the LED :
  for(int i = 0; i < 4; i++){
  pinMode(LED1+i,OUTPUT);
  digitalWrite(LED1+i,ledState[i]);  
  }
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

void backupCam(int arg){ //code that changes outputs
  static int s = 0;  //static means the value of the local variable can persist bewteen 
                     //calls to the function it is inside.
  if(arg != 0){ //'arg' is a local variable, it only exists in 'backupCam'                   
               //only run if arg is not 0 (enable behaviorSet1)
    switch(s){ 
      //LED1 
      case 0: 
        if(rev.fell()){
          camState = HIGH; //set the STATE high, the pin itself will change in the update funcion
          s++;              //same as s = s+1;
          }
        break;
      case 1:
        if(rev.fell()){ //has the button been pressed again?
          backupDelay = 0;
          s++; //
        }
      case 2:
      if(backupDelay > CAM_DELAY){      //check if one second has elapsed
          camState = !camState;   //Turn off camera
          s=0;
        }
        break;
    } //close switch-case
  } //close if statement
}  //close function
void hazards(int arg){ //The name 'arg' can be repeated in a different function
  static int s = 0; //same as before
  static int i = 0; //counter
 
  if(arg != 0){
    switch(s){             
      case 0:
        if(haz.fell()){
          redState = HIGH; //turn on an indicator
          hazDelay = 0;     //set elapsedMillis timer to 0
          s++;              //same as s = s+1;
          }
        break;
      case 1:
        if(haz.fell()){  //has the button been pressed?
          redState = LOW;  //Turn the LED off
          greenState = HIGH;
          s++;              //Go back to the first case that waits for
        }                   //a button press
        else if(hazDelay > HAZ_DELAY){  //time to blink?
          hazDelay = 0;           //reset timer
          ledState[(i%3)] = !ledState[(i%3)]; //Toggle Leds in sequence   
        }
        break;
       case 2:
          greenState = LOW;  //Turn the LED off
          for(int j=0;j<3;i++){
            ledState[j] = LOW;
          }
          s=0;              //Go back to the first case that waits for a button press
        break;              
    } //close swtich case;
  } //close if statement
  else{
    s = 0; 
  }
}

void writeOutputs(){
  for(int j=0;j<3;j++){
    digitalWrite(LED1+j,ledState[j]);
  }
}

void loop() {
 updateButtons;
 backupCam(1);
 hazards(1); //blink at decreasing rate if holding button 2 AND LED1 is blinking
 writeOutputs;
}
