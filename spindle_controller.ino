// Spindle speed control using servo controlled speed controller
// Based on the servo 'Knob' example in Arduino
// Jes1510@gmail.com


#include <Servo.h> 
 
Servo speedServo;        // create servo object to control a servo 
 
int potPin = 0;      // analog pin used to connect the potentiometer
int modeSwitch = 2;  // Mode switch on pin 2
int pwmInputPin = 3;
int ledPin = 13;     // Indicator LED for mode

unsigned long  pulseHighWidth ;    // How wide High portion of pulse 
int val;              // variable to read the value from the analog pin 


int mode = 0;        // Init mode to manual
int lastVal = val;    // tracker for servo values


// State machine for modes
int lastMode = mode;  // tracker for mode state machine
int man = 0;          // Manual mode
int pwm = 1;          // PWM mode
 
void setup() 
{ 
  speedServo.attach(9);                        // Servo on Pin 9
  pinMode(modeSwitch, INPUT_PULLUP);        // Switch for mode, pullup on
  Serial.begin(9600);                                  
} 
 
void loop() 
{ 
  mode = getMode();

  if (mode==man) {
    val = readPot();
  }
  
  if (mode == pwm) {    
    val = readPWM(); 
    Serial.print("PWM: ");
    Serial.println(val);
    val = map(val, 0, 10000, 179, 0) ;  
  }
  Serial.print("C: ");
  Serial.println(val);
    
  if (val != lastVal) {
    setPosition(val) ;
    lastVal = val;
  }
  
  
} 

void setPosition(int pos) {
  speedServo.write(pos);               // set servo position
  delay(15);                          // waits for the servo to get there   
}

unsigned long readPWM() {
  unsigned long pw = pulseIn(pwmInputPin, HIGH);
  return pw;
}

int readPot() {
  val = analogRead(potPin);            // reads the value of the potentiometer (value between 0 and 1023) 
  val = map(val, 0, 1023, 0, 179);     // scale it to use it with the servo (value between 0 and 180) 
  return val;
}

int getMode() {
  mode = digitalRead(modeSwitch);
  digitalWrite(ledPin, mode);
  if (mode != lastMode) {    
    Serial.println(mode);
    lastMode = mode;
    
  }
  return mode;
}
