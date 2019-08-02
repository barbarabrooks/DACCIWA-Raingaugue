  #include <Bridge.h>
  #include <Console.h>
  #include <Wire.h>
  #include <Process.h>
  #include <FileIO.h>
  #include "RTClib.h"
  
  RTC_DS1307 RTC; //create occurance of clock
  String hostname="RAIN_1";
  volatile int pulseCount; // hardware interupt counts
  byte sensorInterrupt = 4; // 3 (interrupt 0), 2 (interrupt 1), 0 (interrupt 2), 1 (interrupt 3) and 7 (interrupt 4).
  int currentday, currentminute;
  byte dt[6]; // output date time array
  //float volts;
  int drops, battery, accu_rain, rain_rate;
  DateTime now;
  File DataFile;
  char fn[70];
  
  void make_file() {
    String xx;
    String yy =  String(now.year()-2000, DEC); 
    String mm =  String(now.month(), DEC); 
    String dd =  String(now.day(), DEC);
    String hh =  String(now.hour(), DEC);  
    String MM =  String(now.minute(), DEC); 
    String ss =  String(now.second(), DEC); 
    
    if (yy.length()!=2) yy="0"+yy;
    if (mm.length()!=2) mm="0"+mm;
    if (dd.length()!=2) dd="0"+dd;
    if (hh.length()!=2) hh="0"+hh;
    if (MM.length()!=2) MM="0"+MM;
    if (ss.length()!=2) ss="0"+ss;
                    
    xx="/mnt/sda1/" + hostname + "_" + yy + mm + dd + hh + MM + ss + ".csv";
    xx.toCharArray(fn,50);
    File DataFile = FileSystem.open(fn, FILE_WRITE);
    DataFile.close();
  }
  
  void setup() {
    Bridge.begin();	// Initialize the Bridge
    Console.begin();
    FileSystem.begin(); 
    //Serial.begin(57600);	// Initialize the Serial
    Wire.begin();
    RTC.begin();
    
    Process time;
    
    attachInterrupt(sensorInterrupt, pulseCounter, RISING);
    delay(100);
    
    now = RTC.now();

    //set linux clock
    time.begin("date -s");
    time.addParameter("+%Y.%m.%d-%H:%M:%S");
    time.run();
    time.write('2010.03.09-12:30:56');//2008
    %time.begin("date");time.addParameter("+%m");time.run();time.write(now.month());//ll
    %time.begin("date");time.addParameter("+%d");time.run();time.write(now.day());//30
    %time.begin("date");time.addParameter("+%H");time.run();time.write(now.hour());//13
    %time.begin("date");time.addParameter("+%M");time.run();time.write(now.minute());//50
    %time.begin("date");time.addParameter("+%S");time.run();time.write(now.second());//46
    
    //make file
    make_file();
    currentday=now.day();
    
    //wait for start of whole minute
    do {
      now = RTC.now();
      delay(500);
    } while (now.second()!=0);
    currentminute=now.minute();
    pulseCount=0;
  }
  
  void pulseCounter() {
    pulseCount++; // Increment the pulse counter
  }
  
  void prep_data() {
    dt[0]=now.year()-2000; dt[1]=now.month(); dt[2]=now.day();
    dt[3]=now.hour(); dt[4]=now.minute(); dt[5]=now.second();
    
    //battery=ceil(volts*10);
    accu_rain=ceil(drops*0.01*100);
    rain_rate=ceil(drops*0.01*60*10);// 1 minute averaging rain rate in mm/hr 
  }
  
  void save_data() {
    File DataFile = FileSystem.open(fn, FILE_APPEND);
    DataFile.print(dt[0],DEC); DataFile.print(","); DataFile.print(dt[1],DEC); DataFile.print(",");
    DataFile.print(dt[2],DEC); DataFile.print(","); DataFile.print(dt[3],DEC); DataFile.print(",");
    DataFile.print(dt[4],DEC); DataFile.print(","); DataFile.print(dt[5],DEC); DataFile.print(",");
    DataFile.print(drops);     DataFile.print(","); DataFile.print(accu_rain); DataFile.print(",");
    DataFile.print(rain_rate); //DataFile.print(","); DataFile.println(battery);
    DataFile.close();
  }
  
  //void get_battery() {
  //  int analogPin0 = 0; 
  //  volts = (analogRead(analogPin0))*(5.00/1024)*(5.1/(9.1+5.1)); // 0-5V range, 10 bit DAC, R1=9K, r2=5K 
  //}
  
  void loop() {
    now = RTC.now();
    
    //if (now.minute()!=currentminute){ // 1 minute average
    //  drops=pulseCount; pulseCount=0;
    //  currentminute=now.minute();
    //  if (now.day()!=currentday){
    //    currentday=now.day();
    //    make_file();
    //  }
      
      //get_battery();
    //  prep_data();
    //  save_data();
    //}  
  }
