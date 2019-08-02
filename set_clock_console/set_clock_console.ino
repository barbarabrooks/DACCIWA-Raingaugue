#include <Bridge.h>
#include <Console.h>
#include <Wire.h>
#include "RTClib.h"

RTC_DS1307 RTC;

void setup () {
    Bridge.begin();	// Initialize the Bridge
    Console.begin();
    Wire.begin();
    RTC.begin();
    RTC.adjust(DateTime(__DATE__, __TIME__)); 
}

void loop () {
    DateTime now = RTC.now();
    
    Console.print(now.year(), DEC);
    Console.print('/');
    Console.print(now.month(), DEC);
    Console.print('/');
    Console.print(now.day(), DEC);
    Console.print(' ');
    Console.print(now.hour(), DEC);
    Console.print(':');
    Console.print(now.minute(), DEC);
    Console.print(':');
    Console.print(now.second(), DEC);
    Console.println();
    
    delay(1000);
}
