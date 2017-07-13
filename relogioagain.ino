/*  StarSimulation was written to utilize any 128x64 display. I was inspired by past versions of 
 *  MicroSoft Windows screen savers of the Starfield Simulation. Please feel free to modify and share
 *  this code for any 128x64 LCD or OLED. StarSimulation sketch was written for use with I2C SH1106.
 *  This code must be modified to work with other display devices. This code was written with (50) stars
 *  and may use upto 200 stars with an UNO R3.
 *  
 *  This code was written entirely by Greg Stievenart with no claim to or any images or information 
 *  provided in this code. Freely published June 21, 2016.
 */
#include <LowPower.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <Wire.h>                     // requried to run I2C SH1106
#include <SPI.h>                      // requried to run I2C SH1106
#include <Adafruit_GFX.h>             // https://github.com/adafruit/Adafruit-GFX-Library
#include <Adafruit_SH1106.h>          // https://github.com/wonho-maker/Adafruit_SH1106
#include <DS1307.h>

#define OLED_RESET 4                  // reset required for SH1106

Adafruit_SH1106 display(OLED_RESET);  // reset required for SH1106

#define navegar 6
#define selecionar 3
#define mais A0
#define menos A3
#define lanterna 5
#define buzer 9

const unsigned char battIconEmpty [] PROGMEM = {
0xFF, 0xFE, 0x80, 0x02, 0xA0, 0x03, 0xA0, 0x03, 0xA0, 0x03, 0xA0, 0x03, 0x80, 0x02, 0xFF, 0xFE
};

const unsigned char battIconLow []  PROGMEM = {
0xFF, 0xFE, 0x80, 0x02, 0xBC, 0x03, 0xBC, 0x03, 0xBC, 0x03, 0xBC, 0x03, 0x80, 0x02, 0xFF, 0xFE
};

const unsigned char battIconHigh []PROGMEM = {
0xFF, 0xFE, 0x80, 0x02, 0xBF, 0xC3, 0xBF, 0xC3, 0xBF, 0xC3, 0xBF, 0xC3, 0x80, 0x02, 0xFF, 0xFE
};

const unsigned char battIconFull [] PROGMEM = {
0xFF, 0xFE, 0x80, 0x02, 0xBF, 0xFB, 0xBF, 0xFB, 0xBF, 0xFB, 0xBF, 0xFB, 0x80, 0x02, 0xFF, 0xFE
};

const byte torch1[] PROGMEM ={
0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,
0x00,0x00,0x07,0xE0,
0x00,0x00,0x1F,0xE0,
0x3F,0xFF,0xFF,0xE0,
0x7F,0xFF,0xFF,0xE0,
0xFF,0xFF,0xFF,0xE0,
0xFF,0xFF,0xFF,0xE0,
0xFF,0xFF,0xFF,0xE0,
0xFF,0xFF,0xFF,0xE0,
0x7F,0xFF,0xFF,0xE0,
0x3F,0xFF,0xFF,0xE0,
0x00,0x00,0x1F,0xE0,
0x00,0x00,0x07,0xE0,
0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,
};

const byte torch[] PROGMEM ={
0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,
0x00,0x00,0x07,0xE0,
0x00,0x00,0x1F,0xE3,
0x3F,0xFF,0xFF,0xEE,
0x7F,0xFF,0xFF,0xE8,
0xFF,0xFF,0xFF,0xE0,
0xFF,0xFF,0xFF,0xEF,
0xFF,0xFF,0xFF,0xEF,
0xFF,0xFF,0xFF,0xE0,
0x7F,0xFF,0xFF,0xE8,
0x3F,0xFF,0xFF,0xEE,
0x00,0x00,0x1F,0xE3,
0x00,0x00,0x07,0xE0,
0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,
};

const byte selectbar_top[] PROGMEM ={
0xFF,0xFF,0xFF,0xFF,0xF0,
0x80,0x00,0x00,0x00,0x10,
0x80,0x00,0x00,0x00,0x10,
0x80,0x00,0x00,0x00,0x10,
0x80,0x00,0x00,0x00,0x10,
0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,
};

const byte selectbar_bottom[] PROGMEM ={
0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,
0x80,0x00,0x00,0x00,0x10,
0x80,0x00,0x00,0x00,0x10,
0x80,0x00,0x00,0x00,0x10,
0x80,0x00,0x00,0x00,0x10,
0xFF,0xFF,0xFF,0xFF,0xF0,
};

const byte menu_settings[] PROGMEM ={
0x00,0x00,0x00,0x00,
0x00,0x03,0xC0,0x00,
0x00,0x03,0xC0,0x00,
0x00,0x03,0xC0,0x00,
0x02,0x03,0xC0,0x40,
0x07,0x87,0xE1,0xE0,
0x0F,0xDF,0xFB,0xF0,
0x07,0xFF,0xFF,0xE0,
0x07,0xF8,0x1F,0xE0,
0x03,0xE0,0x07,0xC0,
0x01,0xC7,0xE3,0x80,
0x03,0x8F,0xF1,0xC0,
0x03,0x9F,0xF9,0xC0,
0x07,0x3E,0x7C,0xE0,
0x7F,0x3C,0x3C,0xFE,
0x7F,0x38,0x1C,0xFE,
0x7F,0x38,0x1C,0xFE,
0x7F,0x3C,0x3C,0xFE,
0x07,0x3E,0x7C,0xE0,
0x03,0x9F,0xF9,0xC0,
0x03,0x8F,0xF1,0xC0,
0x01,0xC7,0xE3,0x80,
0x03,0xE0,0x07,0xC0,
0x07,0xF8,0x1F,0xE0,
0x07,0xFF,0xFF,0xE0,
0x0F,0xDF,0xFB,0xF0,
0x07,0x87,0xE1,0xE0,
0x02,0x03,0xC0,0x40,
0x00,0x03,0xC0,0x00,
0x00,0x03,0xC0,0x00,
0x00,0x03,0xC0,0x00,
0x00,0x00,0x00,0x00,
};

const byte menu_exit[] PROGMEM ={
0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,
0x3F,0xFF,0xF8,0x00,
0x7F,0xFF,0xFC,0x00,
0x70,0x00,0x1C,0x00,
0x60,0x00,0x0C,0x00,
0x60,0x00,0x0C,0xC0,
0x60,0x00,0x0C,0xE0,
0x60,0x00,0x00,0x70,
0x60,0x00,0x00,0x38,
0x60,0x00,0x00,0x1C,
0x60,0x00,0xFF,0xFE,
0x60,0x00,0xFF,0xFE,
0x60,0x00,0x00,0x1C,
0x60,0x00,0x00,0x38,
0x60,0x00,0x00,0x70,
0x60,0x00,0x0C,0xE0,
0x60,0x00,0x0C,0xC0,
0x60,0x00,0x0C,0x00,
0x70,0x00,0x1C,0x00,
0x7F,0xFF,0xFC,0x00,
0x3F,0xFF,0xF8,0x00,
0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,
};

const byte menu_diagnostic[] PROGMEM ={
0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,
0x00,0x10,0x00,0x00,
0x00,0x18,0x00,0x00,
0x00,0x18,0x00,0x00,
0x00,0x38,0x00,0x00,
0x00,0x38,0x00,0x00,
0x00,0x38,0x00,0x00,
0x00,0x2C,0x00,0x00,
0x00,0x6C,0x00,0x00,
0x00,0x6C,0x00,0x00,
0x00,0x6C,0x00,0x00,
0x00,0x6C,0x00,0x00,
0x00,0xC4,0x00,0x00,
0x7F,0xC6,0x00,0x00,
0x7F,0xC6,0x00,0x00,
0x00,0x06,0x00,0x00,
0x00,0x06,0x3F,0xFE,
0x00,0x02,0x7F,0xFE,
0x00,0x02,0x60,0x00,
0x00,0x03,0x60,0x00,
0x00,0x03,0x40,0x00,
0x00,0x03,0xC0,0x00,
0x00,0x03,0xC0,0x00,
0x00,0x01,0xC0,0x00,
0x00,0x01,0x80,0x00,
0x00,0x01,0x80,0x00,
0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,
};

