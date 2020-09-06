#include "GlowTube.h"

rmt_data_t led_data[NR_OF_ALL_BITS];

rmt_obj_t* rmt_send_1 = NULL;
rmt_obj_t* rmt_send_2 = NULL;
rmt_obj_t* rmt_send_3 = NULL;
rmt_obj_t* rmt_send_4 = NULL;
rmt_obj_t* rmt_send_5 = NULL;
rmt_obj_t* rmt_send_6 = NULL;

Adafruit_NeoPixel strip(10, 16, NEO_GRB + NEO_KHZ800);



void GlowTubeInit(void)
{

  rmt_send_1 = rmtInit(LED1_PIN, true, RMT_MEM_64);
  rmt_send_2 = rmtInit(LED2_PIN, true, RMT_MEM_64);
  rmt_send_3 = rmtInit(LED3_PIN, true, RMT_MEM_64);
  rmt_send_4 = rmtInit(LED4_PIN, true, RMT_MEM_64);
  rmt_send_5 = rmtInit(LED5_PIN, true, RMT_MEM_64);
  rmt_send_6 = rmtInit(LED6_PIN, true, RMT_MEM_64);

  rmtSetTick(rmt_send_1, 100);
  rmtSetTick(rmt_send_2, 100);
  rmtSetTick(rmt_send_3, 100);
  rmtSetTick(rmt_send_4, 100);
  rmtSetTick(rmt_send_5, 100);
  rmtSetTick(rmt_send_6, 100);

}

void GlowTubeDisplay(int Data , uint8_t * ModeBuf)
{
    char dataBuf[7];
    
    static uint8_t red[8]  ={255,255,255,255,0  ,0  ,0   ,0}; 
    static uint8_t blue[8] ={0  ,0  ,255,0  ,255,255,0   ,0};
    static uint8_t green[8]={0  ,0  ,0  ,255,0  ,255,255 ,0};

    static uint8_t intensity = 4,mode = 1;
    static float temp = 1.0f;

    if(ModeBuf[0] == 0xFF && ModeBuf[2] == 0xFF)
    {
		intensity = ModeBuf[1];
    }
    else if(ModeBuf[0] == 0xAA && ModeBuf[6] == 0xAA)
    {
		mode = ModeBuf[1];
	  if (mode == 1 || mode == 2)
	  {
		  red[ModeBuf[2]] = ModeBuf[3];
		  blue[ModeBuf[2]] = ModeBuf[4];
		  green[ModeBuf[2]] = ModeBuf[5];
	  }
      
    }

    sprintf(dataBuf, "%07d",Data);
    temp=intensity*0.1f;
//    switch(intensity)
//    {
//      case 1: 
//      temp = temp >= 20 ? 20 : temp + 0.2f;
//      break;
//      case 2: 
//      temp = temp <= 1 ? 1 : temp - 0.2f;
//      break;
//      default:
//      break;
//    }

    switch(mode)
    {
      case 1: //整体显示同一个颜色
      for(int i=1;i<=6;i++)
      {
         colWrite(i,  dataBuf[i]-'0',red[0]/temp ,green[0]/temp ,blue[0]/temp);
         delay(2);
      }
      break;

      case 2: //不同管子不同颜色
      for(int i=1;i<=6;i++)
      {
         colWrite(i,  dataBuf[i]-'0',red[i]/temp ,green[i]/temp ,blue[i]/temp);
         delay(2);
      }
      break;
      
      case 3:// 整体光谱循环
      rainbow(Data,1);
      break;
      case 4://全灭 
      for(int i=1;i<=6;i++)
      {
         colWrite(i,  dataBuf[i]-'0',0 ,0 ,0);
         delay(2);
      }
      break;
      default:
      break;
    }
}

void rainbow(int data, int wait)
{
	static uint32_t color = 0xFF0000;
	char dataBuf[7];
	static long firstPixelHue = 0;
	
	sprintf(dataBuf, "%07d", data);
	if (firstPixelHue < 65536)
	{
		int pixelHue = firstPixelHue + (65536L);
		color = strip.gamma32(strip.ColorHSV(pixelHue));
		firstPixelHue += 256;

		for (int i = 1; i <=6; i++)
		{
			colWrite(i, dataBuf[i] - '0', (color & 0xFF0000) >> 16, (color & 0xFF00) >> 8, color & 0xFF);
			delay(1);

		}
		delay(wait);

	}
	else
	{
		firstPixelHue = 0;
	}

}


