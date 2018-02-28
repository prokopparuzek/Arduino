#include<Arduino.h>
#include<HardwareSerial.h>
#include<Fsm.h>
#include<Bounce2.h>
#define GLED 11
#define RLED 12
#define BUT1 3
#define BUT2 4
#define BUT2T 2
Bounce debouncer1 = Bounce();
Bounce debouncer2 = Bounce();

void houkacka()
{
    static unsigned long lastTime;
    static short stat;
    debouncer2.update();
    if (!debouncer2.read())
        fsm.trigger(BUT2T);
    
}