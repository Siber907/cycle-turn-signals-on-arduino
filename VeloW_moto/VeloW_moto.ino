
#include <Adafruit_NeoPixel.h>
#include "VeloW_moto.h"
#include <PinChangeInt.h>


Adafruit_NeoPixel LEDS = Adafruit_NeoPixel(PIXEL_NUM, LED_PIN, NEO_GRBW + NEO_KHZ800);


const uint32_t *TURN_LEFT_PICTURES[] = { Picture_1, Picture_2, Picture_3, Picture_4, Picture_5, Picture_6, Picture_7, Picture_8, Picture_9, Picture_10, Picture_11, Picture_12, Picture_13, 
                                         Picture_14, Picture_15, Picture_16, Picture_17, Picture_18, Picture_19, Picture_20, Picture_21, Picture_22, Picture_23, Picture_24, Picture_25, Picture_26,
                                         Picture_27, Picture_28, Picture_29, Picture_30, Picture_31, Picture_32, Picture_33, Picture_34, Picture_35, Picture_36, Picture_37, Picture_38, Picture_39,
                                         Picture_40, Picture_41, Picture_42, Picture_43, Picture_44, Picture_45, Picture_46, Picture_47, Picture_48, Picture_49, Picture_50, Picture_51, Picture_52,
                                         Picture_53, Picture_54, Picture_55, Picture_56, Picture_57, Picture_58, Picture_59, Picture_60, Picture_61, Picture_62, Picture_63, Picture_64, Picture_65,
                                         Picture_66, Picture_67, Picture_68, Picture_69, Picture_70, Picture_71, Picture_72, Picture_73, Picture_74, Picture_75, Picture_76, Picture_77, Picture_78,
                                         Picture_79, Picture_80, Picture_81, Picture_82, Picture_83, Picture_84, Picture_85, Picture_86, Picture_87, Picture_88, Picture_89, Picture_90, Picture_91,
                                         Picture_92, Picture_93, Picture_94, Picture_94, Picture_94, Picture_94, Picture_94, Picture_95
                                          };

const uint32_t *TURN_LEFT_STOP_PICTURES[] = { Picture_96, Picture_97, Picture_98, Picture_99, Picture_100, Picture_101, Picture_102, Picture_103, Picture_104, Picture_105, Picture_106, Picture_107, Picture_108, 
                                         Picture_109, Picture_110, Picture_111, Picture_112, Picture_113, Picture_114, Picture_115, Picture_116, Picture_117, Picture_118, Picture_119, Picture_120, Picture_121,
                                         Picture_122, Picture_123, Picture_124, Picture_125, Picture_126, Picture_127, Picture_128, Picture_129, Picture_130, Picture_131, Picture_132, Picture_133, Picture_134,
                                         Picture_135, Picture_136, Picture_137, Picture_138, Picture_139, Picture_140, Picture_141, Picture_142, Picture_143, Picture_144, Picture_145, Picture_146, Picture_147,
                                         Picture_148, Picture_149, Picture_150, Picture_151, Picture_152, Picture_153, Picture_154, Picture_155, Picture_156, Picture_157, Picture_158, Picture_159, Picture_160,
                                         Picture_161, Picture_162, Picture_163, Picture_164, Picture_165, Picture_166, Picture_167, Picture_168, Picture_169, Picture_170, Picture_171, Picture_172, Picture_173,
                                         Picture_174, Picture_175, Picture_176, Picture_177, Picture_178, Picture_179, Picture_180, Picture_181, Picture_182, Picture_183, Picture_184, Picture_185, Picture_186,
                                         Picture_187, Picture_188, Picture_189, Picture_190,}; // Picture_191, Picture_192, Picture_193, Picture_194
                                         

const uint32_t *TURN_RIGHT_PICTURES[] = { Picture_1, Picture_2, Picture_3, Picture_4, Picture_5, Picture_6, Picture_7, Picture_8, Picture_9, Picture_10, Picture_11, Picture_12, Picture_13, 
                                         Picture_14, Picture_15, Picture_16, Picture_17, Picture_18, Picture_19, Picture_20, Picture_21, Picture_22, Picture_23, Picture_24, Picture_25, Picture_26,
                                         Picture_27, Picture_28, Picture_29, Picture_30, Picture_31, Picture_32, Picture_33, Picture_34, Picture_35, Picture_36, Picture_37, Picture_38, Picture_39,
                                         Picture_40, Picture_41, Picture_42, Picture_43, Picture_44, Picture_45, Picture_46, Picture_47, Picture_48, Picture_49, Picture_50, Picture_51, Picture_52,
                                         Picture_53, Picture_54, Picture_55, Picture_56, Picture_57, Picture_58, Picture_59, Picture_60, Picture_61, Picture_62, Picture_63, Picture_64, Picture_65,
                                         Picture_66, Picture_67, Picture_68, Picture_69, Picture_70, Picture_71, Picture_72, Picture_73, Picture_74, Picture_75, Picture_76, Picture_77, Picture_78,
                                         Picture_79, Picture_80, Picture_81, Picture_82, Picture_83, Picture_84, Picture_85, Picture_86, Picture_87, Picture_88, Picture_89, Picture_90, Picture_91,
                                         Picture_92, Picture_93, Picture_94, Picture_94, Picture_94, Picture_94, Picture_94, Picture_95
                                         };

const uint32_t *TURN_RIGHT_STOP_PICTURES[] = { Picture_96, Picture_97, Picture_98, Picture_99, Picture_100, Picture_101, Picture_102, Picture_103, Picture_104, Picture_105, Picture_106, Picture_107, Picture_108, 
                                         Picture_109, Picture_110, Picture_111, Picture_112, Picture_113, Picture_114, Picture_115, Picture_116, Picture_117, Picture_118, Picture_119, Picture_120, Picture_121,
                                         Picture_122, Picture_123, Picture_124, Picture_125, Picture_126, Picture_127, Picture_128, Picture_129, Picture_130, Picture_131, Picture_132, Picture_133, Picture_134,
                                         Picture_135, Picture_136, Picture_137, Picture_138, Picture_139, Picture_140, Picture_141, Picture_142, Picture_143, Picture_144, Picture_145, Picture_146, Picture_147,
                                         Picture_148, Picture_149, Picture_150, Picture_151, Picture_152, Picture_153, Picture_154, Picture_155, Picture_156, Picture_157, Picture_158, Picture_159, Picture_160,
                                         Picture_161, Picture_162, Picture_163, Picture_164, Picture_165, Picture_166, Picture_167, Picture_168, Picture_169, Picture_170, Picture_171, Picture_172, Picture_173,
                                         Picture_174, Picture_175, Picture_176, Picture_177, Picture_178, Picture_179, Picture_180, Picture_181, Picture_182, Picture_183, Picture_184, Picture_185, Picture_186,
                                         Picture_187, Picture_188, Picture_189, Picture_190,}; // Picture_191, Picture_192, Picture_193, Picture_194
                                         
const uint32_t *STOP_PICTURES[] = { Picture_191}; // , Picture_192};

const uint32_t *SIDE_PICTURES[] = { Picture_193};



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
  
  if (LeftState == TURN_OFF)
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
  
  if (RightState == TURN_OFF)
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
    //ChangePicture(SIDE_PICTURES[0], SIDE_PIXEL_START, SIDE_PIXEL_NUM);
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
    //ChangePicture(SIDE_PICTURES[0], SIDE_PIXEL_START, SIDE_PIXEL_NUM);
  }
}