void GlowTubeTest(int testNum)
{

#if  0
     for(int temp=0;temp<10;temp++)
     {
       colWrite(1, temp ,255 ,0 ,0);
       delay(500);
     }
    for(int i=2;i<6;i++)
    {    
      switch(i)
      {
        case 1: 
           for(int j=0;j<10;j++)
           {
             colWrite(1, j ,255 ,0 ,0);
             delay(100);
           }
           break;
        case 2:
           for(int j=0;j<10;j++)
           {
             colWrite(1, j ,255 ,255 ,0);
             delay(100);
           }
           break;
        case 3:
           for(int j=0;j<10;j++)
           {
             colWrite(1, j ,255 ,0 ,255);
             delay(100);
           } 
            break;
        case 4:
           for(int j=0;j<10;j++)
           {
             colWrite(1, j ,0 ,255 ,0);
             delay(100);
           }
            break;
        case 5:
           for(int j=0;j<10;j++)
           {
             colWrite(1, j ,0 ,255 ,255);
             delay(100);
           }
           break;
        case 6:
           for(int j=0;j<10;j++)
           {
             colWrite(1, j ,0 ,0 ,255);
             delay(100);
           }                  
          break;
        default:
        break;
      }   
    }

   colWrite(1, 31 ,0 ,0 ,255);
//     for(int temp=0;temp<10;temp++)
//     {
//       colWrite(1, temp ,255 ,0 ,0);
//       delay(500);
//     }
     delay(2000);
     for(int temp=21;temp<=30;temp++)
     {
       colWrite(1, temp ,255 ,0 ,0);
       delay(500);
     }

      delay(8000);
#endif

#if  1 //杀马特爆闪
    for(int i=1;i<=4;i++)
    {
      for(int j=0;j<11;j++)
      {
        colWrite(1,  j,255 ,0 ,0);
        delay(2);
        colWrite(2,  j,255 ,255 ,0);
        delay(2);
        colWrite(3,  j,0 ,255 ,0);
        delay(2);
        colWrite(4,  j,0 ,255 ,255);
        delay(2);
        colWrite(5,  j,0 ,0 ,255);
        delay(2);
        colWrite(6,  j,255 ,0 ,255);
        delay(2); 
        delay(100);
      }  
    } 


#if  1 //呼吸效果
    for(int i=1, n=7;i<=4;i++)
    {
      if(i%2)
      {
        for(int j=0;j<=255;j+=3)
        {
          colWrite(1,  n,j ,0 ,0);
          delay(2);
          colWrite(2,  n,j ,j ,0);
          delay(2);
          colWrite(3,  n,0 ,j ,0);
          delay(2);
          colWrite(4,  n,0 ,j ,j);
          delay(2);
          colWrite(5,  n,0 ,0 ,j);
          delay(2);
          colWrite(6,  n,j ,0 ,j);
          delay(2); 
          delay(10);
        }   
      }
      else
      {
        for(int j=255;j>=0;j-=3)
        {
          colWrite(1,  n,j ,0 ,0);
          delay(2);
          colWrite(2,  n,j ,j ,0);
          delay(2);
          colWrite(3,  n,0 ,j ,0);
          delay(2);
          colWrite(4,  n,0 ,j ,j);
          delay(2);
          colWrite(5,  n,0 ,0 ,j);
          delay(2);
          colWrite(6,  n,j ,0 ,j);
          delay(2); 
          delay(10);
        }          
      }
    } 

#endif


#if  1 //轮流点亮
    for(int i=6;i>0;i--)
    {
      for(int j=0;j<10;j++)
      {
//        colWrite(i,  j,255 ,0 ,0);
//        delay(50); 

        switch(i)
        {
          case 1:
             colWrite(1,  j,255 ,0 ,0);   
           delay(2); 
          case 2:
             colWrite(2,  j,255 ,0 ,0);   
           delay(2); 
          case 3:
             colWrite(3,  j,255 ,0 ,0);   
           delay(2); 
          case 4:
             colWrite(4,  j,255 ,0 ,0);   
           delay(2); 
          case 5:
             colWrite(5,  j,255 ,0 ,0);     
            delay(2); 
          case 6:
             colWrite(6,  j,255 ,0 ,0);                    
            break;
          default:
          break;
        } 

        delay(50);
      }  
    } 
  //逐个淡化熄灭
    for(int n=4,i=1;i<n;i++)
    {
      if(i%2)
      {
        if(i!=n-1)
        {
          for(int j=255;j>=0;j-=3)
          {
            colWrite(1,  9,j ,0 ,0);
            delay(2);
            colWrite(2,  9,j ,0 ,0);
            delay(2);
            colWrite(3,  9,j ,0 ,0);
            delay(2);
            colWrite(4,  9,j ,0 ,0);
            delay(2);
            colWrite(5,  9,j ,0 ,0);
            delay(2);
            colWrite(6,  9,j ,0 ,0);
            delay(2); 
            delay(10);
          }  
       }
       else 
       {
          for(int i=1;i<7;i++)
          {
            for(int j=255;j>=0;j-=3)
            {
              colWrite(i,  9,j ,0 ,0);
              delay(10);
            }           
          } 
//          colWrite(6,  0,255 ,0 ,0);
//          delay(10);
       }
      }
      else 
      {
        for(int j=0;j<=255;j+=3)
        {
          colWrite(1,  9,j ,0 ,0);
          delay(2);
          colWrite(2,  9,j ,0 ,0);
          delay(2);
          colWrite(3,  9,j ,0 ,0);
          delay(2);
          colWrite(4,  9,j ,0 ,0);
          delay(2);
          colWrite(5,  9,j ,0 ,0);
          delay(2);
          colWrite(6,  9,j ,0 ,0);
          delay(2); 
          delay(10);
        }          
      }
    } 
#endif    
#endif     
#if  0    //传参控制
    colWrite(1,  testNum,255 ,0 ,0);
    delay(10);
    colWrite(2,  testNum,255 ,255 ,0);
    delay(10);
    colWrite(3,  testNum,0 ,255 ,0);
    delay(10);
    colWrite(4,  testNum,0 ,255 ,255);
    delay(10);
    colWrite(5,  testNum,0 ,0 ,255);
    delay(10);
    colWrite(6,  testNum,255 ,0 ,255);
    delay(10);
#endif 
}

