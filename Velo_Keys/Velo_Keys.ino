
#include <Adafruit_NeoPixel.h>
#include "Velo.h"
#include <PinChangeInt.h>


Adafruit_NeoPixel LEDS = Adafruit_NeoPixel(PIXEL_NUM, LED_PIN, NEO_GRBW + NEO_KHZ800);


const uint32_t *TURN_LEFT_PICTURES[] = { Picture_1, Picture_2, Picture_3, Picture_4, Picture_5, Picture_6, Picture_7, Picture_8, Picture_9, Picture_10, Picture_11, Picture_22, Picture_23};

const uint32_t *TURN_LEFT_STOP_PICTURES[] = { Picture_24, Picture_25, Picture_26, Picture_27, Picture_28, Picture_29, Picture_30, Picture_31, Picture_32, Picture_33, Picture_34, Picture_22, Picture_23};

const uint32_t *TURN_RIGHT_PICTURES[] = { Picture_1, Picture_2, Picture_3, Picture_4, Picture_5, Picture_6, Picture_7, Picture_8, Picture_9, Picture_10, Picture_11, Picture_22, Picture_23};

const uint32_t *TURN_RIGHT_STOP_PICTURES[] = { Picture_24, Picture_25, Picture_26, Picture_27, Picture_28, Picture_29, Picture_30, Picture_31, Picture_32, Picture_33, Picture_34, Picture_22, Picture_23};

const uint32_t *STOP_PICTURES[] = { Picture_69, Picture_70};

const uint32_t *SIDE_PICTURES[] = { Picture_71, };



bool StopState = false;
bool SideState = true;
uint16_t TurnLeftCnt = TURN_SHOW_DELAY - 1;
uint8_t TurnLeftBlinkCnt = TURN_INDICATOR_BLINK_VALUE - 1;
uint8_t TurnLeftBlinkPhase = TURN_INDICATOR_PHASE_OFF;
uint16_t TurnRightCnt = TURN_SHOW_DELAY - 1;
uint8_t TurnRightBlinkCnt = TURN_INDICATOR_BLINK_VALUE - 1;
uint8_t TurnRightBlinkPhase = TURN_INDICATOR_PHASE_OFF;
uint16_t StopCnt = STOP_SHOW_DELAY - 1;
uint16_t SideCnt = SIDE_SHOW_DELAY - 1;
uint8_t TurnLeftPic = 0;
uint8_t TurnRightPic = 0;
uint8_t StopPic = 0;
bool StopActive = false;
bool LEDSUpdate = false;
volatile uint8_t LeftState = TURN_RESET;
volatile uint8_t RightState = TURN_RESET;

