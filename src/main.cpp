#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C display(0x27,16,2);

#define HIGH_BUTTON 13
#define LOW_BUTTON 10
#define CHANGE_FINAL 1

#define LAMBDA_CHAR -1
#define ZERO_CHAR 0
#define ONE_CHAR 1

#define OUTPUT_LENGTH 10
int OUTPUTS[OUTPUT_LENGTH] = {3,2,5,4,6,7,9,8,11,12};

#define INPUT_LENGTH 5
int INPUTS[INPUT_LENGTH] = {A0, A1, A2, A3, 0};

int startState = 1;
int finalState = 1;

int state = 1;

String _word = "";

void setup() {
  display.init();
  display.backlight();
  display.clear(); 

  pinMode(HIGH_BUTTON, INPUT);
  pinMode(LOW_BUTTON, INPUT);

  for(int i = 0; i < OUTPUT_LENGTH; i++) {
    pinMode(OUTPUTS[i], OUTPUT);
  }

  for(int i = 0; i < INPUT_LENGTH; i++) {
    pinMode(INPUTS[i], INPUT);
  }
}

int getPressedButton() {
  return digitalRead(HIGH_BUTTON) == HIGH ? 1 : (digitalRead(LOW_BUTTON) == HIGH ? 0 : -1);
}

int consumeWord(int signal) {
  int port = OUTPUTS[(state - 1) * 2 + (signal == ZERO_CHAR ? 1 : 0)];
  digitalWrite(port, HIGH);
  return port;
}

void handleReceiveSignal() {
  for (int i = 0; i < INPUT_LENGTH; i++) {
    int signal = digitalRead(INPUTS[i]);
    if(signal > 0) {
      state = i + 1;
    }
  }
}

void loop() {

  int changeFinal = digitalRead(CHANGE_FINAL);
  if(changeFinal == HIGH) {
    finalState = finalState < 5 ? finalState + 1 : 1;
  }

  int character = getPressedButton();

  if(character != LAMBDA_CHAR) {
    int port = consumeWord(character);
    _word += character;
    handleReceiveSignal();
    digitalWrite(port, LOW);
  }

  display.setCursor(0,0);
  display.print("E: q");
  display.print(state);

  display.setCursor(6,0);
  display.print("F: q");
  display.print(finalState);

  display.setCursor(12,0);
  display.print("A: ");
  display.print(state == finalState ? "S" : "N");

  display.setCursor(0,1);
  display.print(_word);

  delay(100);
}
