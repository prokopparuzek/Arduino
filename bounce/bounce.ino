#define SERIAL_DEBUG false
#include <SerialDebug.h>
#include<Arduino.h>
#include<HardwareSerial.h>
#include<Bounce2.h>
#define BUT1 2
#define BUT2 3
#define GLED 7
#define RLED 8
#define INTERVAL_LENGHT(A, B) (millis() - (A)) >= (B)  

Bounce debouncer1 = Bounce();
Bounce debouncer2 = Bounce();
void setup() {
    pinMode(BUT1,INPUT_PULLUP);
    pinMode(BUT2,INPUT_PULLUP);
    debouncer1.attach(BUT1);
    debouncer1.interval(8);
    debouncer2.attach(BUT2);
    debouncer2.interval(5);
    pinMode(GLED,OUTPUT);
    pinMode(RLED,OUTPUT);
    SERIAL_DEBUG_SETUP(57600);
}

void loop() {
   manageLED();
   blik();
}

void manageLED() {
    debouncer1.update();
    if(debouncer1.fell())
        digitalWrite(GLED,!digitalRead(GLED));
    else if(debouncer1.rose())
        digitalWrite(RLED,!digitalRead(RLED));
}

int blik() {
    static short stat;
    static unsigned long lastTime;
    debouncer2.update();
    DEBUG("start blik", debouncer2.read());
    if (!debouncer2.read()) {
        DEBUG(millis(),stat,lastTime);
        if (!stat) {
            stat= 1;
            lastTime = millis();
        }
        else if (INTERVAL_LENGHT(lastTime, 2000)) {
            digitalWrite(RLED,!digitalRead(RLED));
            digitalWrite(GLED,!digitalRead(GLED));
            stat = 0;
        }
    }
    else {
        stat = 0;
    }
}