//
// Created by wouter on 31-10-17.
//

#include <Arduino.h>
#include <APA102.h>
#include <ADCTouch.h>

#define PROXIMITY_DEADZONE 2
#define PROXIMITY_TOUCHED 40

// Define which pins to use.
const uint8_t dataPin  = 11;
const uint8_t clockPin = 12;

// Create an object for writing to the LED strip.
APA102<dataPin, clockPin> ledStrip;

// Set the number of LEDs to control.
const uint8_t ledCount = 10;
uint16_t      refA0    = 0;

void writePosition( uint8_t pos );
void writePosition( uint8_t pos, uint8_t dimBegin, uint8_t dimEnd );

void setup()
{
    randomSeed( ( unsigned long ) analogRead( 0 ) );
    pinMode( 13, OUTPUT );
    refA0 = ( uint16_t ) ADCTouch.read( A0 );
}

void loop()
{
    static bool     touched        = false;
    static uint8_t  position       = 0;
    static uint8_t  positionTarget = 0;
    static uint8_t  maxLed         = 0;
    static uint8_t  proximity      = 0;
    static uint16_t speed          = 0;

    if ( position == positionTarget )
    {
        proximity = ADCTouch.read( A0 ) - refA0;
        if ( proximity < PROXIMITY_DEADZONE )
        {
            proximity = 0;
        }
        touched   = proximity > PROXIMITY_TOUCHED;
        digitalWrite( 13, ( uint8_t ) touched );

        maxLed         = ( PROXIMITY_TOUCHED - proximity ) / ( PROXIMITY_TOUCHED / ledCount );
        //positionTarget = uint8_t( random( 1, touched ? 3 : ledCount ) );
        positionTarget = uint8_t( random( 1, maxLed ) );
        speed          = ( uint16_t ) analogRead( 0 ) / ( 1023 / 50 );
    }

    if ( position < positionTarget )
    {
        position++;
    }
    else
    {
        position--;
    }

    if ( proximity )
    {
        writePosition( position, 0, maxLed );
    }
    else
    {
        writePosition( position );
    }

    delay( speed );
}

void writePosition( uint8_t pos )
{
    ledStrip.startFrame();
    for ( uint8_t i = 0; i < ledCount; i++ )
    {
        ledStrip.sendColor( 255, 25, 0, uint8_t( i == pos ) );
    }
    ledStrip.endFrame( ledCount );
}

void writePosition( uint8_t pos, uint8_t dimBegin, uint8_t dimEnd )
{
    ledStrip.startFrame();
    for ( uint8_t i = 0; i < ledCount; i++ )
    {
        if ( dimBegin <= i && i < dimEnd )
        {
            ledStrip.sendColor( 255, 25, 0, uint8_t( i == pos ) + 1 );
        }
        else
        {
            ledStrip.sendColor( 255, 25, 0, 0 );
        }

    }
    ledStrip.endFrame( ledCount );
}