#ifndef SENSOR_H
#define SENSOR_H

#include <Arduino.h>
#include <avr/io.h>

void initSensor();
void sensorOff();
void sensorOn();
void trigger();

#endif