const byte menu_stopwatch[] PROGMEM ={
0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,
0x0C,0x0F,0xF0,0x30,
0x1E,0x3F,0xFC,0x78,
0x3F,0xF1,0x0F,0xFC,
0x3F,0xC1,0x83,0xFC,
0x1F,0x21,0x84,0xF8,
0x0E,0x01,0x80,0x70,
0x0C,0x01,0x80,0x30,
0x18,0x01,0x80,0x18,
0x18,0x01,0x80,0x18,
0x34,0x01,0x80,0x2C,
0x30,0x01,0x80,0x0C,
0x30,0x01,0x80,0x0C,
0x20,0x01,0x80,0x04,
0x60,0x01,0x80,0x06,
0x78,0x01,0xC0,0x1E,
0x60,0x00,0xE0,0x06,
0x20,0x00,0x70,0x04,
0x30,0x00,0x38,0x0C,
0x30,0x00,0x1C,0x0C,
0x34,0x00,0x0E,0x2C,
0x18,0x00,0x07,0x18,
0x18,0x00,0x03,0x18,
0x0C,0x00,0x00,0x30,
0x06,0x00,0x00,0x60,
0x03,0x20,0x04,0xC0,
0x01,0xC1,0x03,0x80,
0x00,0xF1,0x0F,0x00,
0x00,0x3F,0xFC,0x00,
0x00,0x0F,0xF0,0x00,
};

const byte _0011_0[] PROGMEM ={
0x7F,0xFF,0xC0,
0xFF,0xFF,0xE0,
0xFF,0xFF,0xE0,
0xFF,0xFF,0xE0,
0xF8,0x03,0xE0,
0xF8,0x03,0xE0,
0xF8,0x03,0xE0,
0xF8,0x03,0xE0,
0xF8,0x03,0xE0,
0xF8,0x03,0xE0,
0xF8,0x03,0xE0,
0xF8,0x03,0xE0,
0xF8,0x03,0xE0,
0xF8,0x03,0xE0,
0xF8,0x03,0xE0,
0xF8,0x03,0xE0,
0xF8,0x03,0xE0,
0xF8,0x03,0xE0,
0xF8,0x03,0xE0,
0xF8,0x03,0xE0,
0xFF,0xFF,0xE0,
0xFF,0xFF,0xE0,
0xFF,0xFF,0xE0,
0x7F,0xFF,0xC0,
};

const byte _0010_1[] PROGMEM ={
0x00,0xF8,0x00,
0x00,0xF8,0x00,
0x01,0xF8,0x00,
0x01,0xF8,0x00,
0x01,0xF8,0x00,
0x03,0xF8,0x00,
0x00,0xF8,0x00,
0x00,0xF8,0x00,
0x00,0xF8,0x00,
0x00,0xF8,0x00,
0x00,0xF8,0x00,
0x00,0xF8,0x00,
0x00,0xF8,0x00,
0x00,0xF8,0x00,
0x00,0xF8,0x00,
0x00,0xF8,0x00,
0x00,0xF8,0x00,
0x00,0xF8,0x00,
0x00,0xF8,0x00,
0x00,0xF8,0x00,
0x00,0xF8,0x00,
0x00,0xF8,0x00,
0x00,0xF8,0x00,
0x00,0xF8,0x00,
};

const byte _0009_2[] PROGMEM ={
0x3F,0xFF,0xC0,
0x7F,0xFF,0xE0,
0x7F,0xFF,0xE0,
0x7F,0xFF,0xE0,
0x7C,0x03,0xE0,
0x7C,0x03,0xE0,
0x7C,0x03,0xE0,
0x7C,0x03,0xE0,
0x00,0x07,0xE0,
0x00,0x0F,0xC0,
0x00,0x1F,0x80,
0x00,0x3F,0x00,
0x00,0x7E,0x00,
0x00,0xFC,0x00,
0x01,0xF8,0x00,
0x03,0xF0,0x00,
0x07,0xE0,0x00,
0x0F,0xC0,0x00,
0x1F,0x80,0x00,
0x3F,0x00,0x00,
0x7F,0xFF,0xE0,
0xFF,0xFF,0xE0,
0xFF,0xFF,0xE0,
0xFF,0xFF,0xE0,
};

const byte _0008_3[] PROGMEM ={
0x7F,0xFF,0xC0,
0xFF,0xFF,0xE0,
0xFF,0xFF,0xE0,
0xFF,0xFF,0xE0,
0xF8,0x03,0xE0,
0xF8,0x03,0xE0,
0xF8,0x03,0xE0,
0xF8,0x03,0xE0,
0x00,0x07,0xE0,
0x00,0x3F,0xE0,
0x00,0xFF,0x80,
0x01,0xFC,0x00,
0x00,0xFF,0x80,
0x00,0x3F,0xE0,
0x00,0x0F,0xE0,
0x00,0x03,0xE0,
0xF8,0x03,0xE0,
0xF8,0x03,0xE0,
0xF8,0x03,0xE0,
0xF8,0x03,0xE0,
0xFF,0xFF,0xE0,
0xFF,0xFF,0xE0,
0xFF,0xFF,0xE0,
0x7F,0xFF,0xC0,
};

const byte _0007_4[] PROGMEM ={
0x00,0x7E,0x00,
0x00,0x7C,0x00,
0x00,0xF8,0x00,
0x00,0xF8,0x00,
0x01,0xF0,0x00,
0x03,0xE0,0x00,
0x03,0xE0,0x00,
0x07,0xC0,0x00,
0x07,0xC7,0x80,
0x0F,0x87,0x80,
0x1F,0x07,0x80,
0x1F,0x07,0x80,
0x3E,0x07,0x80,
0x3E,0x07,0x80,
0x7C,0x07,0x80,
0xFC,0x07,0x80,
0xFF,0xFF,0xE0,
0xFF,0xFF,0xE0,
0xFF,0xFF,0xE0,
0xFF,0xFF,0xE0,
0x00,0x07,0x80,
0x00,0x07,0x80,
0x00,0x07,0x80,
0x00,0x07,0x80,
};

const byte _0006_5[] PROGMEM ={
0xFF,0xFF,0xE0,
0xFF,0xFF,0xE0,
0xFF,0xFF,0xE0,
0xFF,0xFF,0xE0,
0xF8,0x00,0x00,
0xF8,0x00,0x00,
0xF8,0x00,0x00,
0xF8,0x00,0x00,
0xF8,0x00,0x00,
0xFF,0xFF,0xC0,
0xFF,0xFF,0xE0,
0xFF,0xFF,0xE0,
0xFF,0xFF,0xE0,
0x00,0x03,0xE0,
0x00,0x03,0xE0,
0x00,0x03,0xE0,
0xF8,0x03,0xE0,
0xF8,0x03,0xE0,
0xF8,0x03,0xE0,
0xF8,0x03,0xE0,
0xFF,0xFF,0xE0,
0xFF,0xFF,0xE0,
0xFF,0xFF,0xE0,
0x7F,0xFF,0xC0,
};

const byte _0005_6[] PROGMEM ={
0x7F,0xFF,0xC0,
0xFF,0xFF,0xE0,
0xFF,0xFF,0xE0,
0xFF,0xFF,0xE0,
0xF8,0x03,0xE0,
0xF8,0x03,0xE0,
0xF8,0x03,0xE0,
0xF8,0x00,0x00,
0xF8,0x00,0x00,
0xF8,0x00,0x00,
0xFF,0xFF,0xC0,
0xFF,0xFF,0xE0,
0xFF,0xFF,0xE0,
0xFF,0xFF,0xE0,
0xF8,0x03,0xE0,
0xF8,0x03,0xE0,
0xF8,0x03,0xE0,
0xF8,0x03,0xE0,
0xF8,0x03,0xE0,
0xF8,0x03,0xE0,
0xFF,0xFF,0xE0,
0xFF,0xFF,0xE0,
0xFF,0xFF,0xE0,
0x7F,0xFF,0xC0,
};

