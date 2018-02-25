#include<Arduino.h>
#include<HardwareSerial.h>
#define BUT 2
#define LED 3
void setup()
{
    pinMode(BUT,INPUT_PULLUP);
    pinMode(LED,OUTPUT);
    //Serial.begin(9600);
}

void loop()
{
    if (digitalRead(BUT))
    {
        digitalWrite(LED,LOW);
        //Serial.println("LOW");
    }
    else
    {
        digitalWrite(LED,HIGH);
        //Serial.println("HIGH");
    }
}