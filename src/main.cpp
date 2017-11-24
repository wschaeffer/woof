//
// Created by wouter on 31-10-17.
//

#include <Arduino.h>
#include "LEDstrip.h"
#include "proximity.h"

#define PIN_DATA 11
#define PIN_CLOCK 12
#define LEDCOUNT 10
#define MAX_BRIGHTNESS 20

bool touched             = false;
int8_t    maxLed         = 4;
int8_t    proximity      = 0;
uint8_t   position       = 0;
uint8_t   positionTarget = 0;
uint16_t  speed          = 150;

bool      ledsFalling[LEDCOUNT]   = {};
uint8_t   ledsOn                  = 0;
uint8_t   maxBrightness[LEDCOUNT] = {};
rgb_color leds[LEDCOUNT]          = {};
rgb_color colorOrange             = {255, 25, 0, 1};
LEDstrip  led                     = LEDstrip( PIN_DATA, PIN_CLOCK, LEDCOUNT );

void handleBreathing();

void setup()
{
    randomSeed( ( unsigned long ) analogRead( 0 ) );
    SetupProximity();
}

void loop()
{
    UpdateProximity();

    if(pro){

    }
    handleBreathing();

    led.SetColor( leds, LEDCOUNT );
    led.Write();
    delay( speed );
}

void handleBreathing()
{
    uint8_t ledTarget = uint8_t( random( 0, LEDCOUNT ) );

    for ( uint8_t i = 0; i < LEDCOUNT; i++ )
    {
        if ( leds[i].brightness > 0 )
        {
            if ( ledsFalling[i] )
            {
                leds[i].brightness--;
                if ( leds[i].brightness == 0 )
                {
                    ledsFalling[i] = false;
                    leds[i]        = {};
                    ledsOn--;
                }
            }
            else
            {
                leds[i].brightness++;
                if ( leds[i].brightness == maxBrightness[i] )
                {
                    ledsFalling[i] = true;
                }
            }

        }
        else if ( ledTarget == i )
        {
            if ( ledsOn < maxLed )
            {
                ledsOn++;
                leds[i]          = colorOrange;
                maxBrightness[i] = uint8_t( random( 10, MAX_BRIGHTNESS ) );
            }
        }
    }
}