const byte _0004_7[] PROGMEM ={
0xFF,0xFF,0xE0,
0xFF,0xFF,0xE0,
0xFF,0xFF,0xE0,
0xFF,0xFF,0xE0,
0xF8,0x03,0xE0,
0xF8,0x03,0xC0,
0xF8,0x07,0xC0,
0x00,0x07,0xC0,
0x00,0x0F,0x80,
0x00,0x0F,0x80,
0x00,0x1F,0x00,
0x00,0x1F,0x00,
0x00,0x3E,0x00,
0x00,0x3E,0x00,
0x00,0x3C,0x00,
0x00,0x7C,0x00,
0x00,0x7C,0x00,
0x00,0xF8,0x00,
0x00,0xF8,0x00,
0x01,0xF0,0x00,
0x01,0xF0,0x00,
0x03,0xE0,0x00,
0x03,0xE0,0x00,
0x07,0xE0,0x00,
};

const byte _0003_8[] PROGMEM ={
0x7F,0xFF,0xC0,
0xFF,0xFF,0xE0,
0xFF,0xFF,0xE0,
0xFF,0xFF,0xE0,
0xF8,0x03,0xE0,
0xF8,0x03,0xE0,
0xF8,0x03,0xE0,
0xF8,0x03,0xE0,
0xF8,0x03,0xE0,
0xFF,0x1F,0xE0,
0x7F,0xFF,0xC0,
0x0F,0xFE,0x00,
0x1F,0xFF,0x00,
0x7F,0xBF,0xC0,
0xFC,0x07,0xE0,
0xF8,0x03,0xE0,
0xF8,0x03,0xE0,
0xF8,0x03,0xE0,
0xF8,0x03,0xE0,
0xF8,0x03,0xE0,
0xFF,0xFF,0xE0,
0xFF,0xFF,0xE0,
0xFF,0xFF,0xE0,
0x7F,0xFF,0xC0,
};

const byte _0002_9[] PROGMEM ={
0x7F,0xFF,0xC0,
0xFF,0xFF,0xE0,
0xFF,0xFF,0xE0,
0xFF,0xFF,0xE0,
0xF8,0x03,0xE0,
0xF8,0x03,0xE0,
0xF8,0x03,0xE0,
0xF8,0x03,0xE0,
0xF8,0x03,0xE0,
0xF8,0x03,0xE0,
0xFF,0xFF,0xE0,
0xFF,0xFF,0xE0,
0xFF,0xFF,0xE0,
0x7F,0xFF,0xE0,
0x00,0x03,0xE0,
0x00,0x03,0xE0,
0x00,0x03,0xE0,
0xF8,0x03,0xE0,
0xF8,0x03,0xE0,
0xF8,0x03,0xE0,
0xFF,0xFF,0xE0,
0xFF,0xFF,0xE0,
0xFF,0xFF,0xE0,
0x7F,0xFF,0xC0,
};

const byte pt[] PROGMEM ={
0xFF,0xFF,0xE0,
0xFF,0xFF,0xE0,
0xFF,0xFF,0xE0,
0xFF,0xFF,0xE0,
0xFF,0xFF,0xE0,
0xFF,0xFF,0xE0,
0xFF,0xFF,0xE0,
0xFF,0xFF,0xE0,
0xFF,0xFF,0xE0,
0xFF,0xFF,0xE0,
0xFF,0xFF,0xE0,
0xFF,0xFF,0xE0,
0xFF,0xFF,0xE0,
0xFF,0xFF,0xE0,
0xFF,0xFF,0xE0,
0xFF,0xFF,0xE0,
0xFF,0xFF,0xE0,
0xFF,0xFF,0xE0,
0xFF,0xFF,0xE0,
0xFF,0xFF,0xE0,
0xFF,0xFF,0xE0,
0xFF,0xFF,0xE0,
0xFF,0xFF,0xE0,
0xFF,0xFF,0xE0,
};


const byte  _0[] PROGMEM ={
0x7F,0xC0,
0xFF,0xE0,
0xFF,0xE0,
0xF1,0xE0,
0xE0,0xE0,
0xE0,0xE0,
0xE0,0xE0,
0xE0,0xE0,
0xE0,0xE0,
0xE0,0xE0,
0xE0,0xE0,
0xE0,0xE0,
0xF1,0xE0,
0xFF,0xE0,
0xFF,0xE0,
0x7F,0xC0,
};

const byte _1[] PROGMEM ={
0x07,0x00,
0x0F,0x00,
0x1F,0x00,
0x3F,0x00,
0x3F,0x00,
0x07,0x00,
0x07,0x00,
0x07,0x00,
0x07,0x00,
0x07,0x00,
0x07,0x00,
0x07,0x00,
0x07,0x00,
0x07,0x00,
0x07,0x00,
0x07,0x00,
};

const byte _2[] PROGMEM ={
0x7F,0xC0,
0xFF,0xE0,
0xFF,0xE0,
0xE0,0xE0,
0xE0,0xE0,
0x01,0xE0,
0x03,0xC0,
0x07,0x80,
0x0F,0x00,
0x1E,0x00,
0x3C,0x00,
0x78,0x00,
0xF0,0x00,
0xFF,0xE0,
0xFF,0xE0,
0xFF,0xE0,
};

const byte _3[] PROGMEM ={
0x7F,0xC0,
0xFF,0xE0,
0xFF,0xE0,
0xE1,0xE0,
0x00,0xE0,
0x01,0xE0,
0x03,0xC0,
0x07,0x80,
0x07,0x80,
0x03,0xC0,
0x01,0xE0,
0x00,0xE0,
0xE1,0xE0,
0xFF,0xE0,
0xFF,0xE0,
0x7F,0xC0,
};

const byte _4[] PROGMEM ={
0x03,0x80,
0x07,0x80,
0x0F,0x80,
0x1F,0x80,
0x3F,0x80,
0x7B,0x80,
0xF3,0x80,
0xE3,0x80,
0xE3,0x80,
0xE3,0x80,
0xFF,0xE0,
0xFF,0xE0,
0xFF,0xE0,
0x03,0x80,
0x03,0x80,
0x03,0x80,
};

const byte _5[] PROGMEM ={
0xFF,0xE0,
0xFF,0xE0,
0xFF,0xE0,
0xE0,0x00,
0xE0,0x00,
0xE0,0x00,
0xFF,0x80,
0xFF,0xC0,
0xFF,0xE0,
0x01,0xE0,
0x00,0xE0,
0x00,0xE0,
0xE1,0xE0,
0xFF,0xE0,
0xFF,0xC0,
0x7F,0x80,
};

const byte _6[] PROGMEM ={
0x7F,0xC0,
0xFF,0xE0,
0xFF,0xE0,
0xF0,0xE0,
0xE0,0x00,
0xE0,0x00,
0xE0,0x00,
0xFF,0xC0,
0xFF,0xE0,
0xFF,0xE0,
0xE0,0xE0,
0xE0,0xE0,
0xE0,0xE0,
0xFF,0xE0,
0xFF,0xE0,
0x7F,0xC0,
};

const byte _7[] PROGMEM ={
0xFF,0xE0,
0xFF,0xE0,
0xFF,0xE0,
0xE0,0xE0,
0x00,0xE0,
0x00,0xE0,
0x01,0xE0,
0x03,0xC0,
0x07,0x80,
0x0F,0x00,
0x1E,0x00,
0x3C,0x00,
0x78,0x00,
0xF0,0x00,
0xE0,0x00,
0xC0,0x00,
};

const byte _8[] PROGMEM ={
0x7F,0xC0,
0xFF,0xE0,
0xFF,0xE0,
0xE0,0xE0,
0xE0,0xE0,
0xE0,0xE0,
0xF1,0xE0,
0x7F,0xC0,
0x7F,0xC0,
0xF1,0xE0,
0xE0,0xE0,
0xE0,0xE0,
0xE0,0xE0,
0xFF,0xE0,
0xFF,0xE0,
0x7F,0xC0,
};

