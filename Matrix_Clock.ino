#include <LedControl.h>

#define DIN 11
#define CS_LOAD 10
#define CLK 13

LedControl lc = LedControl(DIN, CLK, CS_LOAD, 1);

int hours = 0;
int minutes = 0;
int seconds = 0;

bool timeSynced = false;

unsigned long previousMillis = 0;
const long interval = 1000;

// -------- FONTS (same as yours) --------
const byte hourFont[13][5] = {
  {B0000,B0000,B0000,B0000,B0000},
  {B0010,B0110,B0010,B0010,B0111},
  {B1110,B0001,B0110,B1000,B1111},
  {B1110,B0001,B0110,B0001,B1110},
  {B0011,B0101,B1001,B1111,B0001},
  {B1111,B1000,B1110,B0001,B1110},
  {B0110,B1000,B1110,B1001,B0110},
  {B1111,B0001,B0010,B0100,B0100},
  {B0110,B1001,B0110,B1001,B0110},
  {B1111,B1001,B1111,B0001,B1111},
  {B1010,B1101,B1101,B1101,B1010},
  {B1010,B1010,B1010,B1010,B1010},
  {B1011,B1001,B1011,B1010,B1011}
};

const byte minOnesFont[10][5] = {
  {B111,B101,B101,B101,B111},
  {B010,B110,B010,B010,B111},
  {B111,B001,B111,B100,B111},
  {B111,B001,B111,B001,B111},
  {B101,B101,B111,B001,B001},
  {B111,B100,B111,B001,B111},
  {B111,B100,B111,B101,B111},
  {B111,B001,B001,B001,B001},
  {B111,B101,B111,B101,B111},
  {B111,B101,B111,B001,B111}
};

const int minTensCoords[5][2] = {
  {0,7},{0,6},{0,5},{1,7},{1,6}
};

const int secCoords[6][2] = {
  {6,0},{6,1},{6,2},
  {7,0},{7,1},{7,2}
};

void setup() {
  Serial.begin(9600);
  lc.shutdown(0,false);
  lc.setIntensity(0,8);
  lc.clearDisplay(0);

  Serial.println("Send time as: TIME HH MM SS");
}

void loop() {

  checkSerialSync();

  if (!timeSynced) return;

  if (millis() - previousMillis >= interval) {
    previousMillis = millis();
    incrementTime();
    updateDisplay();
  }
}

void checkSerialSync() {
  if (Serial.available()) {

    String cmd = Serial.readStringUntil('\n');
    cmd.trim();

    if (cmd.startsWith("TIME")) {

      int first = cmd.indexOf(' ');
      int second = cmd.indexOf(' ', first + 1);
      int third = cmd.indexOf(' ', second + 1);

      hours = cmd.substring(first+1, second).toInt();
      minutes = cmd.substring(second+1, third).toInt();
      seconds = cmd.substring(third+1).toInt();

      if (hours > 12) hours -= 12;
      if (hours == 0) hours = 12;

      timeSynced = true;
      updateDisplay();
    }
  }
}

void incrementTime() {

  seconds++;

  if (seconds >= 60) {
    seconds = 0;
    minutes++;

    if (minutes >= 60) {
      minutes = 0;
      hours++;

      if (hours > 12)
        hours = 1;
    }
  }
}

void updateDisplay() {

  lc.clearDisplay(0);

  int displayHour = hours;

  for (int row = 0; row < 5; row++) {
    byte rowData = hourFont[displayHour][row];
    for (int col = 0; col < 4; col++) {
      if ((rowData >> (3-col)) & 0x01)
        lc.setLed(0,row,col,true);
    }
  }

  int tensDigit = minutes / 10;

  for (int i=0; i<tensDigit && i<5; i++)
    lc.setLed(0,minTensCoords[i][0],minTensCoords[i][1],true);

  int onesDigit = minutes % 10;

  for (int row=0; row<5; row++) {
    byte rowData = minOnesFont[onesDigit][row];
    for (int col=0; col<3; col++) {
      if ((rowData >> (2-col)) & 0x01)
        lc.setLed(0,row+2,col+5,true);
    }
  }

  int secCount = seconds % 6;

  if (secCount == 0 && seconds != 0)
    secCount = 6;

  if (seconds == 0)
    secCount = 0;

  for (int i=0; i<secCount; i++)
    lc.setLed(0,secCoords[i][0],secCoords[i][1],true);
}
