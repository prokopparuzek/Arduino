#include<Arduino.h>
#include<HardwareSerial.h>
#include<Fsm.h>
#include<Bounce2.h>
#define SERIAL_DEBUG false
#include<SerialDebug.h>
#define GLED 11
#define RLED 12
#define BUT1 3
#define BUT2 4
#define HOUK 2 // trigger pro prepnuti do fce houkacka
#define BLIK 1 // trigger pro prepnuti do fce blik
#define TIME(last,interval) (millis() - (last)) >= (interval) // makro na zjisteni doby behu
#define nastav(stav) digitalWrite(GLED,stav); digitalWrite(RLED,stav)
void houkacka(void);
void blik(void);
int butTest(struct change *change);
void prepnuti();

Bounce debouncer1 = Bounce();
Bounce debouncer2 = Bounce();

State state_blik(NULL,&blik,&prepnuti);
State state_houk(NULL,&houkacka,&prepnuti);
Fsm fsm(&state_blik); 

// spojeni vsech udaju na zjisteni doby behu
struct change {
    unsigned long lastTime;
    short stat;
    int interval;
    int trigger;
};

// pokud je aktivni blikani nefunguje pouze po prepnuti
void houkacka()
{
    static struct change toBlik;
    toBlik.interval = 2000;
    toBlik.trigger = BLIK;
    DEBUG(__FUNCTION__);
    digitalWrite(GLED,HIGH);
    digitalWrite(RLED,HIGH);
    butTest(&toBlik);
}

// blikani  
void blik()
{
    static struct change toHouk;
    toHouk.interval = 2000;
    toHouk.trigger = HOUK;
    DEBUG(__FUNCTION__);
    debouncer1.update();
    if (debouncer1.fell()) // zmacknuti zelena
        digitalWrite(GLED,!digitalRead(GLED));
    else if (debouncer1.rose()) // pustenu cervena
        digitalWrite(RLED,!digitalRead(RLED));
    butTest(&toHouk);
}

// testuje zda je tlacitko zmacknute dostatecne dlouhou dobu
int butTest(struct change *change) {
short stat;
debouncer2.update();
if (stat = (!debouncer2.read())) {
    if (!change->stat) {
        change->stat = 1;
        change->lastTime = millis();
    }
    else if (TIME(change->lastTime,change->interval)) {
        change->stat = 0;
        fsm.trigger(change->trigger);
    }
}
else {
    change->stat = 0;
}
return stat;
}
 void prepnuti() {
nastav(LOW);
delay(50);
nastav(HIGH);
delay(50);
nastav(LOW);
delay(50);
 }

void setup()
{
    pinMode(BUT1,INPUT_PULLUP);
    pinMode(BUT2,INPUT_PULLUP);
    pinMode(GLED,OUTPUT);
    pinMode(RLED,OUTPUT);
    debouncer1.attach(BUT1);
    debouncer1.interval(10);
    debouncer2.attach(BUT2);
    debouncer2.interval(10);
    fsm.add_transition(&state_blik,&state_houk,HOUK,NULL);
    fsm.add_transition(&state_houk,&state_blik,BLIK,NULL);
    SERIAL_DEBUG_SETUP(57600);
    //Serial.begin(9600);
}

void loop()
{
    fsm.run_machine();
}