const byte _9[] PROGMEM ={
0x7F,0xC0,
0xFF,0xE0,
0xFF,0xE0,
0xE0,0xE0,
0xE0,0xE0,
0xE0,0xE0,
0xFF,0xE0,
0xFF,0xE0,
0x7F,0xE0,
0x00,0xE0,
0x00,0xE0,
0xE0,0xE0,
0xE0,0xE0,
0xFF,0xE0,
0xFF,0xE0,
0x7F,0xC0,
};

const byte _pt[] PROGMEM ={
0xFF,0xF0,
0xFF,0xF0,
0xFF,0xF0,
0xFF,0xF0,
0xFF,0xF0,
0xFF,0xF0,
0xFF,0xF0,
0xFF,0xF0,
0xFF,0xF0,
0xFF,0xF0,
0xFF,0xF0,
0xFF,0xF0,
0xFF,0xF0,
0xFF,0xF0,
0xFF,0xF0,
0xFF,0xF0,
};

char lant =3;

char velocidade = 20;
char menu = 0;
char botao = 0;
char botao1 = 0;
char botao2 = 0;
char botao3 = 0;
char botao4 = 0;
char in6=0;
char in1=0;
char in2=3;

char dateTime[20];
int RTCValues[7];
char s;
char s1;
char m;
char m1;
char h;
char h1;
int temp = 50;

bool disp= 0;

int m2, mu=0,md=0;                          //Declaramos las variables que vamos a usar
int s2, su=0,sd=0;
int l, lu=0,ld=0,lc=0;
int pulsador,antes,despues;
long int tiempo,inicio;

void setup() {
  
      power_usart0_disable(); // Disable UART (no Serial)
      power_timer1_disable();
      power_timer2_disable();
      power_spi_disable();                         // Disable remaining periphs
      PORTB = PORTC = PORTD = 0xFF;        // enable pullups (for power saving)  
      EICRA = _BV(ISC11); // Falling edge of INT1 (pin 3) generates an interrupt
      EIMSK = _BV(INT1);  // Enable interrupt (vibration switch wakes from sleep)
      
  analogReference(INTERNAL);
  display.begin(SH1106_SWITCHCAPVCC, 0x3C);     // needed for SH1106 display
  display.clearDisplay();                       // clear display
  pinMode(navegar,INPUT_PULLUP);
  pinMode(selecionar,INPUT_PULLUP);
  pinMode(mais,INPUT_PULLUP);
  pinMode(menos,INPUT_PULLUP);
  pinMode(13,OUTPUT);
  pinMode(lanterna,OUTPUT);
  digitalWrite(lanterna,LOW);
  
 pinMode(0, OUTPUT);
 pinMode(1, OUTPUT);
 pinMode(4, OUTPUT);
 pinMode(7, OUTPUT);
 pinMode(8, OUTPUT);
 pinMode(10, OUTPUT);
 pinMode(11, OUTPUT);
 pinMode(12, OUTPUT);

 digitalWrite(0, LOW);
 digitalWrite(1, LOW);
 digitalWrite(4, LOW);
 digitalWrite(7, LOW);
 digitalWrite(8, LOW);
 digitalWrite(10, LOW);
 digitalWrite(11, LOW);
 digitalWrite(12, LOW);
 
  DS1307.begin();
  antes=HIGH;                             //Ponemos la variable antes a HIGH
  despues=HIGH; 
}

