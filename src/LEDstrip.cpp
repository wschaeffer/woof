//
// Created by wouter on 10-11-17.
//

#include "LEDstrip.h"

LEDstrip::LEDstrip( uint8_t dataPin, uint8_t clockPin, uint8_t ledCount )
: dataPin( dataPin ), clockPin( clockPin ), ledCount( ledCount )
{
    colors = ( rgb_color* ) malloc( sizeof( rgb_color ) * ledCount );
    init();
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
        transfer( 0b11100000 | colors[i].brightness );
        transfer( colors[i].blue );
        transfer( colors[i].green );
        transfer( colors[i].red );
    }
    endFrame();
    clearColors();
}

void LEDstrip::clearColors()
{
    rgb_color     color = {};
    for ( uint8_t i     = 0; i < ledCount; i++ )
    {
        colors[i] = color;
    }
}

void LEDstrip::startFrame()
{
    transfer( 0 );
    transfer( 0 );
    transfer( 0 );
    transfer( 0 );
}

void LEDstrip::endFrame()
{
    transfer( 0xFF );
//    for ( uint8_t i = 0; i < 5 + ledCount / 16; i++ )
//    {
//        transfer( 0 );
//    }
}

void LEDstrip::init()
{
    digitalWrite( dataPin, LOW );
    pinMode( dataPin, OUTPUT );
    digitalWrite( clockPin, LOW );
    pinMode( clockPin, OUTPUT );
}

void LEDstrip::transfer( uint8_t b )
{
    digitalWrite( dataPin, b >> 7 & 1 );
    toggleClock();
    digitalWrite( dataPin, b >> 6 & 1 );
    toggleClock();
    digitalWrite( dataPin, b >> 5 & 1 );
    toggleClock();
    digitalWrite( dataPin, b >> 4 & 1 );
    toggleClock();
    digitalWrite( dataPin, b >> 3 & 1 );
    toggleClock();
    digitalWrite( dataPin, b >> 2 & 1 );
    toggleClock();
    digitalWrite( dataPin, b >> 1 & 1 );
    toggleClock();
    digitalWrite( dataPin, b >> 0 & 1 );
    toggleClock();
}

void LEDstrip::toggleClock()
{
    digitalWrite( clockPin, HIGH );
    digitalWrite( clockPin, LOW );
}
