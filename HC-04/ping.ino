#include<Arduino.h>
#include<HardwareSerial.h>
#define SERIAL_DEBUG false
#include<SerialDebug.h>
#include<Fsm.h>
#define TRIG 20
#define ECHO 21
#define LED 13
#define DANGEROUS 1
#define SAVE 0
#define RANGE 4 // v cm
#define INTERVAL 100
#define TIME(last,interval) (millis() - (last)) > (interval) // makro na zjisteni doby behu
double far(void);
void up(void);
void down(void);
State OK(NULL,&far,NULL);
State CRASH(NULL,&far,NULL);
Fsm fsm(&OK);
void setup() {
    pinMode(TRIG,OUTPUT);
    pinMode(ECHO,INPUT);
    pinMode(LED,OUTPUT);
    SERIAL_DEBUG_SETUP(9600);
    Serial.begin(9600);
    fsm.add_transition(&OK,&CRASH,DANGEROUS,&up);
    fsm.add_transition(&CRASH,&OK,SAVE,&down);
}

void loop() {
/*    Serial.print(far());
    Serial.println(" cm");
   //far();
   delay(100);
*/
fsm.run_machine();
//delay(50);
}

double far() {
   // DEBUG(__FUNCTION__);
    static unsigned long last;
    if (TIME(last,INTERVAL)) {
    last = millis();
    DEBUG(last,millis());
    digitalWrite(TRIG,LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG,HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG,LOW);
    double ping = pulseIn(ECHO,HIGH);
    //DEBUG(ping);
    ping = ping * 0.017351l - 1.3l;
    //Serial.println(ping);
    //DEBUG(ping);
    if (ping > RANGE) fsm.trigger(SAVE);
    else if (ping < RANGE) fsm.trigger(DANGEROUS);
    return ping;
    }
}
 void up() {
     //digitalWrite(LED,HIGH);
     tone(LED,442);
     DEBUG("ON");
 }

 void down() {
     //digitalWrite(LED,LOW);
     noTone(LED);
     DEBUG("OFF");
 }