void loop() {

s = RTCValues[6] % 10;
s1 = RTCValues[6] / 10; 
m = RTCValues[5] % 10;
m1 = RTCValues[5] / 10;
h = RTCValues[4] % 10;
h1 = RTCValues[4] / 10;
  



if(RTCValues[6] == temp && menu != 10 ) {    
        digitalWrite(13,LOW);
        display.SH1106_command(SH1106_DISPLAYOFF); // Screen off
        power_timer0_disable();
        power_adc_disable();    // Disable ADC (no analogRead())
        power_twi_disable();    // Disable I2C (no Wire library)
        set_sleep_mode(SLEEP_MODE_PWR_DOWN);         // Deepest sleep
        LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
        sleep_mode();
        // Execution resumes here on wake.
        digitalWrite(13,HIGH);
        menu = 0;
        power_adc_enable();    // Disable ADC (no analogRead())
        power_twi_enable();    // Disable I2C (no Wire library)
        power_timer0_enable(); 
        display.SH1106_command(SH1106_DISPLAYON);  // Main screen turn on
        
      }

  
if(digitalRead(navegar)==LOW){
while(digitalRead(navegar) == LOW);
beep(50,175);
menu++;
if(menu >=12)
{
menu = 0;
}
}

switch(menu)
    {


 case -1:
       for (int i=0; i<70; i+=velocidade)
  {
   display.clearDisplay();
   display.setTextSize(1);
   display.setTextColor(WHITE);
   DS1307.getDate(RTCValues); 
   data(RTCValues[3],RTCValues[2],RTCValues[0],RTCValues[1],69-i);
   mostraM(0, 91-i , h1);
   mostraM(24, 91-i , h);
   display.setTextSize(5);
   display.setTextColor(WHITE);
   display.setCursor(37,86-i);
      if(s % 2 == 1)
  {
   display.println(":");
  }
   mostraM(56, 91-i , m1);
   mostraM(80, 91-i , m);
   mostraS(115, 99-i , s);
   mostraS(102, 99-i, s1);
   bateria(0,125-i);
   display.setTextSize(1);
   display.setCursor(85,125-i);
   display.println("JACKSON");
   display.setCursor(24,0-i);
   display.print("<    MENU   >");
   display.setCursor(0,56-i);
   display.print("Sair");
   bateria(112,0-i);
   display.drawBitmap(45, 15-i,  selectbar_top, 36, 8, 1);
   display.drawBitmap(45, 45-i,  selectbar_bottom, 36, 8, 1);
   display.drawBitmap(96, 18-i,  menu_settings, 32, 32, 1);
   display.drawBitmap(0, 18-i,  menu_stopwatch, 32, 32, 1);
   display.drawBitmap(47, 18-i,  menu_exit, 32,32, 1);
   display.display();
   
    if(i < 70)
  menu = 0;
  }
      break;
      
      case 0:
    botao2 = 0;
   display.clearDisplay();
   display.setTextSize(1);
   display.setTextColor(WHITE);
   DS1307.getDate(RTCValues); 
   data(RTCValues[3],RTCValues[2],RTCValues[0],RTCValues[1],0);
   mostraM(0, 22 , h1);
   mostraM(24, 22 , h);
   display.setTextSize(5);
   display.setTextColor(WHITE);
   display.setCursor(37,17);
    if(s % 2 == 0)
  {
   display.println(":");
  }
   mostraM(56, 22 , m1);
   mostraM(80, 22 , m);
   mostraS(115, 30 , s);
   mostraS(102, 30, s1);
   display.setTextSize(1);
   display.setCursor(85-in6,56);
   display.println("JACKSON");
   display.drawBitmap(0, 56,  _pt, 16, 16, 0);
   display.drawBitmap(5, 56,  _pt, 16, 16, 0);
   bateria(0,56);
   display.display();
   in6+=2;
   botao = 0;
      
      break;
      
      case 1:
       for (int i=0; i<70; i+=velocidade)
  {
   display.clearDisplay();
   display.setTextSize(1);
   display.setTextColor(WHITE);
   DS1307.getDate(RTCValues); 
   data(RTCValues[3],RTCValues[2],RTCValues[0],RTCValues[1],0+i);
   mostraM(0, 22+i , h1);
   mostraM(24, 22+i , h);
   display.setTextSize(5);
   display.setTextColor(WHITE);
   display.setCursor(37,17+i);
   display.println(":");
   mostraM(56, 22+i , m1);
   mostraM(80, 22+i , m);
   mostraS(115, 30+i , s);
   mostraS(102, 30+i, s1);
   bateria(0,56+i);
   display.setTextSize(1);
   display.setCursor(85-in6,56+i);
   display.println("JACKSON");
    
   display.setCursor(24,-69+i);
   display.print("<    MENU   >");
   display.setCursor(0,-13+i);
   display.print("Sair");
   bateria(112,-69+i);
   display.drawBitmap(45, -54+i,  selectbar_top, 36, 8, 1);
   display.drawBitmap(45, -24+i,  selectbar_bottom, 36, 8, 1);
   display.drawBitmap(96, -51+i,  menu_settings, 32, 32, 1);
   display.drawBitmap(0, -51+i,  menu_stopwatch, 32, 32, 1);
   display.drawBitmap(47, -51+i,  menu_exit, 32,32, 1);
   display.display();
    in6+=2;
    if(i < 70)
  menu = 2;
  }
      break;
      
         case 2:
  botao4 = 0;
if(digitalRead(selecionar)==LOW){
while(digitalRead(selecionar) == LOW);
botao++;
beep(35,155);
if(botao >=2)
{
botao = 0;
}
}
        if(botao == 0){ 
            botao4 ==0 ;
   display.clearDisplay();
   display.setTextSize(1);
   display.setCursor(24,0);
   display.print("<    MENU   >");
   display.setCursor(0,56);
   display.print("Sair");
   bateria(112,0);
   display.drawBitmap(45, 15,  selectbar_top, 36, 8, 1);
   display.drawBitmap(45, 45,  selectbar_bottom, 36, 8, 1);
   display.drawBitmap(96, 18,  menu_settings, 32, 32, 1);
   display.drawBitmap(0, 18,  menu_stopwatch, 32, 32, 1);
   display.drawBitmap(47, 18,  menu_exit, 32,32, 1);
   display.display();
        }
        if(botao == 1)
        menu = -1;
          botao4 ==0 ;
      break;

 case 3:
 for (int i=0; i<48; i+=velocidade)
  {
   display.clearDisplay();
   display.setCursor(24,0);
   display.print("<    MENU   >");
   display.setCursor(0,56);
   display.print("Ajustes");
   bateria(112,0);
   display.drawBitmap(45, 15,  selectbar_top, 36, 8, 1);
   display.drawBitmap(45, 45,  selectbar_bottom, 36, 8, 1);
   display.drawBitmap(96-i, 18,  menu_settings, 32, 32, 1);
   display.drawBitmap(0-i, 18,  menu_stopwatch, 32, 32, 1);
   display.drawBitmap(47-i, 18,  menu_exit, 32,32, 1);
   display.drawBitmap(142-i, 18,  torch, 32, 32, 1);
   display.display();
    if(i < 48)
  menu = 4;
  }
      break;

 case 4:
 
if(digitalRead(selecionar)==LOW){
while(digitalRead(selecionar) == LOW);
botao4++;
beep(35,155);
if(botao4 >=10)
{
botao4 = 0;
}
}
if(botao4 ==0){
  
   display.clearDisplay();
   display.setTextSize(1);
   display.setCursor(24,0);
   display.print("<    MENU   >");
   display.setCursor(0,56);
   display.print("Ajustes");
   bateria(112,0);
   display.drawBitmap(45, 15,  selectbar_top, 36, 8, 1);
   display.drawBitmap(45, 45,  selectbar_bottom, 36, 8, 1);
   display.drawBitmap(96, 18,  torch, 32, 32, 1);
   display.drawBitmap(0, 18,  menu_exit, 32, 32, 1);
   display.drawBitmap(47, 18,  menu_settings, 32,32, 1);
   display.display();
}
if(botao4 ==1){

if(digitalRead(mais)==LOW ){
RTCValues[3]++;
beep(20,180);
}

if(digitalRead(menos)==LOW ){
RTCValues[3]--;
beep(20,180);
}
 
   display.clearDisplay();
   display.setTextSize(1);
   display.setCursor(24,0);
   display.print("<   AJUSTES  >");
   bateria(112,0);
   data(RTCValues[3],RTCValues[2],RTCValues[0],RTCValues[1],20);
   display.setCursor(10,25);
   display.println("___");
   display.setTextSize(1);
     display.setCursor(39,38);
       display.println(RTCValues[4]);
       display.setCursor(49,38);
       display.println(":");
        display.setTextSize(1);
        display.setCursor(59,38);
       display.println(RTCValues[5]);
        display.setCursor(69,38);
       display.println(":");
        display.setTextSize(1);
      display.setCursor(79,38);
       display.println(RTCValues[6]);
       display.setCursor(0,56);
   display.print("DATA/HORA");
   display.display();
   if(RTCValues[3] > 7)
   {
   RTCValues[3] = 1;
   }
    if(RTCValues[3] < 1)
    {
   RTCValues[3] = 7;
    }
}
if(botao4 ==2){

if(digitalRead(mais)==LOW ){
RTCValues[2]++;
beep(20,180);
}

if(digitalRead(menos)==LOW ){
RTCValues[2]--;
beep(20,180);
}
 
   display.clearDisplay();
   display.setTextSize(1);
   display.setCursor(24,0);
   display.print("<   AJUSTES  >");
   bateria(112,0);
   data(RTCValues[3],RTCValues[2],RTCValues[0],RTCValues[1],20);
   display.setCursor(37,25);
   display.println("__");
   display.setTextSize(1);
     display.setCursor(39,38);
       display.println(RTCValues[4]);
       display.setCursor(49,38);
       display.println(":");
        display.setTextSize(1);
        display.setCursor(59,38);
       display.println(RTCValues[5]);
        display.setCursor(69,38);
       display.println(":");
        display.setTextSize(1);
      display.setCursor(79,38);
       display.println(RTCValues[6]);
       display.setCursor(0,56);
   display.print("DATA/HORA");
   display.display();
   if(RTCValues[2] > 30)
   {
   RTCValues[2] = 1;
   }
    if(RTCValues[2] < 1)
    {
   RTCValues[2] =30;
    }
}

if(botao4 ==3){

 if(digitalRead(mais)==LOW ){
RTCValues[1]++;
beep(20,180);
}

if(digitalRead(menos)==LOW ){
RTCValues[1]--;
beep(20,180);
}
 
   display.clearDisplay();
   display.setTextSize(1);
   display.setCursor(24,0);
   display.print("<   AJUSTES  >");
   bateria(112,0);
   data(RTCValues[3],RTCValues[2],RTCValues[0],RTCValues[1],20);
   display.setCursor(60,25);
   display.println("___");
   display.setTextSize(1);
     display.setCursor(39,38);
       display.println(RTCValues[4]);
       display.setCursor(49,38);
       display.println(":");
        display.setTextSize(1);
        display.setCursor(59,38);
       display.println(RTCValues[5]);
        display.setCursor(69,38);
       display.println(":");
        display.setTextSize(1);
      display.setCursor(79,38);
       display.println(RTCValues[6]);
       display.setCursor(0,56);
   display.print("DATA/HORA");
   display.display();
   if(RTCValues[1] > 12)
   {
   RTCValues[1] = 1;
   }
    if(RTCValues[1] < 1)
    {
   RTCValues[1] =12;
    }
}


if(botao4 ==4){

 if(digitalRead(mais)==LOW ){
RTCValues[0]++;
beep(20,180);
}

if(digitalRead(menos)==LOW ){
RTCValues[0]--;
beep(20,180);
}
 
   display.clearDisplay();
   display.setTextSize(1);
   display.setCursor(24,0);
   display.print("<   AJUSTES  >");
   bateria(112,0);
   data(RTCValues[3],RTCValues[2],RTCValues[0],RTCValues[1],20);
   display.setCursor(90,25);
   display.println("____");
   display.setTextSize(1);
     display.setCursor(39,38);
       display.println(RTCValues[4]);
       display.setCursor(49,38);
       display.println(":");
        display.setTextSize(1);
        display.setCursor(59,38);
       display.println(RTCValues[5]);
        display.setCursor(69,38);
       display.println(":");
        display.setTextSize(1);
      display.setCursor(79,38);
       display.println(RTCValues[6]);
       display.setCursor(0,56);
   display.print("DATA/HORA");
   display.display();
   if(RTCValues[0] > 99)
   {
   RTCValues[0] = 17;
   }
    if(RTCValues[0] < 17)
    {
   RTCValues[0] =99;
    }
}

if(botao4 ==5){

 if(digitalRead(mais)==LOW ){
RTCValues[4]++;
beep(20,180);
}

if(digitalRead(menos)==LOW ){
RTCValues[4]--;
beep(20,180);
}
 
   display.clearDisplay();
   display.setTextSize(1);
   display.setCursor(24,0);
   display.print("<   AJUSTES  >");
   bateria(112,0);
   data(RTCValues[3],RTCValues[2],RTCValues[0],RTCValues[1],20);
   display.setCursor(39,43);
   display.println("__");
   display.setTextSize(1);
     display.setCursor(39,38);
       display.println(RTCValues[4]);
       display.setCursor(49,38);
       display.println(":");
        display.setTextSize(1);
        display.setCursor(59,38);
       display.println(RTCValues[5]);
        display.setCursor(69,38);
       display.println(":");
        display.setTextSize(1);
      display.setCursor(79,38);
       display.println(RTCValues[6]);
       display.setCursor(0,56);
   display.print("DATA/HORA");
   display.display();
   if(RTCValues[4] > 23)
   {
   RTCValues[4] = 0;
   }
    if(RTCValues[4] < 0)
    {
   RTCValues[4] =23;
    }
}

if(botao4 ==6){

 if(digitalRead(mais)==LOW ){
RTCValues[5]++;
beep(20,180);
}

if(digitalRead(menos)==LOW ){
RTCValues[5]--;
beep(20,180);
}
 
   display.clearDisplay();
   display.setTextSize(1);
   display.setCursor(24,0);
   display.print("<   AJUSTES  >");
   bateria(112,0);
   data(RTCValues[3],RTCValues[2],RTCValues[0],RTCValues[1],20);
   display.setCursor(59,43);
   display.println("__");
   display.setTextSize(1);
     display.setCursor(39,38);
       display.println(RTCValues[4]);
       display.setCursor(49,38);
       display.println(":");
        display.setTextSize(1);
        display.setCursor(59,38);
       display.println(RTCValues[5]);
        display.setCursor(69,38);
       display.println(":");
        display.setTextSize(1);
      display.setCursor(79,38);
       display.println(RTCValues[6]);
       display.setCursor(0,56);
   display.print("DATA/HORA");
   display.display();
   if(RTCValues[5] > 59)
   {
   RTCValues[5] = 0;
   }
    if(RTCValues[5] < 0)
    {
   RTCValues[5] =59;
    }
}

if(botao4 ==7){

 if(digitalRead(mais)==LOW ){
RTCValues[6]++;
beep(20,180);
}

if(digitalRead(menos)==LOW ){
RTCValues[6]--;
beep(20,180);
}
 
   display.clearDisplay();
   display.setTextSize(1);
   display.setCursor(24,0);
   display.print("<   AJUSTES  >");
   bateria(112,0);
   data(RTCValues[3],RTCValues[2],RTCValues[0],RTCValues[1],20);
   display.setCursor(79,43);
   display.println("__");
   display.setTextSize(1);
     display.setCursor(39,38);
       display.println(RTCValues[4]);
       display.setCursor(49,38);
       display.println(":");
        display.setTextSize(1);
        display.setCursor(59,38);
       display.println(RTCValues[5]);
        display.setCursor(69,38);
       display.println(":");
        display.setTextSize(1);
      display.setCursor(79,38);
       display.println(RTCValues[6]);
       display.setCursor(0,56);
   display.print("DATA/HORA");
   display.display();
   if(RTCValues[6] > 59)
   {
   RTCValues[6] = 0;
   }
    if(RTCValues[6] < 0)
    {
   RTCValues[6] =59;
    }
    DS1307.setDate(RTCValues[0], RTCValues[1], RTCValues[2], RTCValues[3], RTCValues[4], RTCValues[5], RTCValues[6]);
}


if(botao4 ==8){

 if(digitalRead(mais)==LOW ){
temp++;
beep(20,180);
}

if(digitalRead(menos)==LOW ){
temp--;
beep(20,180);
}
 
   display.clearDisplay();
   display.setTextSize(1);
   display.setCursor(24,0);
   display.print("<   AJUSTES  >");
   bateria(112,0);
   display.setCursor(0,20);
   display.println("Tempo ligado:");
   display.setCursor(80,20);
   display.print(temp);
   display.setCursor(80,25);
   display.println("__");
   display.setCursor(0,38);
   display.println("Inverter cores:");
  if(disp ==1){
   display.setCursor(90,38);
   display.print("S");
   }
   if(disp == 0){
   display.setCursor(90,38);
   display.print("N");
   }
   display.setCursor(0,56);
   display.print("TEMPO/INVERTER");
   display.display();
   if(temp > 60)
   {
   temp = 10;
   }
    if(temp < 10)
    {
   temp =60;
    }
}


if(botao4 ==9){

 if(digitalRead(mais)==LOW ){
 disp = 1;
beep(20,180);
}

if(digitalRead(menos)==LOW ){
disp = 0;
beep(20,180);
}
 
   display.clearDisplay();
   display.setTextSize(1);
   display.setCursor(24,0);
   display.print("<   AJUSTES  >");
   bateria(112,0);
   display.setCursor(0,20);
   display.println("Tempo ligado:");
   display.setCursor(80,20);
   display.print(temp);
   display.setCursor(90,43);
   display.println("_");
   display.setCursor(0,38);
   display.println("Inverter cores:");
   if(disp ==1){
   display.invertDisplay(true);
   display.setCursor(90,38);
   display.print("S");
   }
   if(disp == 0){
   display.invertDisplay(false);
   display.setCursor(90,38);
   display.print("N");
   }
   display.setCursor(0,56);
   display.print("TEMPO/INVERTER");
   display.display();

}
 
   botao3 = 0;
      break;

case 5:
for (int i=0; i<48; i+=velocidade)
  {
   display.clearDisplay();
   display.setTextSize(1);
   display.setCursor(24,0);
   display.print("<    MENU   >");
   display.setCursor(0,56);
   display.print("Lanterna");
   bateria(112,0);
   display.drawBitmap(45, 15,  selectbar_top, 36, 8, 1);
   display.drawBitmap(45, 45,  selectbar_bottom, 36, 8, 1);
   display.drawBitmap(96-i, 18,  torch, 32, 32, 1);
   display.drawBitmap(0-i, 18,  menu_exit, 32, 32, 1);
   display.drawBitmap(47-i, 18,  menu_settings, 32,32, 1);
   display.drawBitmap(142-i, 18,  menu_diagnostic, 32, 32, 1);
   display.display();
     if(i < 48)
  menu = 6;
  }
      break;
      
      
case 6:

if(digitalRead(selecionar)==LOW){
while(digitalRead(selecionar) == LOW);
botao3++;
beep(35,155);
if(botao3 >=2)
{
botao3 = 0;
}
}

if(botao3 ==0){
   display.clearDisplay();
   display.setTextSize(1);
   display.setCursor(24,0);
   display.print("<    MENU   >");
   display.setCursor(0,56);
   display.print("Lanterna");
   bateria(112,0);
   display.drawBitmap(45, 15,  selectbar_top, 36, 8, 1);
   display.drawBitmap(45, 45,  selectbar_bottom, 36, 8, 1);
   display.drawBitmap(96, 18,  menu_diagnostic, 32, 32, 1);
   display.drawBitmap(0, 18,  menu_settings, 32, 32, 1);
   display.drawBitmap(47, 18,  torch, 32,32, 1);
   display.display();
}

if(botao3 ==1){

if(digitalRead(mais)==LOW && lant ==3){
while(digitalRead(mais) == LOW);
lant = 1;
beep(35,155);
}

if(digitalRead(menos)==LOW && lant ==2){
while(digitalRead(menos) == LOW);
lant = 0;
beep(35,155);
}
  if(lant ==1){

   display.clearDisplay();
   display.setCursor(24,0);
   display.print("<  LANTERNA  >");
   bateria(112,0);
   display.drawBitmap(47+in1, 18,  torch, 32,32, 1);
   analogWrite(lanterna,map(in1, 0, 50, 0, 255));
   display.display();
  in1+=6;
  if(in1 >= 50)
  lant = 2;
    
 
  }
if(lant ==2){
    display.clearDisplay();
   display.setCursor(24,0);
   display.print("<  LANTERNA  >");
   bateria(112,0);
   display.drawBitmap(100, 18,  torch, 32,32, 1);
   analogWrite(lanterna,255);
   display.display();
   in1=0;
  }
  
 if(lant ==0){
    display.clearDisplay();
   display.setCursor(24,0);
   display.print("<  LANTERNA  >");
   bateria(112,0);
   display.drawBitmap(100-in2, 18,  torch, 32,32, 1);
   analogWrite(lanterna,map(in2, 0, 50, 255, 0));
   display.display();
    in2+=6;
  if(in2 >= 50)
  lant = 3;
  } 
if(lant ==3){
    display.clearDisplay();
   display.setCursor(24,0);
   display.print("<  LANTERNA  >");
   bateria(112,0);
   display.drawBitmap(47, 18,  torch, 32,32, 1);
   analogWrite(lanterna,0);
   display.display();
   in2=0;
  }
  
}

 botao1 = 0;
      break;

      case 7:
for (int i=0; i<48; i+=velocidade)
  {
   display.clearDisplay();
   display.setCursor(24,0);
   display.print("<    MENU   >");
   display.setCursor(0,56);
   display.print("Info");
   bateria(112,0);
   display.drawBitmap(45, 15,  selectbar_top, 36, 8, 1);
   display.drawBitmap(45, 45,  selectbar_bottom, 36, 8, 1);
   display.drawBitmap(96-i, 18,  menu_diagnostic, 32, 32, 1);
   display.drawBitmap(0-i, 18,  menu_settings, 32, 32, 1);
   display.drawBitmap(47-i, 18,  torch, 32,32, 1);
   display.drawBitmap(142-i, 18,  menu_stopwatch, 32, 32, 1);
   display.display();
     if(i < 48)
  menu = 8;
  }
      break;

      case 8:
      botao2 = 0;
      botao3 = 0;
if(digitalRead(selecionar)==LOW){
while(digitalRead(selecionar) == LOW);
botao1++;
beep(35,155);
if(botao1 >=2)
{
botao1 = 0;
}
}
if(botao1 ==0){
   display.clearDisplay();
   display.setCursor(24,0);
   display.print("<    MENU   >");
   display.setCursor(0,56);
   display.print("Info");
   bateria(112,0);
   display.drawBitmap(45, 15,  selectbar_top, 36, 8, 1);
   display.drawBitmap(45, 45,  selectbar_bottom, 36, 8, 1);
   display.drawBitmap(96, 18,  menu_stopwatch, 32, 32, 1);
   display.drawBitmap(0, 18,  torch, 32, 32, 1);
   display.drawBitmap(47, 18,  menu_diagnostic, 32,32, 1);
   display.display();
}
if(botao1 ==1){
  
   display.clearDisplay();
   display.setCursor(24,0);
   display.print("<    INFO   >");
   bateria(112,0);
   display.setCursor(0,13);
   display.println(readVcc());
   display.setCursor(19,13);
   display.println(" :Tensao da Bat/mV");
   display.setCursor(0,22);
   display.println(GetTemp());
   display.setCursor(22,22);
   display.println(" :Temperatura CPU");
   display.setCursor(0,31);
   display.println(temp);
   display.setCursor(22,31);
   display.println(" :Tempo ligado");
   display.setCursor(0,40);
   display.println(0.1);
   display.setCursor(20,40);
   display.println(" :Ver/Firmware");
   display.setCursor(0,56);
   display.println("JACKSON JUNIO 2017");
   display.display();
}
      break;

          case 9:
for (int i=0; i<48; i+=velocidade)
  {
   display.clearDisplay();
   display.setTextSize(1);
   display.setCursor(24,0);
   display.print("<    MENU   >");
   display.setCursor(0,56);
   display.print("Cronometro");
   bateria(112,0);
   display.drawBitmap(45, 15,  selectbar_top, 36, 8, 1);
   display.drawBitmap(45, 45,  selectbar_bottom, 36, 8, 1);
   display.drawBitmap(96-i, 18,  menu_stopwatch, 32, 32, 1);
   display.drawBitmap(0-i, 18,  torch, 32, 32, 1);
   display.drawBitmap(47-i, 18,  menu_diagnostic, 32,32, 1);
   display.drawBitmap(142-i, 18,  menu_exit, 32, 32, 1);
   display.display();
     if(i < 48)
  menu = 10;
  }
      break;


          case 10:
 botao1 = 0;     
if(digitalRead(selecionar)==LOW){
while(digitalRead(selecionar) == LOW);
botao2++;
beep(35,155);
if(botao2 >=2)
{
botao2 = 0;
}
}
if(botao2 ==0){
   display.clearDisplay();
   display.setTextSize(1);
   display.setCursor(24,0);
   display.print("<    MENU   >");
   display.setCursor(0,56);
   display.print("Cronometro");
   bateria(112,0);
   display.drawBitmap(45, 15,  selectbar_top, 36, 8, 1);
   display.drawBitmap(45, 45,  selectbar_bottom, 36, 8, 1);
   display.drawBitmap(96, 18,  menu_exit, 32, 32, 1);
   display.drawBitmap(0, 18,  menu_diagnostic, 32, 32, 1);
   display.drawBitmap(47, 18,  menu_stopwatch, 32,32, 1);
   display.display();
}
if(botao2 ==1){
   display.clearDisplay();
   display.setTextSize(1);
   display.setCursor(22,0);
   display.print("< CRONOMETRO >");
   bateria(112,0);
 display.setTextSize(3);
        display.setCursor(0,24);
       display.println(md);
       display.setCursor(18,24);
       display.println(mu);
       display.setCursor(31,24);
       display.println(":");
        display.setTextSize(3);
        display.setCursor(44,24);
       display.println(sd);
       display.setCursor(62,24);
       display.println(su);
        display.setCursor(75,24);
       display.println(":");
        display.setTextSize(3);
      display.setCursor(88,24);
       display.println(lc);
       display.setCursor(106,24);
       display.println(ld);

  if(digitalRead(menos)==LOW){
     beep(15,100);
    ld=0;
    lc= 0;
    su = 0;
    sd = 0;
    mu = 0;
    md = 0;
                 
  }
  despues=digitalRead(mais);                 //Guardamos en la variable despues el valor del pin6 (sensor start/stop)
  if(despues==LOW&&antes==HIGH){          //Si el valor que tenia era HIGH y ahora tiene LOW sera porque lo hemos activado
           beep(15,100);         //Mandamos una señal PWM con valor 100 al pin 10 (zumbador)
    inicio=millis();                      //Guardamos en inicio el tiempo que se lleva ejecutando el programa hasta que
                                          //se activa el sensor start/stop
    antes=despues;                        //cambiamos el valor de la variable antes al que tiene la variable despues
    pulsador=0;                           //Ponemos la variable pulsador a 0
                              //Establecemos un retardo de 200ms
     
    while(pulsador==0){                   //Cuando la variable pulsador sea 0
      tiempo=millis()-inicio;            //Calculamos el tiempo que paso desde que se activo el sensor start/stop
      m2=(tiempo/1000)/60;                 //Calculamos los minutos
      mu=m2%10;                            //Descomponemos los minutos y sacamos el valor de las unidades
      md=(m2-mu)/10;                       //Descomponemos los minutos y sacamos el valor de las decenas
      s2=(tiempo/1000)%60;                 //Calculamos los segundos
      su=s2%10;                            //Descomponemos los segundos y sacamos el valor de las unidades
      sd=(s2-su)/10;                       //Descomponemos los segundos y sacamos el valor de las decenas
      l=(tiempo%1000);                    //Calculamos las milesimas de segundo
      lu=l%10;                            //Descomponemos las milesimas y sacamos el valor de las unidades
      ld=((l-lu)/10)%10;                  //Descomponemos las milesimas y sacamos el valor de las decenas
      lc=(l-(ld*10)-lu)/100; 
     
   display.clearDisplay();
   display.setTextSize(1);
   display.setCursor(22,0);
   display.print("< CRONOMETRO >");
   bateria(112,0);
   display.setTextSize(3);
        display.setCursor(0,24);
       display.println(md);
       display.setCursor(18,24);
       display.println(mu);
       display.setCursor(31,24);
       display.println(":");
        display.setTextSize(3);
        display.setCursor(44,24);
       display.println(sd);
       display.setCursor(62,24);
       display.println(su);
        display.setCursor(75,24);
       display.println(":");
        display.setTextSize(3);
      display.setCursor(88,24);
   display.println(lc);
  display.setCursor(106,24);
 display.println(ld);
 display.display();
 
      despues=digitalRead(mais);             
      if(despues==LOW&&antes==HIGH){      
        antes=despues;                    
        pulsador=1;                       
            beep(15,100);         
      }
      antes=despues;                      
    }
  }
  antes=despues;   
   display.display();
}
 
      break;

 case 11:
for (int i=0; i<48; i+=velocidade)
  {
   display.clearDisplay();
   display.setTextSize(1);
   display.setCursor(24,0);
   display.print("<    MENU   >");
   display.setCursor(0,56);
   display.print("Sair");
   bateria(112,0);
   display.drawBitmap(45, 15,  selectbar_top, 36, 8, 1);
   display.drawBitmap(45, 45,  selectbar_bottom, 36, 8, 1);
   display.drawBitmap(96-i, 18,  menu_exit, 32, 32, 1);
   display.drawBitmap(0-i, 18,  menu_diagnostic, 32, 32, 1);
   display.drawBitmap(47-i, 18,  menu_stopwatch, 32,32, 1);
   display.drawBitmap(142-i, 18,  menu_settings, 32, 32, 1);
   display.display();
     if(i < 48)
  menu = 2;
  }
      break;

    }
}


