//
// Created by wouter on 31-10-17.
//

#include <Arduino.h>
#include "LEDstrip.h"
#include "proximity.h"

#define PIN_DATA 11
#define PIN_CLOCK 12
#define LEDCOUNT 10
#define MIN_BRIGHTNESS 10
#define MAX_BRIGHTNESS 20
#define MAX_DELAY 100
#define MIN_DELAY 1
#define TIME_ACTIVE 5

bool     touched        = false;
int8_t   proximity      = 0;
uint16_t delayms        = 100;
long     touchStartTime = 0;

rgb_color colorOrange = {255, 25, 0, 1};
LEDstrip  led         = LEDstrip( PIN_DATA, PIN_CLOCK, LEDCOUNT );

void HandleSateBreathing();
void HandleStateActive();

void setup()
{
    randomSeed( ( unsigned long ) analogRead( 0 ) );
    SetupProximity();
}

void loop()
{
    // Get proximity
    UpdateProximity();
    proximity = GetProximity();
    delayms   = ( uint8_t ) map( proximity, 0, PROXIMITY_TOUCHED, MAX_DELAY, MIN_DELAY );

    // Control out of range
    if ( proximity < PROXIMITY_DEADZONE )
    {
        proximity = 0;
    }
    else if ( proximity >= PROXIMITY_TOUCHED )
    {
        proximity = PROXIMITY_TOUCHED;
        if ( !touched )
        {
            touched        = true;
            touchStartTime = millis();
        }
    }
    else
    {
        touched = false;
    }

    // Set sate
    if ( touched && millis() - touchStartTime < TIME_ACTIVE * 1000 )
    {
        HandleStateActive();
    }
    else
    {
        HandleSateBreathing();
    }

    // Update ledstrip
    led.Write();
    delay( delayms );
}

void HandleSateBreathing()
{
    static bool      ledsFalling[LEDCOUNT]   = {};
    static uint8_t   ledsOn                  = 0;
    static uint8_t   maxBrightness[LEDCOUNT] = {};
    static rgb_color leds[LEDCOUNT]          = {};

    uint8_t ledTarget = uint8_t( random( 0, LEDCOUNT ) );
    uint8_t maxLed    = ( uint8_t ) map( proximity, 0, PROXIMITY_TOUCHED * 0.5, 1, LEDCOUNT );

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
                maxBrightness[i] = uint8_t( random( MIN_BRIGHTNESS, MAX_BRIGHTNESS ) );
            }
        }
    }
    led.SetColor( leds, LEDCOUNT );
}

void HandleStateActive()
{
    static uint8_t delayLed       = 0;
    static uint8_t position       = 0;
    static uint8_t positionTarget = 0;

    if ( position == positionTarget )
    {
        positionTarget = uint8_t( random( 1, LEDCOUNT ) );
        delayLed       = ( uint8_t ) random( 0, 10 );
        delay( delayLed * 2 );
    }

    position < positionTarget ? ( position++ ) : ( position-- );

    led.SetColor( colorOrange, MAX_BRIGHTNESS, position );

    delay( delayLed );
}