#include <TimerOne.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

int LED = 12;           // signalization LED
int previous_save_value = 0;
int set_value = 0;
int x = 128;

int add_button = 9;
int subtract_button = 7;
int save_button = 8;
int save_button_state = HIGH;
int last_save_button_state = HIGH;
bool save = false;

int Triac = 10; //AC Load

int inc = 1;

volatile int i = 0;
volatile boolean zero_crossing = false; //zero crosing value
int value = 128;           //value for operating triac
int frequency_step = 75;   // 50Hz -> 75 || 60Hz -> 65

/* It is calculated based on the frequency of the power supply and the desired brightness level.
 *
 * The program relies on two zero-crossing points during one cycle, which is either for 120Hz and 60Hz or for 100Hz and 50Hz.
 *
 * To determine the frequency step, divide the duration of a full signal half-cycle in microseconds by the number of brightness levels per step.
 *
 * (120Hz = 8,333μs) -> 128 steps yield 65μs
 * (100Hz = 10,000μs) -> 128 steps yield 75μs
 */



void setup() {
  Serial.begin(9600);
  pinMode(Triac, OUTPUT);
  pinMode(LED, OUTPUT);
  pinMode(add_button, INPUT_PULLUP);
  pinMode(subtract_button, INPUT_PULLUP);
  pinMode(save_button, INPUT_PULLUP);

  attachInterrupt(0, detect_zero_crossing, RISING);   // zero crossing detector on pin numb. 2
  Timer1.initialize(frequency_step);
  Timer1.attachInterrupt(check_value, frequency_step);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display();
  display.clearDisplay();
}

void detect_zero_crossing() {
  zero_crossing = true;
  i = 0;
  digitalWrite(Triac, LOW);
}

void check_value() {
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

int AddButton(int add_button) {
  if (digitalRead(add_button) == LOW) {
    if (x < 128) {
      x = x + 1;
      delay(10);
    }
  }
  return x;
}

int SubtractButton(int subtract_button) {
  if (digitalRead(subtract_button) == LOW) {
    if (value > 0) {
      x = x - 1;
      delay(10);
    }
  }
  return x;
}

bool SaveButton(int save_button) {
  save_button_state = digitalRead(save_button);
  if (save_button_state == LOW && last_save_button_state == HIGH) {
    save = !save;
  }
  last_save_button_state = save_button_state;
  delay(10);
  return save;
}

void test() {
  value += inc;
  if ((value >= 128) || (value <= 0))
    inc *= -1;
  delay(20);
}

void loop() {
  int save_button_state = digitalRead(save_button);

  if (save_button_state == LOW && last_save_button_state == HIGH) {
    save = !save;
    if (save == 0) {
      value = x;
    }
  }

  last_save_button_state = save_button_state;

  if (save == 1) {
    digitalWrite(LED, HIGH);
    
    x = AddButton(add_button);
    x = SubtractButton(subtract_button);

    display.clearDisplay();
    display.setTextSize(3);
    display.setTextColor(WHITE);
    display.setCursor(40, 0);
    display.print(map(x, 128, 0, 0, 100));
    display.print("%");

    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(20, 25);
    display.print("Phase regulation");
    display.display();
  } else if (save == 0 && set_value == 0) {
    for (int i = 0; i < x; i++) {
      value++;
    }
    set_value = 1;

    display.clearDisplay();
    display.setTextSize(3);
    display.setTextColor(WHITE);
    display.setCursor(40, 0);
    display.print(map(x, 128, 0, 0, 100));
    display.print("%");

    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(20, 25);
    display.print("Phase regulation");
    display.display();
  }

  if (save == 0) {
    digitalWrite(LED, HIGH);
    delay(500);
    digitalWrite(LED, LOW);
    delay(500);
  }

  Serial.println("X " + String(x) + " Value " + value);
}
