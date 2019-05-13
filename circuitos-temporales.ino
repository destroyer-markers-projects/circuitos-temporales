#include "SevenSegmentTM1637.h"
#include "SevenSegmentExtended.h"
#include "DS1307RTC.h"
#include "RTClib.h"

const byte PIN_CLK_Red = A0;   // define CLK pin 
const byte PIN_CLK_Green = A1;   // define CLK pin 
const byte PIN_CLK_Orange = A2;   // define CLK pin 

//RED Displays
const byte PIN_DIO_R1 = 3;
SevenSegmentExtended      red1(PIN_CLK_Red, PIN_DIO_R1);
const byte PIN_DIO_R2 = 4;
SevenSegmentTM1637       red2(PIN_CLK_Red, PIN_DIO_R2);
const byte PIN_DIO_R3 = 5;
SevenSegmentExtended     red3(PIN_CLK_Red, PIN_DIO_R3);

//GREEN Displays
const byte PIN_DIO_G1 = 6;
SevenSegmentExtended      green1(PIN_CLK_Green, PIN_DIO_G1);
const byte PIN_DIO_G2 = 7;
SevenSegmentTM1637       green2(PIN_CLK_Green, PIN_DIO_G2);
const byte PIN_DIO_G3 = 8;
SevenSegmentExtended     green3(PIN_CLK_Green, PIN_DIO_G3);
int greenAM = 12;
int greenPM = 13;

// ORANGE Displays
const byte PIN_DIO_O1 = 9;   // define DIO pin (any digital pin)
SevenSegmentExtended      orange1(PIN_CLK_Orange, PIN_DIO_O1);
const byte PIN_DIO_O2 = 10;   
SevenSegmentTM1637        orange2(PIN_CLK_Orange, PIN_DIO_O2); 
const byte PIN_DIO_O3 = 11;   
SevenSegmentExtended       orange3(PIN_CLK_Orange, PIN_DIO_O3); 

bool parse=false;
bool config=false;
tmElements_t tm;
int Hour=0;

const char *monthName[12] = {
  "Ene", "Feb", "Mar", "Abr", "May", "Jun",
  "Jul", "Ago", "Sep", "Oct", "Nov", "Dic"
};

void setup() {

  pinMode(PIN_CLK_Red, OUTPUT);
  pinMode(PIN_CLK_Green, OUTPUT);
  pinMode(PIN_CLK_Orange, OUTPUT);
  pinMode(PIN_DIO_O1, OUTPUT);
  pinMode(PIN_DIO_O2, OUTPUT);
  pinMode(PIN_DIO_O3, OUTPUT);
  pinMode(PIN_DIO_G1, OUTPUT); 
  pinMode(PIN_DIO_G2, OUTPUT);
  pinMode(PIN_DIO_G3, OUTPUT); 
  pinMode(PIN_DIO_R1, OUTPUT);
  pinMode(PIN_DIO_R2, OUTPUT);
  pinMode(PIN_DIO_R3, OUTPUT);
  pinMode(greenAM, OUTPUT);
  pinMode(greenPM, OUTPUT);

  Serial.begin(9600);       // initializes the Serial connection @ 9600 baud
  orange1.begin();            // initializes the display
  orange2.begin();
  orange3.begin();              
  green1.begin();
  green2.begin();
  green3.begin();            
  red1.begin();
  red2.begin();
  red3.begin();            
  orange1.setBacklight(100);  // set the brightness to 100 %
  orange2.setBacklight(100);
  orange2.setColonOn(0); // Switch off ":" for orange "year"
  orange3.setBacklight(100);  
  green1.setBacklight(100); 
  green2.setBacklight(100);
  green3.setBacklight(100); 
  red1.setBacklight(100);
  red2.setBacklight(100);
  red3.setBacklight(100);  
  red2.setColonOn(0); // Switch off ":" for red "year"

/* 1)Décommenter les lignes suivantes pour mettre l'heure à jour
 2)Téléverser le code vers l'arduino
 3)Recommenter
 4)Téléverser de nouveau le code vers l'arduino
 */
 
/* 
 1)uncomment the flowing lines to update the time
 2)Send your code to the arduino
 3)re-comment
 4)send your code again to the arduino
 */
 
/*
// get the date and time the compiler was run
  if (getDate(__DATE__) && getTime(__TIME__)) {
    parse = true;
    // and configure the RTC with this info
    if (RTC.write(tm)) {
      config = true;
    }
  }
  Serial.begin(9600);
  while (!Serial) ; // wait for Arduino Serial Monitor
  delay(200);
  if (parse && config) {
    Serial.print("DS1307 configured Time=");
    Serial.print(__TIME__);
    Serial.print(", Date=");
    Serial.println(__DATE__);
  } else if (parse) {
    Serial.println("DS1307 Communication Error :-{");
    Serial.println("Please check your circuitry");
  } else {
    Serial.print("Could not parse info from the compiler, Time=\"");
    Serial.print(__TIME__);
    Serial.print("\", Date=\"");
    Serial.print(__DATE__);
    Serial.println("\"");
  }
*/
}

