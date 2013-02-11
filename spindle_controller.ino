/* Written by Jesse Merritt www.github.com/jes1510 February 5, 2013

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License 
as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <http://www.gnu.org/licenses/


Spindle speed control using servo controlled speed controller
Based partially on the servo 'Knob' example in Arduino.
Requires Arduino IDE v 1.0.3 or greater

/**/

#include <Servo.h> 
 
Servo speedServo;        // create servo object to control a servo 
 
int potPin = 0;          // analog pin used to connect the potentiometer
int modeSwitch = 2;      // Mode switch on pin 2
int pwmInputPin = 3;     // Pin connected to parallel port on the computer
int ledPin = 13;         // Indicator LED for mode indicator

unsigned long  pulseHighWidth ;     // Measure width of incoming pulse. Default PWM is 100Hz
int val;                            // variable to hold the value from the analog pin 


int mode = 0;        // Init mode to manual
int lastVal = val;    // tracker for servo values


// State machine for modes
int lastMode = mode;  // tracker for mode state machine
int man = 0;          // Manual mode
int pwm = 1;          // PWM mode
 
void setup() 
{ 
  speedServo.attach(9);                      // Servo on Pin 9
  pinMode(modeSwitch, INPUT_PULLUP);         // Switch for mode, pullup on
  Serial.begin(9600);                        // Serial port init at 9600 baud                                
} 
 
void loop() 
{ 
  mode = getMode();            // Read the mode 

  if (mode==man) {            // If the mode is manual then read the pot
    val = readPot();          // Read pot and put it in val
  }
  
  if (mode == pwm) {          // If the mode is pwm then read the pulse from the computer
    val = readPWM();          //  read the pulse width and put it in val
    //Serial.print("PWM: ");     // Simple serial debugging to read pulse width
    //Serial.println(val);
    val = map(val, 0, 10000, 179, 0) ;  // The pulse width can be up to 10000uS.  Map it to degrees for servo
  }
  
    
  if (val != lastVal) {       // If the value has changed then update the servo
    Serial.print("Servo Value: ");        // Send remapped value out the serial port
    Serial.println(val);        // Send the value
    setPosition(val) ;        // Set the servo position    
    lastVal = val;            // reset to last state
  } 
} 


  

void setPosition(int pos) {            // Set the servo position
  speedServo.write(pos);               // Write the position to the servo
  delay(15);                           // waits for the servo to get there   
}

unsigned long readPWM() {              // Read pulse from the computer
  unsigned long pw = pulseIn(pwmInputPin, HIGH);  // Read pulse width 
  return pw;                            // return the pulse width
}

int readPot() {                        // Read value of manual speed knob
  val = analogRead(potPin);            // reads the value of the potentiometer (value between 0 and 1023) 
  val = map(val, 0, 1023, 0, 179);     // scale it to use it with the servo (value between 0 and 180) 
  return val;
}

int getMode() {                        // Read mode switch.  Either Manual or PWM
  mode = digitalRead(modeSwitch);      // Read the value of the switch
  digitalWrite(ledPin, mode);          // Set the debugging LED to the mode state
  if (mode != lastMode) {              // If the mode changes then send it out serial port
    Serial.println(mode);              // Send mode value to computer
    lastMode = mode;                   // Reset lastmode to current mode
    
  }
  return mode;                         // Return the mode
}
