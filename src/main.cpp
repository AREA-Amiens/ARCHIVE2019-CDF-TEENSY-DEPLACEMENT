#include "deplacement.h"

//deplacement scotch

void setup() {
  Wire.begin(my_adr);
  Serial.begin(9600);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
  Serial.println("coucou");
}

void loop() {
  delay(1000);

}
void receiveEvent(int howMany){
  Serial.println("received");
    Serial.println(Wire.read());
}
void requestEvent(){
  Wire.write('a');
}
