
#include <Wire.h>
#include <TroykaIMU.h>
#include <Adafruit_NeoPixel.h>



#define   LED_PIN              12
#define   PIXEL_NUM            21

#define   FREE_FALL_ACCEL          ((float) 9.8)
#define   ACCEL_THRESHOLD          ((float) 0.5)
#define   FREE_FALL_LOW_THRESHOLD  ((float) (FREE_FALL_ACCEL - ACCEL_THRESHOLD))
#define   FREE_FALL_HIGH_THRESHOLD ((float) (FREE_FALL_ACCEL + ACCEL_THRESHOLD))

#define   FREE_FALL_COLOR          ((uint32_t) 0x05000000)
#define   LOW_VIBRATION_COLOR      ((uint32_t) 0x00000100)
#define   MEDIUM_VIBRATION_COLOR   ((uint32_t) 0x00010100)
#define   HIGH_VIBRATION_COLOR     ((uint32_t) 0x00010000)
#define   PEAK_LED_COLOR           ((uint32_t) 0x00010000)

#define   MEDIUM_VIBRATION_ZONE_START   11
#define   HIGH_VIBRATION_ZONE_START     17

#define   ACCEL_VALUE_PER_PIXEL         ((float) 1.5)
#define   SCALE_SHOW_DELAY              ((uint16_t) 30)//mc

#define   SCALE_DIRECTION_UP            0
#define   SCALE_DIRECTION_DOWN          1

#define   PEAK_PHASE_PAUSE              0
#define   PEAK_PHASE_FALLING            1

#define   PEAK_FALLING_VALUE            4//6
#define   PEAK_PAUSE_VALUE              15//20



typedef struct
{
  uint8_t MaximumLeds;
  uint8_t ActiveLEDs;
  uint8_t Direction;
  uint8_t PeakLEDIndex;
  uint8_t Counter;
  uint8_t PeakPhase;
  
} VibrationScaleType;



Adafruit_NeoPixel LEDS = Adafruit_NeoPixel(PIXEL_NUM, LED_PIN, NEO_GRBW + NEO_KHZ800);
Accelerometer accel;
VibrationScaleType VibroScale;


void setup()
{
  Serial.begin(115200);

  VibroScale.MaximumLeds = 0;
  VibroScale.ActiveLEDs = 0;
  VibroScale.Direction = SCALE_DIRECTION_UP;
  VibroScale.PeakLEDIndex = 1;
  VibroScale.Counter = PEAK_FALLING_VALUE - 1;
  VibroScale.PeakPhase = PEAK_PHASE_FALLING;  

  LEDS.begin();
  for(uint8_t i = 0; i < PIXEL_NUM; i++)
    LEDS.setPixelColor(i, FREE_FALL_COLOR);
    
  LEDS.show();
  
  accel.begin();
  accel.setRange(RANGE_2G);
}

void loop()
{
  if (VibroScale.ActiveLEDs == VibroScale.MaximumLeds)
  {
    float XVal;
    float YVal;
    float ZVal;

    accel.readAXYZ(&XVal, &YVal, &ZVal);
    float AccelResultNew = sqrt(XVal * XVal + YVal * YVal + ZVal * ZVal);
  
    if((AccelResultNew < FREE_FALL_LOW_THRESHOLD) || (AccelResultNew > FREE_FALL_HIGH_THRESHOLD))
    {
      VibroScale.MaximumLeds = (uint8_t)(AccelResultNew / ACCEL_VALUE_PER_PIXEL);
      if (VibroScale.MaximumLeds > (PIXEL_NUM - 1))
        VibroScale.MaximumLeds = (PIXEL_NUM - 1);
    }
    else
      VibroScale.MaximumLeds = 0;

    if (VibroScale.ActiveLEDs <= VibroScale.MaximumLeds)
      VibroScale.Direction = SCALE_DIRECTION_UP;
    else
      VibroScale.Direction = SCALE_DIRECTION_DOWN;     
        
    Serial.println(AccelResultNew);
  }
  
  ScaleUpdate();
  delay(SCALE_SHOW_DELAY);
}
//-------------------------------------------------------------
void ScaleUpdate()
{
  if ((VibroScale.Direction == SCALE_DIRECTION_UP) && (VibroScale.ActiveLEDs < VibroScale.MaximumLeds))
    VibroScale.ActiveLEDs++;    
  else if ((VibroScale.Direction == SCALE_DIRECTION_DOWN) && (VibroScale.ActiveLEDs > VibroScale.MaximumLeds))
    VibroScale.ActiveLEDs--;

  if (VibroScale.PeakLEDIndex >= VibroScale.ActiveLEDs)
  {
    VibroScale.Counter++;
      
    if ((VibroScale.PeakPhase == PEAK_PHASE_PAUSE) && (VibroScale.Counter == PEAK_PAUSE_VALUE))
    {
      VibroScale.PeakPhase = PEAK_PHASE_FALLING;        
      VibroScale.Counter = PEAK_FALLING_VALUE - 1;
    }
      
    else if ((VibroScale.PeakPhase == PEAK_PHASE_FALLING) && (VibroScale.Counter == PEAK_FALLING_VALUE))
    { 
      if (VibroScale.PeakLEDIndex > VibroScale.ActiveLEDs)
        VibroScale.PeakLEDIndex--;
      VibroScale.Counter = 0;
    }
  }
  else if (VibroScale.Direction == SCALE_DIRECTION_UP)
  {
    VibroScale.PeakLEDIndex = VibroScale.ActiveLEDs;
    if (VibroScale.PeakPhase == PEAK_PHASE_FALLING)
    {
      VibroScale.PeakPhase = PEAK_PHASE_PAUSE;
      VibroScale.Counter = 0;
    }
  }
  
  for(uint8_t led = 0; led < PIXEL_NUM; led++)
  {
    if (led < VibroScale.ActiveLEDs)
    {
      if (led < MEDIUM_VIBRATION_ZONE_START)
        LEDS.setPixelColor(led, LOW_VIBRATION_COLOR);
      else if (led < HIGH_VIBRATION_ZONE_START)
        LEDS.setPixelColor(led, MEDIUM_VIBRATION_COLOR);
      else
        LEDS.setPixelColor(led, HIGH_VIBRATION_COLOR);
    }

    else
      LEDS.setPixelColor(led, FREE_FALL_COLOR);
  }

  LEDS.setPixelColor(VibroScale.PeakLEDIndex, PEAK_LED_COLOR);

  LEDS.show();
}






