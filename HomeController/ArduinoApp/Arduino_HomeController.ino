#include <Wire.h>
#include <LiquidCrystal.h> 
#include <SD.h>
#include <DS1307.h>

//Load Liquid Crystal Library
LiquidCrystal lcd(43, 42, 47, 46, 45, 44);

// Init the DS1307
DS1307 rtc(SDA, SCL);

 // will store last time LED was updated
unsigned long previousMillis = 0;       
// constants won't change :
const long interval = 10000;
 
// Raindrops sensor lowest and highest sensor readings:
const int sensorMin = 0;     // sensor minimum
const int sensorMax = 1024;  // sensor maximum
const int chipSelect = 4; //sd card
const byte HUMPin = A0; //analog pin humidity
const byte RAINDROP = A1;
float temperature;
float humidity;
String raindrop;
int countLine=0;
String sdcardReadArray[60];

//*************for thermistor************//
const byte NTCPin = A2;
#define SERIESRESISTOR 10000
#define NOMINAL_RESISTANCE 10000
#define NOMINAL_TEMPERATURE 25
#define BCOEFFICIENT 3435

void setup() 
{
 Wire.begin();
 lcd.begin(16,2);
 
// set cursor position to start of first line on the LCD
lcd.setCursor(0,0);
 
//text to print
Serial.begin(115200);
 
// Initialize the rtc object
rtc.begin();
  
// Set the clock to run-mode
rtc.halt(false);
 
// The following lines can be commented out to use the values already stored in the DS1307
rtc.setDOW(TUESDAY);        // Set Day-of-Week to SUNDAY
rtc.setTime(7, 11, 0);     // Set the time to 12:00:00 (24hr format)
rtc.setDate(6, 20, 2017);   // Set the date to October 3th, 2010

// Set SQW/Out rate to 1Hz, and enable SQW
rtc.setSQWRate(SQW_RATE_1);
rtc.enableSQW(true);
pinMode(4, OUTPUT); // required for SD lib
digitalWrite(4,HIGH);  

 if (!SD.begin(chipSelect))
 {
 // chipselect cs=pin 53
// Serial.println("Card failed, or not present");
lcd.setCursor(0,1);
lcd.print("SD Card Failed ");
return;
}
 
lcd.print("Hello");
delay(1000);
lcd.clear();
}

void loop()
{
  //*****************************Raindrops sensor***************************
   // read the rain sensor on analog A0:
  int sensorReading = sample(RAINDROP);
 
  // map the sensor range (four options):
  // ex: 'long int map(long int, long int, long int, long int, long int)'
  int range = map(sensorReading, sensorMin, sensorMax, 0, 3);
  
  // range value:
  switch (range) 
 {
 case 0:    // Sensor getting wet
 lcd.setCursor(0,0);
 raindrop="Flood";
 lcd.print(raindrop+"           ");
    break;
 case 1:
  lcd.setCursor(0,0);
  raindrop="Rain Warning";
  lcd.print(raindrop+"    ");
    break;
 case 2:    // Sensor dry - To shut this up delete the " Serial.println("Not Raining"); " below.
  lcd.setCursor(0,0);
  raindrop="Not Raining";
  lcd.print(raindrop+"           ");
    break;
  }
 
  //***************************** END   Raindrops sensor***************************
 
float ADCvalue;
float Resistance;
ADCvalue = sample(NTCPin);         
Resistance = ((1023.0/3.3)*4.9/ADCvalue) - 1;  //1023/ADCvalue////////***///**//
Resistance = SERIESRESISTOR * Resistance;

temperature = Resistance / NOMINAL_RESISTANCE; // (R/Ro)
temperature = log(temperature); // ln(R/Ro)
temperature /= BCOEFFICIENT; // 1/B * ln(R/Ro)
temperature += 1.0 / (NOMINAL_TEMPERATURE + 273.15); // + (1/To)
temperature = 1.0 / temperature; // Invert
temperature -= 273.15; // convert to C
//Serial.print("Temp: ");
// Serial.println((String)temperature);//print temp
lcd.setCursor(0,1);
lcd.print("T: "+ String(temperature)+"  ");
//********************************end temp code************************

//************************Humidity code:***********
//getting the readings from the sensors
float humvolt=sample(HUMPin)/(1023.0/3.3);//////////*******////////**///**//**/
humidity =(humvolt/0.03);
 
//   Serial.println("volt: "+String(humvolt)+" Humidity: "+ String(humidity));
lcd.setCursor(9,1);
lcd.print("H: "+ String(humidity)+"  ");
//********************************end humidity code************************
 
LogToSD();
delay(100);
Serial.println("0)"+String(rtc.getTimeStr())+","+String(rtc.getDOWStr(FORMAT_SHORT))+","+String(rtc.getDateStr())+","+String(raindrop)+","+String(temperature)+","+String(humidity));  
delay(3500);
wifisent();  
}

void wifisent()
{
  File myFile = SD.open("OLAYAN.csv");
  countLine=0;
 if(myFile)
 {
  while (myFile.available() && countLine<10)
  {
      countLine++;
   
     if(!myFile.readStringUntil('\n').equals(""))
     {
     //  sdcardReadArray[countLine]=myFile.readStringUntil('\n');
      Serial.println(String(countLine)+")"+myFile.readStringUntil('\n'));
      delay(3500);
     }
  }
  
  // close the file:
    myFile.close();}

  else 
  {
  // if the file didn't open, print an error:
 // Serial.println("error opening test.txt");
  lcd.setCursor(0,0);
  lcd.print("error sdcard");
  delay(2000);
 }
}

  void LogToSD()
  {
   // see if the card is present and can be initialized:
   // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
    File dataFile = SD.open("olayan.csv", FILE_WRITE);
   
  // if the file is available, write to it:
  if (dataFile) 
  {
     dataFile.print("time ");
    dataFile.print(rtc.getTimeStr());
    dataFile.print(", day ");
    dataFile.print(rtc.getDOWStr(FORMAT_SHORT));
    dataFile.print(", date ");
    dataFile.print(rtc.getDateStr());
    dataFile.print(", weather ");
    dataFile.print(raindrop);
    dataFile.print(",temp ");
    dataFile.print(String(temperature));
    dataFile.print(", hum");
    dataFile.print(String(humidity));
    dataFile.println();
    dataFile.close();
    } 
   
  // if the file isn't open, pop up an error:
  else 
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("error sd card   ");
    SD.begin(chipSelect);
    delay(5000);
    lcd.clear();
      }
 }

 float sample(byte z)
/* This function will read the Pin 'z' 5 times and take an average.*/
{
  byte i;
  int sval = 0;
  for (i = 0; i < 10; i++)
  {
  sval = sval + analogRead(z);// sensor on analog pin 'z'
  }
  sval = sval / 10;    // average
  return sval;
}

//****************************************************************************
String getStringPartByNr(String data, char separator, int index)
{
    // spliting a string and return the part nr index
    // split by separator
    int stringData = 0;        //variable to count data part nr 
    String dataPart = "";      //variable to hole the return text
    
    for(int i = 0; i<data.length()-1; i++)
    {   
     //Walk through the text one letter at a time
      if(data[i]==separator) 
      {
        //Count the number of times separator character appears in the text
        stringData++;
      }
     else if(stringData==index)
     {
        //get the text when separator is the rignt one
        dataPart.concat(data[i]);  
      }
     else if(stringData>index)
     {
        //return text and stop if the next separator appears - to save CPU-time
        return dataPart;
        break; 
      }
    }
    //return text if this is the last part
    return dataPart;
}
