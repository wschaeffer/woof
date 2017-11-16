//
// Created by wouter on 10-11-17.
//

#include "LEDstrip.h"
#include <SPI.h>

LEDstrip::LEDstrip( uint8_t dataPin, uint8_t clockPin, uint8_t ledCount )
: dataPin( dataPin ), clockPin( clockPin ), ledCount( ledCount )
{
    colors = ( rgb_color* ) malloc( sizeof( rgb_color ) * ledCount );
    SPI.begin();
}

void LEDstrip::SetColor( rgb_color color, uint8_t position )
{
    colors[position] = color;
}

void LEDstrip::SetColor( rgb_color color, uint8_t brightness, uint8_t position )
{
    color.brightness = brightness;
    colors[position] = color;
}

void LEDstrip::Write()
{
    startFrame();
    for ( uint8_t i = 0; i < ledCount; i++ )
    {
        SPI.transfer( 0b11100000 | colors[i].brightness );
        SPI.transfer( colors[i].blue );
        SPI.transfer( colors[i].green );
        SPI.transfer( colors[i].red );
    }
    endFrame();
    clearColors();
}

void LEDstrip::clearColors()
{
    rgb_color color = {};

    for ( uint8_t i = 0; i < ledCount; i++ )
    {
        colors[i] = color;
    }
}

void LEDstrip::startFrame()
{
    for ( uint8_t i = 0; i < 4; i++ )
    {
        SPI.transfer( 0 );
    }
}

void LEDstrip::endFrame()
{
    SPI.transfer( 0xFF );
    for ( uint8_t i = 0; i < 5 + ledCount / 16; i++ )
    {
        SPI.transfer( 0 );
    }
}
