/* ADXL335 Simple Test*/
#include <SD.h>

#define SD_CARD_CD_DIO 4 /* DIO pin used to control the modules CS pin */

#define BAUDRATE 115200                //  Valid values: 1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200. 
#define SEP ","

#define BOARD_PROMINI 0
#define BOARD_LEONARDO 1
#define BOARD_UNO 0

#if BOARD_LEONARDO
  #define _SERIAL Serial1
#else
  #define _SERIAL Serial
#endif

#define SETUP_DELAY 1000


int i;
unsigned int x,y,z;
unsigned int freno;

void setup() { 
  /*pinMode(0, INPUT_PULLUP);*/
  //Initialize serial and wait for port to open:
  _SERIAL.begin(BAUDRATE); 
  while (!_SERIAL) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  //  Change mybt to your device name, and 1234 to your pin
  _SERIAL.write("AT+NAMEFrenoBici");
  delay(SETUP_DELAY);
  /*_SERIAL.write("AT+PIN1234");*/
  /*delay(SETUP_DELAY);*/
  switch(BAUDRATE)  {
     case 1200:    _SERIAL.write("AT+BAUD1");  break; 
     case 2400:    _SERIAL.write("AT+BAUD2");  break; 
     case 4800:    _SERIAL.write("AT+BAUD3");  break; 
     case 9600:    _SERIAL.write("AT+BAUD4");  break; 
     case 19200:   _SERIAL.write("AT+BAUD5");  break; 
     case 38400:   _SERIAL.write("AT+BAUD6");  break;
     case 57600:   _SERIAL.write("AT+BAUD7");  break; 
     case 115200:  _SERIAL.write("AT+BAUD8");  break;
     default:      _SERIAL.write("AT+BAUD4");  break; 
  }
  delay(SETUP_DELAY);

  analogReference(EXTERNAL);
  i = 0;
  freno = LOW;

  // see if the card is present and can be initialized:
  if (!SD.begin(SD_CARD_CD_DIO)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");
}

void loop() {
  x = analogRead(0); 
  y = analogRead(1); 
  z = analogRead(2); 

  freno = (x > 600) ? freno = HIGH : freno = LOW;

  digitalWrite(13, freno);

  String accel = "";
  accel += String(x);
  accel += SEP;
  accel += String(y);
  accel += SEP;
  accel += String(z);
  accel += SEP;
  accel += String(millis());
  _SERIAL.println(accel);

  File dataFile = SD.open("bici.tsv", FILE_WRITE);
  if (dataFile) {
    dataFile.println(accel);
    dataFile.close();
  }  
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening file");
  }   
  /*if (freno == LOW)*/
    delay(50);
  /*else*/
  /*{*/
    /*_SERIAL.println("Freno!");*/
    /*delay(2000);*/
  /*}*/

  i++;
}