void loop() {
 
  if (RTC.read(tm)) {
    Serial.print("Ok, Time = ");
    print2digits(tm.Hour);
    Serial.write(':');
    print2digits(tm.Minute);
    Serial.write(':');
    print2digits(tm.Second);
    Serial.print(", Date (D/M/Y) = ");
    Serial.print(tm.Day);
    Serial.write('/');
    Serial.print(tm.Month);
    Serial.write('/');
    Serial.print(tmYearToCalendar(tm.Year));
    Serial.println();
  }
  else {
    if (RTC.chipPresent()) {
      Serial.println("The DS1307 is stopped.  Please run the SetTime");
      Serial.println("example to initialize the time and begin running.");
      Serial.println();
    } else {
      Serial.println("DS1307 read error!  Please check the circuitry.");
      Serial.println();
    }
    delay(9000);
  }
  delay(1000);


//Convert 12/24h and lighting AM/PM
if(tm.Hour>=13){
  Hour = tm.Hour - 12;
  digitalWrite(greenAM,0);
  digitalWrite(greenPM,1);}
else if(tm.Hour==12)
  {Hour=tm.Hour;
  digitalWrite(greenAM,0);
  digitalWrite(greenPM,1);}
else{
  Hour=tm.Hour;
  digitalWrite(greenAM,1);
  digitalWrite(greenPM,0);}

int year_red = 2028;
int year_orange = 1985;

//Res displays -  Destination TIME 11.05 2028 10:04
red1.printTime(11, 05, true);
red2.print(year_red, true);
red3.printTime(10, 07, true); 

//Green Displays - Present TIME - Heure actuelle
green1.printTime(tm.Month, tm.Day, true);  
green2.print(tmYearToCalendar(tm.Year));
green3.printTime(Hour, tm.Minute, true);

// Orange Displays - Last time departed 10.26 1985 01:24                      
orange1.printTime(10, 26, true);                     
orange2.print(year_orange, true);
orange3.printTime(01, 24, true);
                     
}


//============================= functions =============================//

void print2digits(int number) {
  if (number >= 0 && number < 10) {
    Serial.write('0');
  }
  Serial.print(number);
}

bool getTime(const char *str)
{
  int Hour, Min, Sec;

  if (sscanf(str, "%d:%d:%d", &Hour, &Min, &Sec) != 3) return false;
  tm.Hour = Hour;
  tm.Minute = Min;
  tm.Second = Sec;
  return true;
}

bool getDate(const char *str)
{
  char Month[12];
  int Day, Year;
  uint8_t monthIndex;

  if (sscanf(str, "%s %d %d", Month, &Day, &Year) != 3) return false;
  for (monthIndex = 0; monthIndex < 12; monthIndex++) {
    if (strcmp(Month, monthName[monthIndex]) == 0) break;
  }
  if (monthIndex >= 12) return false;
  tm.Day = Day;
  tm.Month = monthIndex + 1;
  tm.Year = CalendarYrToTm(Year);
  return true;
}
