//#include "user_interface.h"
#include "hSensor.h"
// some control values init
//int MaxVal = 3;  // number of cycles for sensor off
//int value = 0;   // initial value
//long int TOO = 0;
long int TCycle = 0;                        // cycling time
long int maxMin = 60;                       // max minutes before reset or self ckeck one hour here
long int maxTCycle = 1000 * (maxMin * 60);  // 1 hour to reset in msec
long int idESP = ESP32getChipId();
String sMSG = "";
bool adc_conversion_done = false;
//*****************************************************************************

int iS[40];  // global integers max 40 readings
uint8_t adc_pins[] = {
  36,
  39,
  34,
  35,
  32,
  33
};

uint8_t digi_pins[] = {
  25,
  26,
  27,
  14,
  12,
  13,
  23,
  22,
  19,
  18,
  5,
  17,
  16
};
uint8_t digi_pins_count = sizeof(digi_pins) / sizeof(uint8_t);  // number of digi pins
uint8_t adc_pins_count = sizeof(adc_pins) / sizeof(uint8_t);    // number total of pins
volatile bool adc_coversion_done = false;                       // Addc conversion done switch
adc_continuous_data_t *result = NULL;                           // pointer to results
_intJoy xintJoy;                                                // variable// Joystick output array
_flt smJoy;                                                     //filtering array
//String outString;                                               // data string broadcasted after reading inputs
//*****************************************************************************
// Check conversion
//*****************************************************************************
void ARDUINO_ISR_ATTR adcComplete() {
  adc_coversion_done = true;
}
//*****************************************************************************
// read inputs and pack them in a string fill global array of int
//*****************************************************************************
String readAnalogs() {
  if (!adc_coversion_done) return "";

  adc_conversion_done = false;

  if (!analogContinuousRead(&result, 0)) return "Error";

  String outString = "*";
  for (int i = 0; i < adc_pins_count; ++i) {
    int val = constrain(result[i].avg_read_raw, 0, 4095);
    iS[i] = val;
    outString += "," + String(val);
  }

  delay(1);  // minimal delay for stability, if truly needed

  for (int i = 0; i < digi_pins_count; ++i) {
    int digVal = digitalRead(digi_pins[i]) ? 4095 : 0;
    iS[adc_pins_count + i] = digVal;

    if (i == 1 && digVal == 0) {
      delay(100);
      ESP.restart();
    }

    outString += "," + String(digVal);
  }
  //Serial.println(outString);
  return outString;
}
//=============================================================================

//==============================================================================
long int ESP32getChipId() {
  long int chipId = 0;
  for (int i = 0; i < 17; i = i + 8) {
    chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
  }
  return chipId;
}

int sgn(int a) {
  int s = 0;
  if (a > 0) { s = 1; }
  if (a < 0) { s = -1; }
  return s;
}
// Pretty print int -----------------------------------------------------------
void prInt(String T, double i) {
  char s[5];
  sprintf(s, "%05d", int(i));
  Serial.print(T + String(s) + "| ");
}
//===============================================================================
// transfer string
_intJoy TrS(String S) {
  _intJoy tk;
  //char sz[] = "  500,  499,  500,  499,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,  500,  500 ";  // prototipe
  char sz[] = "*,1567,2728,1389,2677,4095,1708,4095,4095,4095,0,4095,4095,4095,4095,4095,4095,4095,4095,4095";
    char buf[sizeof(sz)];
  if (S.indexOf("*,") != -1) {
    S.remove(0, 2);
  }
  S.toCharArray(buf, sizeof(buf));
  char *p = buf;
  char *str;
  int k = 0;
  while ((str = strtok_r(p, ",", &p)) != NULL) {
    tk._val[k] = atoi(str);
    k++;
  }
  return tk;
}
// angle calculation===========================================================
double_t Angle(int DX, int DY) {
  double_t A = 0;

  if ((DX != 0) && (DY != 0)) {

    A = (atan(fabs(DY) / fabs(DX)));  // first Q

    if (DX < 0 && DY > 0) {
      A = M_PI - A;
    }  // second Q2

    else if (DX < 0 && DY < 0) {
      A = M_PI + A;
    }  // thirdh Q3
    else if (DX > 0 && DY < 0) {
      A = (2.0 * M_PI) - A;
    }  //Q4
  }

  if ((DX == 0) && (DY > 0)) { A = M_PI / 2; }      // fw
  if ((DX == 0) && (DY < 0)) { A = 3 * M_PI / 2; }  // rw

  if ((DX > 0) && (DY == 0)) { A = 0; }     //
  if ((DX < 0) && (DY == 0)) { A = M_PI; }  //
  if ((DX == 0) && (DY == 0)) { A = 0.0; }  //

  return A;
}  //end of Angle

// AI Angle calc ===============================================================
double_t AIAngle(int DX, int DY) {
  if (DX == 0 && DY == 0) return 0.0;

  double_t angle = atan2((double)DY, (double)DX);  // Handles all quadrants
  if (angle < 0) angle += 2.0 * M_PI;              // Normalize to [0, 2π)

  return angle;
}

// soft centering the joystick around neutraal point===========================
int mapJY(int JX, int JXN) {
  int tJX = JX;
  if (JX < 0) { JX = map(
                  tJX, -JXN, 0,
                  -2047, 0); }
  if (JX > 0) {
    JX = map(
      tJX, 0, (4095 - JXN),
      0, 2047);
  }
  return JX;
}
// Shift values in the input history===========================================
_flt shift(int x, _flt ret) {
  for (int k = 1; k < FLT; k++) { ret._[k] = ret._[k - 1]; }
  ret._[0] = x;
  return ret;
}

// average of the input history================================================
int _avg(_flt f) {
  int s = 0;
  for (int k = 0; k < FLT; k++) {
    s += f._[k];
  }
  s = int(s / FLT);
  return s;
}
// map a float to integer=======================================================
int mapFloatToInt(float x, float in_min, float in_max, int out_min, int out_max) {
  return (int)((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min);
}
