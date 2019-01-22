#include "deplacement.h"

//deplacement scotch
AccelStepper motor_left(1, step_left, dir_left);


void setup() {
//  Wire.begin(my_adr);
//  Serial.begin(9600);
//  Wire.onReceive(receiveEvent);
//  Wire.onRequest(requestEvent);
//  Serial.println("coucou");

  pinMode(reset_left, OUTPUT);
  digitalWrite(reset_left, HIGH);
  pinMode(sleep_left, OUTPUT);
  digitalWrite(sleep_left, HIGH);

  motor_left.setSpeed(100);
  motor_left.setAcceleration(100);
  motor_left.moveTo(32000);
}

void loop() {
  motor_left.run();
}
void receiveEvent(int howMany){
  Serial.println("receivedl");
  Serial.println(Wire.read());
}
void requestEvent(){
  Wire.write('a');
}
