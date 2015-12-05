/*
 * MagicBrakeLight
 * enlavin@gmail.com
 * 
 * Uses an ADXL335 accelerometer to detect that the bike is stopping and lights up
 * a brake light (8x8 LED module). It also sends the sensor data over the serial
 * port to be used by other devices (i.e. via bluetooth).
 */

#define BAUDRATE 115200                //  Valid values: 1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200. 
#define DATA_SEP ","

#define BOARD_PROMINI 0
#define BOARD_LEONARDO 1
#define BOARD_UNO 0

#if BOARD_LEONARDO
  #define _SERIAL Serial1
#else
  #define _SERIAL Serial
#endif

#define SETUP_DELAY 1000
#define SENSOR_DELAY 50

#define WINDOW_SIZE 3

/* setup this axes to match the orientation of your sensor */
#define FORWARD_AXIS 0
#define SIDE_AXIS 1
#define VERTICAL_AXIS 2

#define INITIAL_BRAKE_THRESHOLD -60


int x, y, z;
unsigned int brake_light;
int brake_threshold;
int window[WINDOW_SIZE];

void setupSerial() {
  //Initialize serial and wait for port to open:
  _SERIAL.begin(BAUDRATE); 
  while (!_SERIAL) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  //  Change mybt to your device name, and 1234 to your pin
  _SERIAL.write("AT+NAMEFrenoBici");
  delay(SETUP_DELAY);
  /* uncomment this if you want to provide a PIN
  _SERIAL.write("AT+PIN1234");
  delay(SETUP_DELAY); */

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
}

void initWindow()
{
  /* Prefill the window. Most recent entries are on the lower addresses */
  for (int i=0; i<WINDOW_SIZE; i++)
  {
    window[WINDOW_SIZE - i - 1] = analogRead(FORWARD_AXIS);
    delay(SENSOR_DELAY);
  }
}

void setup() { 
  // ADXL335 uses 3.3V
  analogReference(EXTERNAL);

  setupSerial();
  initWindow();

  brake_light = LOW;

  brake_threshold = INITIAL_BRAKE_THRESHOLD;
}

int mean(int *window, int window_size)
{
  int sum = 0;
  for (int i = 0; i < window_size; i++)
  {
    sum += window[i];
  }

  return sum / window_size;
}

void scrollWindow(int *window, int window_size)
{
  memcpy(window + 1, window, window_size - 1);
}

void loop() {
  x = analogRead(FORWARD_AXIS); 
  y = analogRead(SIDE_AXIS); 
  z = analogRead(VERTICAL_AXIS); 

  brake_light = x - mean(window, WINDOW_SIZE) < brake_threshold;

  digitalWrite(13, brake_light);

  String accel = "";
  accel += String(brake_light);
  accel += DATA_SEP;
  accel += String(x);
  accel += DATA_SEP;
  accel += String(y);
  accel += DATA_SEP;
  accel += String(z);
  accel += DATA_SEP;
  accel += String(millis());
  _SERIAL.println(accel);
  delay(50);

  scrollWindow(window, WINDOW_SIZE);
}
