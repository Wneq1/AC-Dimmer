#include <TimerOne.h>                    // library for phase control

#include <Wire.h>                         // display initialization
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);
#if (SSD1306_LCDHEIGHT != 64)
//#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif/

int LED = 12;

int button_add = 9;                   // button to increase power

int button_subtract = 7;                 // button to decrease power

int button_save = 8;                   // save button
int button_save_state = HIGH;
int last_button_save_state = HIGH;
int save = 0;

int Triac = 10;                          // pin to control the triac

int inc = 1;                                // test variable

volatile int i = 0;                         // counts time since zero crossing CHYBA?!?
volatile boolean zero_crossing = 0;            // variable for zero crossing detection
int value = 128;                            // variable for changing power value 128 - Off (0%), 0 - On (100%)
int frequency_step = 75;                        // For 60 Hz should be changed to 65, determines time before activation

/* It is calculated based on the frequency of the supply voltage and the brightness level needed
 *
 * The program is based on two zero crossings during one cycle, which means for 120Hz and 60Hz or for 100Hz and 50Hz
 *
 * To determine the frequency step, divide the length of a full half-cycle of the signal in microseconds by the number of brightness levels per step
 *
 * (120Hz =  8,333uS) -> 128 steps gives 65uS
 * (100Hz = 10,000uS) -> 128 steps gives 75uS
 */

void setup() {

  pinMode(Triac, OUTPUT);                // set triac as output

  pinMode(button_add, INPUT_PULLUP);  // connect buttons with pull-up resistors
  pinMode(button_subtract, INPUT_PULLUP);
  pinMode(button_save, INPUT_PULLUP);


  attachInterrupt(0, detect_zero_crossing, RISING);  // function responsible for detecting zero crossing connected to pin D2 -> (0)
  Timer1.initialize(frequency_step);                    // initialize timer for the required frequency
  Timer1.attachInterrupt(check_value, frequency_step);    // function checks if the triac should be turned on at the right moment and works in milliseconds

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize display via I2C with address 0x3D and resolution of 128x64
  display.display();
  display.clearDisplay();

}

void detect_zero_crossing() {   // function for controlling the triac when a zero crossing is detected
  zero_crossing = true;
  i = 0;
  digitalWrite(Triac, LOW);
}

void check_value() {            // function responsible for checking zero crossing
  if (zero_crossing == true) {
    if (i >= value) {
      digitalWrite(Triac, HIGH);
      i = 0;
      zero_crossing = false;
    }
    else {
      i++;
    }
  }
}

int Button_Add(int button_add, int button_save){ // Button to increase the value of the variable "value" representing power
  if(save==1){
    if (digitalRead(button_add) == LOW) { 
      if (value < 128) { 
        value = value + 1;
        delay(10);
      }
    }
    return(value);
  }
}

int Button_Subtract(int button_subtract, int button_save){ // Button to decrease the value of the variable "value" representing power
  if(save==1){
    if (digitalRead(button_subtract) == LOW) { 
      if (value > 0) { 
        value=value - 1; 
        delay(10);
      }
    }
    return(value);
  }
}

int Button_Save(int button_save){ // Button that enables changing the power only after pressing it
  button_save_state = digitalRead(button_save);
  if(button_save_state == LOW && last_button_save_state == HIGH){
    save = 1 - save;
  }
  last_button_save_state = button_save_state;
  delay(10);
  return (save);
} 

void test(){ // Sequentially turns on and off a load
  value += increment;
  if((value >= 128) || (value <= 0))
    increment *= -1;
  delay(20);
}

void loop() { 
  
  Button_Save(button_save); // Initialize functions to control buttons
  Button_Add(button_add, button_save);
  Button_Subtract(button_subtract, button_save);
 
  display.clearDisplay(); // Initialize the display
  display.setTextSize(4);
  display.setTextColor(WHITE);
  display.setCursor(24,0);
  display.print(map(value, 128, 0, 0, 100)); 
  display.print("%");
  display.display();
  
  if (save == 1){
    digitalWrite(LED, HIGH);
  }
  else
    digitalWrite(LED, LOW);
}


