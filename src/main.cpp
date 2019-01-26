#include "deplacement.h"

//deplacement scotch
AccelStepper motor_G(1, step_G, dir_G);
AccelStepper motor_D(1, step_D, dir_D);

int t[3],com=0;
int vmd,vmg;
void setup() {
  char i;
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

  motor_G.moveTo(13263);
  motor_D.moveTo(13263);


  for(i=0;i<3;i++)t[i]=0;
}
//motor_G.moveTo(-500);
//motor_D.moveTo(500);
//motor_G.run();
//motor_D.run();
void loop() {
  char i;
  motor_D.run();
  motor_G.run();


  for(i=0;i<3;i++)Serial.println(t[i]);
  delay(10000);
}
void receiveEvent(int howMany){
  if(com>3)com=0;
  t[com]=Wire.read();
  com++;
  //char res;
  //res=Wire.read();
}
void requestEvent(){
  Wire.write('a');
}
