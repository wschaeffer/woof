//
// Created by wouter on 31-10-17.
//

#include <Arduino.h>
#include <APA102.h>
#include <ADCTouch.h>

// Define which pins to use.
const uint8_t dataPin  = 11;
const uint8_t clockPin = 12;

// Create an object for writing to the LED strip.
APA102<dataPin, clockPin> ledStrip;

// Set the number of LEDs to control.
const uint8_t ledCount = 10;
int ref = 0;

void writePosition( int8_t pos );

void setup()
{
    randomSeed( analogRead( 0 ) );
    pinMode(13, OUTPUT);
    ref = ADCTouch.read(A0);
}

void loop()
{
    static uint8_t  position       = 0;
    static uint8_t  positionTarget = 0;
    static uint16_t speed          = 0;

    if ( position == positionTarget )
    {
        positionTarget = uint8_t( random( 1, 10 ) );
        //speed          = uint8_t( random( 1, 50 ) );
        speed          = analogRead( 0 ) / ( 1023 / 50 );
    }

    if ( position < positionTarget )
    {
        position++;
    }
    else
    {
        position--;
    }

    digitalWrite(13, ADCTouch.read(A0) - ref > 1);

    writePosition( position );

    delay( speed );
}

void writePosition( int8_t pos )
{
    ledStrip.startFrame();
    for ( uint8_t i = 0; i < ledCount; i++ )
    {
        ledStrip.sendColor( 255, 25, 0, uint8_t( i == pos ) );
    }
    ledStrip.endFrame( ledCount );
}