void mostraS(int x, int y , int z)

{

  
switch (z) {
  
    case 0:
    display.drawBitmap(x, y,  _0, 16, 16, 1);
    break;
      
    case 1:
      display.drawBitmap(x, y,  _1, 16, 16, 1);
      break;
      
    case 2:
   display.drawBitmap(x, y,  _2, 16, 16, 1);
    break;
    
     case 3:
   display.drawBitmap(x, y,  _3, 16, 16, 1);
    break;
    
     case 4:
   display.drawBitmap(x, y,  _4, 16, 16, 1);
    break;
    
     case 5:
     display.drawBitmap(x, y,  _5, 16, 16, 1);
    break;
    
     case 6:
   display.drawBitmap(x, y,  _6, 16, 16, 1);
    break;
    
     case 7:
 display.drawBitmap(x, y,  _7, 16, 16, 1);
    break;
    
     case 8:
   display.drawBitmap(x, y,  _8, 16, 16, 1);
    break;
    
     case 9:
    display.drawBitmap(x, y,  _9, 16, 16, 1);
    break;
    
  
  }

} 


void mostraM(int x, int y , int z)

{

  
switch (z) {
  
      case 0:
      display.drawBitmap(x, y,  _0011_0, 24, 24, 1);
      break;
      
      case 1:
      display.drawBitmap(x, y,  _0010_1, 24, 24, 1);
      break;

      case 2:
      display.drawBitmap(x, y,  _0009_2, 24, 24, 1);
      break;

      case 3:
      display.drawBitmap(x, y,  _0008_3, 24, 24, 1);
      break;

      case 4:
      display.drawBitmap(x, y,  _0007_4, 24, 24, 1);
      break;

      case 5:
      display.drawBitmap(x, y,  _0006_5, 24, 24, 1);
      break;

      case 6:
      display.drawBitmap(x, y,  _0005_6, 24, 24, 1);
      break;

      case 7:
      display.drawBitmap(x, y,  _0004_7, 24, 24, 1);
      break;

      case 8:
      display.drawBitmap(x, y,  _0003_8, 24, 24, 1);
      break;

      case 9:
      display.drawBitmap(x, y,  _0002_9, 24, 24, 1);
      break;
 
    
  }

} 

