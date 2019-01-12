#include <Arduino.h>
#include <Wire.h>         //liaison I2C pour les cartes
#include <FastCRC.h>      //gestion d'erreur sur bus I2C
#include <AccelStepper.h> //un moteur pas a pas
#include <MultiStepper.h> //getion simultaner de moteur pas a pas limieter

#define my_adr 10         //mon adresse bus I2C







void receiveEvent(int howMany);
void requestEvent();
