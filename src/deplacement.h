#include <Arduino.h>
#include <Wire.h>         //liaison I2C pour les cartes
#include <FastCRC.h>      //gestion d'erreur sur bus I2C
#include <AccelStepper.h> //un moteur pas a pas
        //AccelStepper motor_left(1, step_left, dir_left); definie la comme avec le diver
        //
#include <MultiStepper.h> //getion simultaner de moteur pas a pas limieter

#define my_adr 10         //mon adresse bus I2C

#define reset_G   13 // pin du moteur gauche
#define sleep_G   15
#define step_G    0
#define dir_G     1

#define reset_D   9// pin du moteur droit
#define sleep_D   10
#define step_D    11
#define dir_D     12


#define acceleration  100
#define speed         200



void receiveEvent(int howMany);
void requestEvent();
void lancer_les_moteur(byte t[5],byte etat);
