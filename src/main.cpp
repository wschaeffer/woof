//
// Created by wouter on 31-10-17.
//

#include <Arduino.h>
#include "LEDstrip.h"
#include <ADCTouch.h>

#define PIN_DATA 11
#define PIN_CLOCK 12
#define LEDCOUNT 10

#define PROXIMITY_DEADZONE 3
#define PROXIMITY_TOUCHED 30

bool      touched        = false;
uint8_t   position       = 0;
uint8_t   positionTarget = 0;
uint8_t   maxLed         = 0;
uint8_t   proximity      = 0;
uint16_t  speed          = 0;
uint16_t  refA0          = 0;
rgb_color colorOrange    = {255, 25, 0, 1};
LEDstrip  led            = LEDstrip( PIN_DATA, PIN_CLOCK, LEDCOUNT );

void setup()
{
    randomSeed( ( unsigned long ) analogRead( 0 ) );
    refA0 = ( uint16_t ) ADCTouch.read( A0 );
}

void loop()
{
    if ( position == positionTarget )
    {
        proximity = ADCTouch.read( A0 ) - refA0;
        if ( proximity < PROXIMITY_DEADZONE )
        {
            proximity = 0;
        }
        else if ( proximity > PROXIMITY_TOUCHED )
        {
            proximity = PROXIMITY_TOUCHED;
        }

        touched = proximity > PROXIMITY_TOUCHED;

        maxLed         = ( PROXIMITY_TOUCHED - proximity ) / ( PROXIMITY_TOUCHED / LEDCOUNT );
        positionTarget = uint8_t( random( 1, maxLed ) );
        speed          = ( uint16_t ) analogRead( 0 ) / ( 1023 / 50 );
    }

    position < positionTarget ? ( position++ ) : ( position-- );

    if ( proximity )
    {
        for ( uint8_t i = 0; i < maxLed; i++ )
        {
            led.SetColor( colorOrange, i );
            if ( i == position )
            {
                led.SetColor( colorOrange, 2, i );
            }
        }
    }
    else
    {
        led.SetColor( colorOrange, position );
    }

    led.Write();
    delay( speed );
}