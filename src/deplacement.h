#include <Arduino.h>
#include <Wire.h>         //liaison I2C pour les cartes
#include <FastCRC.h>      //gestion d'erreur sur bus I2C
#include <AccelStepper.h> //un moteur pas a pas
        //AccelStepper motor_left(1, step_left, dir_left); definie la comme avec le diver
        //
#include <MultiStepper.h> //getion simultaner de moteur pas a pas limieter
#include <math.h>//pour les calcule

#define my_adr 10         //mon adresse bus I2C

#define reset_G   13 // pin du moteur gauche
#define sleep_G   15
#define step_G    0
#define dir_G     1

#define reset_D   9// pin du moteur droit
#define sleep_D   10
#define step_D    11
#define dir_D     12


#define acceleration  335//acelearion du robo x 2coéficien_go
#define speed         100

#define coeficien_turn 17.81426307//coéficiant determiner par rapore au diamétre de roue, sélection du pas(ici 1/4),le tout pour 1°
#define coeficien_go   7.520210565//coéficiant determiner par rapore au diamétre de roue, sélection du pas(ici 1/4),le tout pour 1mm

#define pi 3.1415926536//

void receiveEvent(int howMany);
//fonction pour quan l'ésclave resoi une trame de adr_actionneur
//met la trame dans un tableau


void requestEvent();
//fonction pour quan le maitre appelle l'esclave
//ranvoi si l'esclave et prés a recevoire une trame(déplacment fini)

byte iso_bite(byte analiser, byte decalage);
//vas isoler le bite du decalge
//analise = xxxx xxxx
//dealage = 2
//return = 0000 00x00