void setup()
{  
  pinMode(TURN_LEFT_PIN, INPUT);
  digitalWrite(TURN_LEFT_PIN, HIGH);
  PCintPort::attachInterrupt(TURN_LEFT_PIN, &KeyLeft_ISR, FALLING);
  pinMode(TURN_RIGHT_PIN, INPUT);
  digitalWrite(TURN_RIGHT_PIN, HIGH);
  PCintPort::attachInterrupt(TURN_RIGHT_PIN, &KeyRight_ISR, FALLING);  
  pinMode(STOP_PIN, INPUT);
  digitalWrite(STOP_PIN, HIGH);
  
  LEDS.begin();
  for (uint8_t j = TURN_LEFT_PIXEL_START; j < (TURN_LEFT_PIXEL_NUM + TURN_RIGHT_PIXEL_NUM); j++)
    LEDS.setPixelColor(j, TURN_BACKGROUND);
  
  LEDS.show();
}
//----------------------------------------
void loop()
{ 
  //if (digitalRead(STOP_PIN))
  if (!digitalRead(STOP_PIN))
  {
    SideReset();
    
    if (!StopActive)
    {
      //TurnLeftReset();
      //TurnRightReset();
        
      SideReset();      
      StopActive = true;
      StopState = true;
      StopPic = 0;
      StopCnt = STOP_SHOW_DELAY - 1;
      for (uint8_t j = STOP_PIXEL_START; j < (STOP_PIXEL_START + STOP_PIXEL_NUM); j++)
        LEDS.setPixelColor(j, STOP_ACTIVE_COLOR);

      for (uint8_t j = STOP_INDICATOR_START; j < (STOP_INDICATOR_START + STOP_INDICATOR_PIXEL_NUM); j++)
        LEDS.setPixelColor(j, STOP_INDICATOR_COLOR);
      
      LEDSUpdate = true;
    }
  }
  else if (StopState)
  {
    SideReset();    
    StopActive = false;
    StopCnt++;

    if (StopCnt == STOP_SHOW_DELAY)
    {
      StopCnt = 0;

      if (StopPic < STOP_PICTURE_NUM)
      {
        ChangePicture(STOP_PICTURES[StopPic], STOP_PIXEL_START, STOP_PIXEL_NUM);
        StopPic++;
      }
      else
      {
        StopState = false;
        StopPic = 0;
        StopCnt = STOP_SHOW_DELAY - 1;
        
        for (uint8_t j = STOP_INDICATOR_START; j < (STOP_INDICATOR_START + STOP_INDICATOR_PIXEL_NUM); j++)
          LEDS.setPixelColor(j, 0);

        SideState = true;      
      }

      LEDSUpdate = true;
    }
  }
  
  if (SideState)
  {
    SideCnt++;
    if (SideCnt == SIDE_SHOW_DELAY)
    {
      
      //static uint16_t j = 0;
      uint8_t PixelStart;
      uint8_t PixelNum;
      LEDSUpdate = true;
      SideCnt = 0;

      if (LeftState == TURN_ON)
      {
        PixelStart = TURN_RIGHT_PIXEL_START;
        PixelNum = TURN_RIGHT_PIXEL_NUM;        
      }

      else if (RightState == TURN_ON)
      {
        PixelStart = TURN_LEFT_PIXEL_START;
        PixelNum = TURN_LEFT_PIXEL_NUM;        
      }

      else
      {
        PixelStart = SIDE_PIXEL_START;
        PixelNum = SIDE_PIXEL_NUM;        
      }

      for(uint8_t i=0; i< PixelNum; i++)
        LEDS.setPixelColor(i + PixelStart, TURN_BACKGROUND);

/*
      for(uint8_t i=0; i< PixelNum; i++)
        LEDS.setPixelColor(i + PixelStart, Wheel(((i * 256 / PixelNum) + j) & 255));     

      j++;
      if (j == 256*5)
        j = 0;
*/        
    }
  }

  TurnRightCheck();  
  TurnLeftCheck();
  
  if(LEDSUpdate)
  {
    LEDS.show();
    LEDSUpdate = false;
  }

  delay(1);
}
//----------------------------------------
void ChangePicture(const uint32_t *CharPixel, uint8_t StartPixel, uint8_t PixelCount)
{
  for (uint8_t PixelIndex = 0; PixelIndex < PixelCount; PixelIndex++)
    LEDS.setPixelColor(PixelIndex + StartPixel, pgm_read_dword(&CharPixel[PixelIndex]));
}
//--------------------------------------------
void TurnLeftCheck()
{
  if (LeftState == TURN_ON)
  {
    //if (SideState)
      //SideReset();

    TurnLeftCnt++;

    if (TurnLeftCnt == TURN_SHOW_DELAY)
    {
      TurnLeftCnt = 0;
      LEDSUpdate = true;

      TurnLeftBlinkCnt++;
      if (TurnLeftBlinkCnt == TURN_INDICATOR_BLINK_VALUE)
      {
        TurnLeftBlinkCnt = 0;
        
        if (TurnLeftBlinkPhase == TURN_INDICATOR_PHASE_OFF)
        {
          TurnLeftBlinkPhase = TURN_INDICATOR_PHASE_ON;
          
          for(uint8_t i = 0; i < TURN_LEFT_INDICATOR_PIXEL_NUM; i++)
            LEDS.setPixelColor(TURN_LEFT_INDICATOR_START + i, TURN_INDICATOR_COLOR);
        }
        else
        {
          TurnLeftBlinkPhase = TURN_INDICATOR_PHASE_OFF;
          
          for(uint8_t i = 0; i < TURN_LEFT_INDICATOR_PIXEL_NUM; i++)
            LEDS.setPixelColor(TURN_LEFT_INDICATOR_START + i, 0);
        }
      }

      //if(!digitalRead(STOP_PIN))
      if (digitalRead(STOP_PIN))
        ChangePicture(TURN_LEFT_PICTURES[TurnLeftPic], TURN_LEFT_PIXEL_START, TURN_LEFT_PIXEL_NUM);
      else
        ChangePicture(TURN_LEFT_STOP_PICTURES[TurnLeftPic], TURN_LEFT_PIXEL_START, TURN_LEFT_PIXEL_NUM);

      TurnLeftPic++;
      if (TurnLeftPic == TURN_PICTURE_NUM)
        TurnLeftPic = 0;
    }
  }
  
  else//if (LeftState == TURN_OFF)
  {
    TurnLeftReset();
    LEDSUpdate = true;
  }
}
//--------------------------------------------
void TurnRightCheck()
{
  if(RightState == TURN_ON)
  {     
    //if (SideState)
      //SideReset();

    TurnRightCnt++;

    if (TurnRightCnt == TURN_SHOW_DELAY)
    {      
      TurnRightCnt = 0;
      LEDSUpdate = true;

      TurnRightBlinkCnt++;
      if (TurnRightBlinkCnt == TURN_INDICATOR_BLINK_VALUE)
      {
        TurnRightBlinkCnt = 0;
        
        if (TurnRightBlinkPhase == TURN_INDICATOR_PHASE_OFF)
        {
          TurnRightBlinkPhase = TURN_INDICATOR_PHASE_ON;
          
          for(uint8_t i = 0; i < TURN_RIGHT_INDICATOR_PIXEL_NUM; i++)
            LEDS.setPixelColor(TURN_RIGHT_INDICATOR_START + i, TURN_INDICATOR_COLOR);
        }
        else
        {
          TurnRightBlinkPhase = TURN_INDICATOR_PHASE_OFF;
          
          for(uint8_t i = 0; i < TURN_RIGHT_INDICATOR_PIXEL_NUM; i++)
            LEDS.setPixelColor(TURN_RIGHT_INDICATOR_START + i, 0);
        }
      }

      //if (!digitalRead(STOP_PIN))
      if (digitalRead(STOP_PIN))
        ChangePicture(TURN_RIGHT_PICTURES[TurnRightPic], TURN_RIGHT_PIXEL_START, TURN_RIGHT_PIXEL_NUM);
      else
        ChangePicture(TURN_RIGHT_STOP_PICTURES[TurnRightPic], TURN_RIGHT_PIXEL_START, TURN_RIGHT_PIXEL_NUM);        

      TurnRightPic++;
      if (TurnRightPic == TURN_PICTURE_NUM)
        TurnRightPic = 0;
    }
  }
  
  else//if (RightState == TURN_OFF)
  {
    TurnRightReset();
    LEDSUpdate = true;
  }
}
//-----------------------------------------
void TurnLeftReset()
{
  TurnLeftPic = 0;
  LeftState = TURN_RESET;
  TurnLeftCnt = TURN_SHOW_DELAY - 1;
  SideState = true;
  TurnLeftBlinkCnt = TURN_INDICATOR_BLINK_VALUE - 1;
  TurnLeftBlinkPhase = TURN_INDICATOR_PHASE_OFF;
  
  for(uint8_t i = 0; i < TURN_LEFT_INDICATOR_PIXEL_NUM; i++)
    LEDS.setPixelColor(TURN_LEFT_INDICATOR_START + i, 0);
}
//--------------------------------------------
void TurnRightReset()
{
  TurnRightPic = 0;
  RightState = TURN_RESET;
  TurnRightCnt = TURN_SHOW_DELAY - 1;
  SideState = true;
  TurnRightBlinkCnt = TURN_INDICATOR_BLINK_VALUE - 1;
  TurnRightBlinkPhase = TURN_INDICATOR_PHASE_OFF;
  
  for(uint8_t i = 0; i < TURN_RIGHT_INDICATOR_PIXEL_NUM; i++)
    LEDS.setPixelColor(TURN_RIGHT_INDICATOR_START + i, 0);
}
//--------------------------------------------
void SideReset()
{
  SideState = false;
  SideCnt = SIDE_SHOW_DELAY - 1;
}
//----------------------------------------
void KeyLeft_ISR()
{ 
  if (LeftState == TURN_ON)
    LeftState = TURN_OFF;
  else
  {
    LeftState = TURN_ON;
    TurnRightReset();
  }
}
//----------------------------------------
void KeyRight_ISR()
{ 
  if (RightState == TURN_ON)
    RightState = TURN_OFF;
  else
  {
    RightState = TURN_ON;
    TurnLeftReset();
  }
}

