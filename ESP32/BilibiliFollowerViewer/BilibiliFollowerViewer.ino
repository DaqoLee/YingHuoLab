/* Includes ------------------------------------------------------------------*/
#include "DEV_Config.h"
#include "EPD.h"
#include "GUI_Paint.h"
#include "imagedata.h"

#include "Game.h"
#include "GlowTube.h"
#include "Bilibili.h"
#include "BLE.h"
#include "Clock.h"

#include <Ticker.h>
#include <stdlib.h>

#define BILI  0
#define TIME  1
#define GAME  2
#define TEST  3

// 定时器
Ticker Timer;

int TimerCount = 0;
boolean TimerFlag = true;
int GameDelayFlag=0;

// WiFi 名称与密码
const char *Ssid = "ssid"; //这里填你家中的wifi名 
const char *Password = "password";//这里填你家中的wifi密码 

void setup()
{
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  Serial.begin(115200);
  BLEInit();
  GlowTubeInit();
  WiFi.begin(Ssid, Password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  ClockInit();
  Timer.attach(1, timerCallback); //1s进一次中断
 
  
  getFollower(MyFollowerUrl);
  getWeather(WeatherUrl);
  getClock();
  
  
  DEV_Module_Init();
  EPD_2IN13_V2_Init(EPD_2IN13_V2_FULL);
  EPD_2IN13_V2_Clear();
  DEV_Delay_ms(500);
}


void loop()
{
    
  int numb=0;
  int weaTemp=1;

  UBYTE *BlackImage;
  /* you have to edit the startup_stm32fxxx.s file and set a big enough heap size */
  UWORD Imagesize = ((EPD_2IN13_V2_WIDTH % 8 == 0) ? (EPD_2IN13_V2_WIDTH / 8 ) : (EPD_2IN13_V2_WIDTH / 8 + 1)) * EPD_2IN13_V2_HEIGHT;
  if ((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
    printf("Failed to apply for black memory...\r\n");
    while (1);
  }
  printf("Paint_NewImage\r\n");
  Paint_NewImage(BlackImage, EPD_2IN13_V2_WIDTH, EPD_2IN13_V2_HEIGHT, 180, WHITE);
  Paint_SelectImage(BlackImage);
  Paint_SetMirroring(MIRROR_HORIZONTAL); 
  Paint_Clear(WHITE);
  
#if   1
  Paint_DrawBitMap(gImage_2233); 
  EPD_2IN13_V2_Display(BlackImage);
#endif

  Paint_Clear(WHITE);
  EPD_2IN13_V2_Init(EPD_2IN13_V2_FULL);
  EPD_2IN13_V2_DisplayPartBaseImage(BlackImage);
  EPD_2IN13_V2_Init(EPD_2IN13_V2_PART);
  Paint_Clear(WHITE);
  delay(2000);
  Clock.Flag=1;
  
  while (1)
  {
   
    StatusLoop((uint8_t*)BLEbuf);//BLEbuf BtBuff
    
    EPD_2IN13_V2_DisplayPart(BlackImage);
    if(GameDelayFlag)
    {
       delay(200);
    }

    if (deviceConnected)
    {
      pCharacteristic->setValue((char*)BLEbuf);
      pCharacteristic->notify(); // Send the value to the app!
    }
    if (!deviceConnected && oldDeviceConnected) {
      delay(500); // give the bluetooth stack the chance to get things ready
      pServer->startAdvertising(); // restart advertising
      Serial.println("start advertising");
      oldDeviceConnected = deviceConnected;
    }
    // connecting
    if (deviceConnected && !oldDeviceConnected) {
      // do stuff here on connecting
      oldDeviceConnected = deviceConnected;
    }
 }
 
//  EPD_2IN13_V2_Init(EPD_2IN13_V2_FULL);
//  EPD_2IN13_V2_Clear();
//  DEV_Delay_ms(2000);//Analog clock 1s
//  printf("Goto Sleep...\r\n");
//  EPD_2IN13_V2_Sleep();
//  free(BlackImage);
//  BlackImage = NULL;
//  DEV_Delay_ms(1000);//Analog clock 1s 
}
// 定时器回调函数
void timerCallback()
{
  TimerCount++;
  if (TimerCount >= 60)//一分钟刷新一次
  {
    TimerFlag = true; 
    TimerCount = 0;
  }
  Clock.Second++;
  if(Clock.Second>=60)
  {  
    
    Clock.Second=0;
    Clock.Minute++;
    if( Clock.Minute>=60)
    {
      Clock.Minute=0;
      Clock.Hour++;
      Clock.Flag=1;
      if(Clock.Hour>=24)
      {
        Clock.Hour=0;
      }
    }  
  }
}



void StatusLoop(uint8_t * ModeBuf)
{
  long clockData=1000000+Clock.Hour*10000+Clock.Minute*100+Clock.Second;
  static uint8_t N=0,biliFlag=1,timeFlag=0,gameFlag=0,gameDir=4,gameTemp,testFlag=0;

  if(ModeBuf[0]==0x5A&&ModeBuf[2]==0x5A)
  {
    N=ModeBuf[1];
  }
  else if(ModeBuf[0]==0x55&&ModeBuf[2]==0x55)
  {
    gameDir=ModeBuf[1];
  }
  else if(ModeBuf[0]==0x3A&&ModeBuf[2]==0x3A)
  {
    gameTemp=ModeBuf[1];
  } 
  switch(N)
  {
    case BILI:
      if(biliFlag)
      {
        biliFlag=0;
        timeFlag=1;
        gameFlag=1;
        testFlag=1;
        if(GameDelayFlag)
        {
          GAME_SnakeFreeNode(&SnakeHand);
        }
        GameDelayFlag=0;
        Paint_Clear(WHITE);
        getFollower(MyFollowerUrl);
        getWeather(WeatherUrl);
        WeatherDisplay(WeatherCode, Temperature);// weaTemp 
        DateDisplay();
        Paint_DrawBitMap_1(0,40,40,120,gImage_bili); 
        Paint_DrawString_CN(0, 220,"萤 火 实 验 室" , &Font12CN, BLACK, WHITE);
      }
     if(TimerFlag)
     {  
        TimerFlag = false;
        if(!getFollower(MyFollowerUrl))
        {
//          WiFi.reconnect();
//          while (WiFi.status() != WL_CONNECTED)
//          {
//            delay(500);
//            Serial.print("\r\nreconnect\r\n");
//          }  
          getClock();       
        }

        if(Clock.Flag)
        {
          getClock();
          getWeather(WeatherUrl);
          WeatherDisplay(WeatherCode, Temperature);// weaTemp
          DateDisplay();
          Clock.Flag=0;
        }      
     }
    ClockDisplay();    
    GlowTubeDisplay(Follower,ModeBuf);
    break;
    
    case TIME: 
      if(timeFlag)
      {
        biliFlag=1;
        timeFlag=0;
        gameFlag=1;
        testFlag=1;
         if(GameDelayFlag)
        {
          GAME_SnakeFreeNode(&SnakeHand);
        }
        GameDelayFlag=0;
        Paint_Clear(WHITE);
        getFollower(MyFollowerUrl);
        getWeather(WeatherUrl);
        WeatherDisplay(WeatherCode, Temperature);// weaTemp
        DateDisplay();
        FollowerDisplay();
        Paint_DrawBitMap_1(0,40,40,120,gImage_bili);  
        Paint_DrawString_CN(0, 220,"萤 火 实 验 室" , &Font12CN, BLACK, WHITE);
      }
     if(TimerFlag)
     {  
        TimerFlag = false;
        if(!getFollower(MyFollowerUrl))
        {
          WiFi.reconnect();
          while (WiFi.status() != WL_CONNECTED)
          {
            delay(500);
            Serial.print("\r\nreconnect\r\n");
          }         
        }
        FollowerDisplay();
        if(Clock.Flag)
        {
          getClock();
          getWeather(WeatherUrl);
          WeatherDisplay(WeatherCode, Temperature);// weaTemp
          DateDisplay();
          Clock.Flag=0;
        }      
     } 
     GlowTubeDisplay(clockData,ModeBuf);    
    break;    
    
    case TEST:
     if(testFlag)
     {
       biliFlag=1;
       timeFlag=1;
       gameFlag=1;
       testFlag=0;
       if(GameDelayFlag)
       {
         GAME_SnakeFreeNode(&SnakeHand);
       }
       GameDelayFlag=0;
       Paint_Clear(WHITE);
     }
     GlowTubeTest(0);

    break;
    case GAME:
      if(gameFlag)
      {
        biliFlag=1;
        timeFlag=1;
        gameFlag=0;
        testFlag=1;
        GameDelayFlag=1;
        Paint_Clear(WHITE);
        GAME_SnakeInit(&SnakeHand,GameMap);
        Paint_DrawBitMap_1(0,40,40,120,gImage_bili);  
        Paint_ClearWindows(0, 220, 0 + Font12CN.Width * 7, 220 + Font12CN.Height, WHITE);
        Paint_DrawString_CN(0, 220,"萤 火 实 验 室" , &Font12CN, BLACK, WHITE);
      }

      if(!GAME_SnakeMove(GameMap,&SnakeHand,gameDir))
      {
        N=15;
      }
      
    break;
    
    default:
       if(timeFlag)
      {
        biliFlag=1;
        timeFlag=0;
        gameFlag=1;
        testFlag=1;
     if(GameDelayFlag)
     {
       GAME_SnakeFreeNode(&SnakeHand);
       gameFlag=1;
     }
     }
     GameDelayFlag=1;
     N=GAME_Over(0);
    break;
  }  
  
}



void WeatherDisplay(int weaCode, int temp)
{
    Paint_ClearWindows(3, 52, 3 + Font20.Width * 2, 52 + Font20.Height, WHITE);
    Paint_DrawNum(3, 52, temp, &Font20, BLACK, WHITE);
   switch(weaCode)
  {
    case 1:
      Paint_ClearWindows(0, 5, 0 + Font24CN.Width, 5 + Font24CN.Height, WHITE);
      Paint_DrawString_CN(0, 5,"晴", &Font24CN, BLACK, WHITE);
      Paint_DrawBitMap_1(42,170,80,80,gImage_01);                         //  Update strip to match
      break;
    case 4:
      Paint_ClearWindows(0, 5, 0 + Font24CN.Width, 5 + Font24CN.Height, WHITE);
      Paint_DrawString_CN(0, 5,"云", &Font24CN, BLACK, WHITE);
      Paint_DrawBitMap_1(42,170,80,80,gImage_04);                         //  Update strip to match
      break;
    case 9:
      Paint_ClearWindows(0, 5, 0 + Font24CN.Width, 5 + Font24CN.Height, WHITE);
      Paint_DrawString_CN(0, 5,"阴", &Font24CN, BLACK, WHITE);
      Paint_DrawBitMap_1(42,170,80,80,gImage_09);                        //  Update strip to match
      break;
    case 10:
      Paint_ClearWindows(0, 5, 0 + Font24CN.Width, 5 + Font24CN.Height, WHITE);
      Paint_DrawString_CN(0, 5,"雨", &Font24CN, BLACK, WHITE);
      Paint_DrawBitMap_1(42,170,80,80,gImage_10);                       //  Update strip to match
      break;
    case 21:
      Paint_ClearWindows(0, 5, 0 + Font24CN.Width, 5 + Font24CN.Height, WHITE);
      Paint_DrawString_CN(0, 5,"雪", &Font24CN, BLACK, WHITE);
      Paint_DrawBitMap_1(42,170,80,80,gImage_21);                       //  Update strip to match
      break;
    case 30:
      Paint_ClearWindows(0, 5, 0 + Font24CN.Width, 5 + Font24CN.Height, WHITE);
      Paint_DrawString_CN(0, 5,"雾", &Font24CN, BLACK, WHITE);
      Paint_DrawBitMap_1(42,170,80,80,gImage_30);                       //  Update strip to match
      break;
    default:
      Paint_ClearWindows(0, 5, 0 + Font24CN.Width, 5 + Font24CN.Height, WHITE);
      Paint_DrawString_CN(0, 5,"雨", &Font24CN, BLACK, WHITE);
      Paint_DrawBitMap_1(42,170,80,80,gImage_10);
    break;
  }    
}


void getClock(void)
{
  now();

  Clock.Hour=hour();
  Clock.Minute=minute();
  Clock.Second=second();
}



void ClockDisplay()
{
  char timeBuf[10];
    
  sprintf(timeBuf, "%02d %02d", Clock.Hour, Clock.Minute);
  Paint_ClearWindows(1, 135, 1 + Font24_d.Width * 5+2, 135 + Font24_d.Height, WHITE);
  Paint_DrawString_EN(1, 135, timeBuf, &Font24_d, BLACK, WHITE);
  sprintf(timeBuf, "%02d", Clock.Second);
  Paint_DrawString_EN(53, 150, timeBuf, &Font12, BLACK, WHITE);
}

void DateDisplay(void)
{
  char dateBuf[5];
  Paint_ClearWindows(0, 90, 0 + Font20.Width*4, 90 + Font20.Height, WHITE);
  Paint_DrawNum(0, 90,year(), &Font20, BLACK, WHITE);
  sprintf(dateBuf, "%02d%02d", month(), day());
  Paint_ClearWindows(0, 110, 0 + Font20.Width*4, 110 + Font20.Height, WHITE);
  Paint_DrawString_EN(0, 110, dateBuf, &Font20, BLACK, WHITE);
  Paint_ClearWindows(80, 85, 80 + Font24CN.Width, 85 + Font24CN.Height, WHITE);
  Paint_DrawString_CN(80, 85,num_week(weekday(),4), &Font24CN, BLACK, WHITE);
}

void FollowerDisplay(void)
{
    static char flowBuf[7];
    sprintf(flowBuf, "%07d",Follower );
    Paint_ClearWindows(5, 140, 5 + Font16_d.Width * 7, 140 + Font16_d.Height, WHITE);
    Paint_DrawString_EN(5, 140, flowBuf, &Font16_d, BLACK, WHITE);
}


/*
@功能:判断星期并赋值
*/
char week1[10],week2[10],week3[2],week4[4];

char *num_week(uint8_t dayofweek,int Mode){
  switch(dayofweek)
  {
    case 1: 
    strcpy(week1,"Sunday");
    strcpy(week2,"周日");
    strcpy(week3,"Su");
    strcpy(week4,"日"); 
      break;
    case 2: 
    strcpy(week1,"Monday");
    strcpy(week2,"周一");
    strcpy(week3,"Mo");
    strcpy(week4,"一"); 
      break;
    case 3: 
    strcpy(week1,"Tuesday");
    strcpy(week2,"周二");
    strcpy(week3,"Tu");
    strcpy(week4,"二"); 
      break;
    case 4: 
    strcpy(week1,"Wednesday");
    strcpy(week2,"周三"); 
    strcpy(week3,"We");
    strcpy(week4,"三");  
      break;
    case 5: 
    strcpy(week1,"Thursday");
    strcpy(week2,"周四"); 
    strcpy(week3,"Th");
    strcpy(week4,"四"); 
      break;
    case 6: 
    strcpy(week1,"Friday");
    strcpy(week2,"周五");
    strcpy(week3,"Fr"); 
    strcpy(week4,"五");
      break;
    case 7: 
    strcpy(week1,"Saturday");
    strcpy(week2,"周六"); 
    strcpy(week3,"Sa");
    strcpy(week4,"六");
      break;
    default:
    strcpy(week1,"NO");
    strcpy(week2,"无");
    strcpy(week3,"NO");
    strcpy(week4,"无");
      break; 
  }
  switch(Mode)
  {
    case 1: return week1; break;
    case 2: return week2; break;
    case 3: return week3; break;
    case 4: return week4; break;
    default:
    break;
  }
}
 