void data(int x , int y , int z , int w , int f)
{
  
 display.setCursor(37,f);
 display.print(y);
 
 z = z + 2000;
 display.setCursor(90,f);
  display.print(z);
  
switch(x)
    {
      case 1:
      display.setCursor(10,f);
      display.print("Dom");
      break;
      case 2:
      display.setCursor(10,f);
      display.print("Seg");
      break;
      case 3:
      display.setCursor(10,f);
      display.print("Ter");
      break;
      case 4:
      display.setCursor(10,f);
      display.print("Qua");
      break;
      case 5:
      display.setCursor(10,f);
      display.print("Qui");
      break;
      case 6:
      display.setCursor(10,f);
      display.print("Sex");
      break;
      case 7:
      display.setCursor(10,f);
      display.print("Sab");
      break;
    }
switch(w)
    {
      case 1:
      display.setCursor(60,f);
      display.print("Jan");
      break;
      case 2:
      display.setCursor(60,f);
      display.print("Fev");
      break;
      case 3:
      display.setCursor(60,f);
      display.print("Mar");
      break;
      case 4:
      display.setCursor(60,f);
      display.print("Abr");
      break;
      case 5:
      display.setCursor(60,f);
      display.print("Mai");
      break;
      case 6:
      display.setCursor(60,f);
      display.print("Jun");
      break;
      case 7:
      display.setCursor(60,f);
      display.print("Jul");
      break;
      case 8:
      display.setCursor(60,f);
      display.print("Ago");
      break;
      case 9:
      display.setCursor(60,f);
      display.print("Set");
      break;
      case 10:
      display.setCursor(60,f);
      display.print("Out");
      break;
      case 11:
      display.setCursor(60,f);
      display.print("Nov");
      break;
      case 12:
      display.setCursor(60,f);
      display.print("Dez");
      break;

    }

}
  

