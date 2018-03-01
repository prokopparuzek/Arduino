#include<Arduino.h>
#include<HardwareSerial.h>
#include "Fsm.h"

// Used pins
#define LED_PIN LED_BUILTIN

// State machine variables
#define FLIP_LIGHT_SWITCH 1

State state_light_on(on_light_on_enter, NULL, &on_light_on_exit);
State state_light_off(on_light_off_enter, NULL, &on_light_off_exit);
Fsm fsm(&state_light_off);

// Transition callback functions
void on_light_on_enter() {
  Serial.println(__FUNCTION__);
  digitalWrite(LED_PIN, HIGH);
}

void on_light_on_exit() {
  Serial.println(__FUNCTION__);
  digitalWrite(LED_PIN, LOW);
}

void on_light_off_enter() {
  Serial.println(__FUNCTION__);
}

void on_light_off_exit() {
  Serial.println(__FUNCTION__);
}

void on_trans_light_on_light_off() {
  Serial.println(__FUNCTION__);
  Serial.println("  Transitioning from LIGHT_ON to LIGHT_OFF");
}

void on_trans_light_off_light_on() {
  Serial.println(__FUNCTION__);
  Serial.println("  Transitioning from LIGHT_OFF to LIGHT_ON");
}

// standard arduino functions
void setup() {
  Serial.begin(9600);
  Serial.println("FSM Light Switch");
  pinMode(LED_PIN, OUTPUT);

  fsm.add_transition(&state_light_on, &state_light_off,
                     FLIP_LIGHT_SWITCH,
                     &on_trans_light_on_light_off);
  fsm.add_transition(&state_light_off, &state_light_on,
                     FLIP_LIGHT_SWITCH,
                     &on_trans_light_off_light_on);
  fsm.run_machine();
}

void loop() {
  Serial.println("-- loop --");
  delay(2000);
  fsm.trigger(FLIP_LIGHT_SWITCH);
}