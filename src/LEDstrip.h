//
// Created by wouter on 10-11-17.
//

#ifndef ADDOG_LEDSTRIP_H
#define ADDOG_LEDSTRIP_H

#include <Arduino.h>

typedef struct rgb_color
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t brightness;
} rgb_color;

class LEDstrip
{
public:
    LEDstrip( uint8_t dataPin, uint8_t clockPin, uint8_t ledCount );
    void SetColor( rgb_color color, uint8_t position );
    void SetColor( rgb_color color, uint8_t brightness, uint8_t position );
    void Write();
private:
    uint8_t dataPin;
    uint8_t clockPin;
    uint8_t ledCount;
    rgb_color* colors;

    void startFrame();
    void endFrame();
    void clearColors();
    void init();
    void transfer( uint8_t b );
    void toggleClock();
};

#endif //ADDOG_LEDSTRIP_H