void bateria(int x , int y)
{
  
 if(readVcc() >= 3700 ){
 display.drawBitmap(x, y,  battIconFull, 16, 8, 1);
  }  
  
  if(readVcc() >= 3500 && readVcc() < 3700 ){
  display.drawBitmap(x, y,  battIconHigh, 16, 8, 1);
  }  
  
    if(readVcc() >= 3300 && readVcc() < 3500 ){
 display.drawBitmap(x, y,  battIconLow, 16, 8, 1);
  }
  
     if( readVcc() < 3300 ){
 display.drawBitmap(x, y,  battIconEmpty, 16, 8, 1);
  }
  
}

void beep(unsigned char delayms, int x){
  analogWrite(buzer, x);      // Almost any value can be used except 0 and 255
  // experiment to get the best tone
  delay(delayms);          // wait for a delayms ms
  analogWrite(buzer, 0);       // 0 turns it off
  delay(delayms);          // wait for a delayms ms   
}

long readVcc() //***************************** Leitura da tensão  capatado da internet ***************
{ 
  long result;
  // Read 1.1V reference against AVcc
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Convert
  while (bit_is_set(ADCSRA, ADSC));
  result = ADCL;
  result |= ADCH << 8;
  result = 1126400L / result; // Back-calculate AVcc in mV
  return result;
}


double GetTemp(void)
{
  unsigned int valorADC;
  double temperatura;
 
  ADMUX = (_BV(REFS1) | _BV(REFS0) | _BV(MUX3)); //seleciona tensão interna de 1,1 V e canal 8
  ADCSRA |= _BV(ADEN);  // habilita o conversor AD
 
  delay(20);            // aguarda estabilização da tensão
 
  ADCSRA |= _BV(ADSC);  // inicia a conversão
 
  //aguarda o fim da conversão
  while (bit_is_set(ADCSRA,ADSC));
 
  // le o valor da conversão 
  valorADC = ADCW;
 
  // calcula o valor da temperatura 
 temperatura = (valorADC - 324.31 ) / 1.22;
 
  // retorna o valor da temperatura em °C
  return (temperatura);
}

ISR(INT1_vect) { } // Vibration switch wakeup interrupt
