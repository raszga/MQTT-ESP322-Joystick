#include <Arduino.h>
#include <Wire.h>
#define CONVERSIONS_PER_PIN 10

struct _intJoy {
  int _val[50] = { 0 };
};
// filtering data
const int FLT = 16;  // filter length
struct _flt {
  int _[FLT] = { 0 };
};  // input hitory

extern int MaxVal;  // number of cycles for sensor off

extern String sMSG;
extern long int idESP;

//================Program control varialbles===================
extern long int TCycle;
extern long int maxMin;     // max number of minutes before reset
extern long int maxTCycle;  // 1 hour to reset

//=================================
extern int iS[40];  // global integers max 40 readings
extern uint8_t adc_pins[6];
extern uint8_t digi_pins[13];
extern uint8_t digi_pins_count;
extern uint8_t adc_pins_count;
extern volatile bool adc_coversion_done;
extern adc_continuous_data_t *result;
extern String outString;
extern _intJoy xintJoy;  // variable command joystick
long int ESP32getChipId();
// read analogs and digitals --------------------------------------------------
String readAnalogs();
//ckeck reading analogs -------------------------------------------------------
void ARDUINO_ISR_ATTR adcComplete();
// Functions-------------------------------------------------------------------
int sgn(int a);  // sign function
// pretty print int -----------------------------------------------------------
void prInt(String T, double i);
//---Translate string----------------------------------------------------------
_intJoy TrS(String S);
// angle calculation-----------------------------------------------------------
double_t Angle(int DX, int DY);
// soft center-----------------------------------------------------------------
int mapJY(int JX, int JXN);
// shift-----------------------------------------------------------------------
_flt shift(int x, _flt ret);
// avg--------------------------------------------------------------------------
int _avg(_flt f);
int mapFloatToInt(float x, float in_min, float in_max, int out_min, int out_max);
