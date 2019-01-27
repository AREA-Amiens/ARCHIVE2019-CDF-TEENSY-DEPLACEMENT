#include "deplacement.h"

//deplacement scotch
AccelStepper motor_G(1, step_G, dir_G);
AccelStepper motor_D(1, step_D, dir_D);

byte t[5];
byte com,etat=0x00,etatp=0;

void setup() {
  byte i;
  Wire.begin(my_adr);
  Serial.begin(9600);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
  Serial.println("coucou");

  //initialisation du moteur gauche
  pinMode(reset_G, OUTPUT);    //le resete se fait a l'aita bas
  digitalWrite(reset_G, HIGH);
  pinMode(sleep_G, OUTPUT);    //le sleep se met a l'aita bas poru une carte fonctionelle
  digitalWrite(sleep_G, HIGH);

  motor_G.setSpeed(100);
  motor_G.setAcceleration(100);


  //initialisation du moteur doit
  pinMode(reset_D, OUTPUT);    //le resete se fait a l'aita bas
  digitalWrite(reset_D, HIGH);
  pinMode(sleep_D, OUTPUT);    //le sleep se met a l'aita bas poru une carte fonctionelle
  digitalWrite(sleep_D, HIGH);

  motor_D.setSpeed(100);
  motor_D.setAcceleration(100);

//  motor_G.moveTo(13263);
//  motor_D.moveTo(13263);


  for(i=0;i<4;i++)t[i]=0;
  etat=1;
}
//motor_G.moveTo(-500);
//motor_D.moveTo(500);
void loop() {

  //mise a jour capteur
  //mise a jour etat

  //mise a jour actionneur
  switch (etat){
    case 1://antante de com

    if (com==1){
      com=0;
      if(t[0]|=(1<<1)){
        etat=2;
        Serial.println("la");
      }else if(t[0]|=(1<<3))etat=3;
      else if(t[0]|=(1<<5))etat=4;
    }
    break;

    case 2://turn
    Serial.println("2");
      motor_D.move(t[1]*36);
      motor_G.move(t[1]*36);
      etat=5;
      etatp=2;
    break;

    case 3:
      Serial.println((t[3]+(t[2]<<8)));
      motor_D.move((t[3]+(t[2]<<8))*-8);
      motor_G.move((t[3]+(t[2]<<8))*8);
      etat=5;
      etatp=3;
    break;

    case 4:
    Serial.println("4");
      motor_D.move(t[4]*36);
      motor_G.move(t[4]*36);
      etat=5;
      etatp=4;
    break;
    case 5://attante de fin adr_deplacement
      if(motor_D.isRunning()==false & motor_G.isRunning()==false){
        Serial.println("etat");
        Serial.println(etat);
        Serial.println(etatp);
        if(etatp==2)etat=3;
        else if(etatp==3)etat=4;
        else if(etatp==4)etat=1;
      }
    break;
  }


  motor_D.run();
  motor_G.run();
  //mise a jour afficheur



  //(t[1]*36,8421030522)
}
void receiveEvent(int howMany){
  byte i;
  for(i=0;i<howMany;i++)t[i]=Wire.read();
  com=1;
  Serial.println(com);
}

void requestEvent(){
  Wire.write('a');
}
