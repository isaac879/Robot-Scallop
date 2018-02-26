/*--------------------------------------------------------------------------------------------------------------------------------------------------------
 * 
 * CHECK THE CODE FOR "TODO:" AND EDIT APPROPRIATELY 
 * 
 * The code is developed for a 3D printed robot scallop that is afraid of the dark. The scallop is controlled by an Arduino Uno.
 * 
 * Robot Scallop STL files: Not currently publicly available.
 * 
 * All measurements are in SI units unless otherwise specified
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS 
 * IN THE SOFTWARE
 * 
 * Code written by isaac879
 *
 * * Last modified 26/02/2018
 * 
 *--------------------------------------------------------------------------------------------------------------------------------------------------------*/

#include <Iibrary.h> //A library I created for Arduino that contains some simple functions I commonly use. Library available at: https://github.com/isaac879/Iibrary
#include <Servo.h>

#define SERVO_MIN 0
#define SERVO_MAX 180

#define DARK_THRESHOLD 250
#define SCHMITT_TRIGGER_THRESHOLD 10

#define LDR_PIN A0
#define SPEAKER_MOSFET_PIN 8
#define SERVO_PIN 6

#define OPEN_CLOSE_DELAY 30

int ldr_value = 0;
bool speaker_on = false;
bool is_open = false;
unsigned long previous_millis = 0;
Servo SG90;

void getFear(){
    int ldr_value = analogRead(LDR_PIN);//get the ldr value
    
    if(ldr_value < DARK_THRESHOLD - SCHMITT_TRIGGER_THRESHOLD){
        digitalWrite(SPEAKER_MOSFET_PIN, HIGH);//speaker on
        speaker_on = true;
    }
    else if(ldr_value > DARK_THRESHOLD + SCHMITT_TRIGGER_THRESHOLD){
        digitalWrite(SPEAKER_MOSFET_PIN, LOW);//speaker off
        speaker_on = false;
    }
    printi("LDR Value: ", ldr_value);
    printi("speaker_on Value: ", speaker_on);
    printi("is_open Value: ", is_open);
}

void setup() {
    pinMode(SPEAKER_MOSFET_PIN, OUTPUT);
    digitalWrite(SPEAKER_MOSFET_PIN, LOW);//speaker off
    SG90.attach(SERVO_PIN);
    SG90.write(SERVO_MIN);//Moves the servo to the position in degrees
    Serial.begin(57600);
    
}

void loop() { 
    getFear();
    
    if(is_open && previous_millis + 10000 < millis()){
        previous_millis = millis();
        for(int i = SERVO_MIN; i <= SERVO_MAX; i++){
            getFear();
            SG90.write(i);//Moves the servo to the position in degrees
            delay(OPEN_CLOSE_DELAY);
        }
        is_open = false;
    }
    else if(!is_open){
        for(int i = SERVO_MAX; i >= SERVO_MIN; i--){
            getFear();
            SG90.write(i);//Moves the servo to the position in degrees
            delay(OPEN_CLOSE_DELAY);
        }
        is_open = true;
    } 
}
