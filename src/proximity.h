//
// Created by wouter on 17-11-17.
//

#ifndef ADDOG_PROXIMITY_H
#define ADDOG_PROXIMITY_H

#include <Arduino.h>
#include <ADCTouch.h>

#define PROXIMITY_MEASUREMENTS 5
#define PROXIMITY_DEADZONE 2
#define PROXIMITY_TOUCHED 35

void SetupProximity();
void UpdateProximity();
int8_t ReadProximity();
int8_t AverageProximity();

#endif //ADDOG_PROXIMITY_H