void colWrite(int Num,int Data,int R,int G,int B)
{
    int led, col, bit;
    int i=0,j=0;

    for (led=0; led<NR_OF_LEDS; led++) 
    {
        for (j=0; j<3; j++ ) 
        {
          if(j==0)
          {
            col=G;
          }
          else if(j==1)
          {
            col=R;
          }
          else 
          {
            col=B;
          }
              
          for (bit=0; bit<8; bit++)
          {
              if ( (col & (1<<(8-bit))) && ((led == Data)||led == Data+10) ){
                  led_data[i].level0 = 1;
                  led_data[i].duration0 = 8;
                  led_data[i].level1 = 0;
                  led_data[i].duration1 = 4;
              } else {
                  led_data[i].level0 = 1;
                  led_data[i].duration0 = 4;
                  led_data[i].level1 = 0;
                  led_data[i].duration1 = 8;
              }
              i++;
          }
        }
    }

  switch(Num)
  {
    case 1:
      rmtWrite(rmt_send_1, led_data, NR_OF_ALL_BITS);                      //  Update strip to match
      break;
    case 2:
      rmtWrite(rmt_send_2, led_data, NR_OF_ALL_BITS);                         //  Update strip to match
      break;
    case 3:
      rmtWrite(rmt_send_3, led_data, NR_OF_ALL_BITS);                          //  Update strip to match
      break;
    case 4:
      rmtWrite(rmt_send_4, led_data, NR_OF_ALL_BITS);                         //  Update strip to match
      break;
    case 5:
      rmtWrite(rmt_send_5, led_data, NR_OF_ALL_BITS);                      //  Update strip to match
      break;
    case 6:
      rmtWrite(rmt_send_6, led_data, NR_OF_ALL_BITS);                    //  Update strip to match
      break;
    default:
    break;
  }
}
