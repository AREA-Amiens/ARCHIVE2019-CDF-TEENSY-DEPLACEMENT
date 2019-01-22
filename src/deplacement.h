#include <Arduino.h>
#include <Wire.h>         //liaison I2C pour les cartes
#include <FastCRC.h>      //gestion d'erreur sur bus I2C
#include <AccelStepper.h> //un moteur pas a pas
#include <MultiStepper.h> //getion simultaner de moteur pas a pas limieter

#define my_adr 10         //mon adresse bus I2C

#define reset_left  13
#define sleep_left  15
#define step_left   0
#define dir_left    1


#define acceleration  100
#define speed         200



void receiveEvent(int howMany);
void requestEvent();
