//
// Created by wouter on 31-10-17.
//

#include <Arduino.h>
#include "LEDstrip.h"
#include <ADCTouch.h>

#define PIN_DATA 11
#define PIN_CLOCK 12
#define LEDCOUNT 10

#define PROXIMITY_MEASUREMENTS 10
#define PROXIMITY_DEADZONE 2
#define PROXIMITY_TOUCHED 25

bool      touched        = false;
uint8_t   position       = 0;
uint8_t   positionTarget = 0;
int8_t    maxLed         = 0;
int8_t    proximity      = 0;
uint16_t  speed          = 0;
rgb_color colorOrange    = {255, 25, 0, 1};
LEDstrip  led            = LEDstrip( PIN_DATA, PIN_CLOCK, LEDCOUNT );

int16_t refA0                                   = 0;
int8_t  proximityValues[PROXIMITY_MEASUREMENTS] = {};
uint8_t proximityCounter                        = 0;

int8_t ReadProximity();
void UpdateProximity();
int8_t AverageProximity();

void setup()
{
    Serial.begin( 9600 );
    randomSeed( ( unsigned long ) analogRead( 0 ) );
    refA0 = ( int16_t ) ADCTouch.read( A0 );
}

void loop()
{
    UpdateProximity();

    if ( position == positionTarget )
    {
        proximity = AverageProximity();
        Serial.println( proximity );
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

int8_t ReadProximity()
{
    int proximity = ADCTouch.read( A0 ) - refA0;
    return proximity;
}

void UpdateProximity()
{
    proximityValues[proximityCounter] = ReadProximity();

    proximityCounter++;
    if ( proximityCounter == PROXIMITY_MEASUREMENTS )
    {
        proximityCounter = 0;
    }
}

int8_t AverageProximity()
{
    int16_t counter = 0;

    for ( uint8_t i = 0; i < PROXIMITY_MEASUREMENTS; i++ )
    {
        counter += proximityValues[i];
    }
    return counter / PROXIMITY_MEASUREMENTS;
}