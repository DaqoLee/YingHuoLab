#ifndef __GLOW_TUBE_H
#define __GLOW_TUBE_H


#include "Arduino.h"
#include "esp32-hal.h"
#include <Adafruit_NeoPixel.h>

#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif


#define LED1_PIN    17
#define LED2_PIN    18
#define LED3_PIN    19
#define LED4_PIN    21
#define LED5_PIN    22
#define LED6_PIN    23

#define NR_OF_LEDS   30
#define NR_OF_ALL_BITS 24*NR_OF_LEDS

extern rmt_data_t led_data[NR_OF_ALL_BITS];

extern rmt_obj_t* rmt_send_1;
extern rmt_obj_t* rmt_send_2;
extern rmt_obj_t* rmt_send_3;
extern rmt_obj_t* rmt_send_4;
extern rmt_obj_t* rmt_send_5;
extern rmt_obj_t* rmt_send_6;


void GlowTubeDisplay(int Data , uint8_t * ModeBuf);
void rainbow(int data, int wait);
void GlowTubeTest(int testNum);
void GlowTubeInit(void);
void colWrite(int Num,int Data,int R,int G,int B);
#endif
