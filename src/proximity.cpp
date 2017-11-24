//
// Created by wouter on 17-11-17.
//

#include "proximity.h"

int16_t refA0                                   = 0;
int8_t  proximityValues[PROXIMITY_MEASUREMENTS] = {};
uint8_t proximityCounter                        = 0;

void SetupProximity()
{
    refA0 = ( int16_t ) ADCTouch.read( A0, 1000 );
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

int8_t ReadProximity()
{
    int proximity = ADCTouch.read( A0 ) - refA0;
    return proximity;
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