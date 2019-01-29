#include "deplacement.h"

//deplacement scotch
AccelStepper motor_G(1, step_G, dir_G);//declaration du moteur gauche
AccelStepper motor_D(1, step_D, dir_D);//declatation du moteur droit

byte reception_trame[5],com,etat=0,etatp=0;
//com pout savoir si une nouvelle comunication est posible avec le métre
//etat variable de la machine d'état +mise a l'état 0 voir sur la machine d'état
//etatp variable donnant l'état précédant

//le setup ne sexcute qune seul foi lor du demarage de la teensi
void setup() {
  byte i;//variable pour le for
  Serial.begin(9600);
  //lancemant dune liason serie pour comuniquer avec l'ordinateur sutilise Serial.prinln(valeur ou variable)
  Wire.begin(my_adr);//lancemenant de la liason i²c avec son adresse d'esclave
  Wire.onReceive(receiveEvent);//donne la fonction a ouvrire l'or de la réception de dune trasmition
  Wire.onRequest(requestEvent);//va envoyer se qui se trouve dans la fonciton

  //initialisation du moteur gauche
  pinMode(reset_G, OUTPUT);    //le resete se fait a l'aita bas
  digitalWrite(reset_G, HIGH);
  pinMode(sleep_G, OUTPUT);    //le sleep se met a l'aita bas poru une carte fonctionelle
  digitalWrite(sleep_G, HIGH);

  motor_G.setSpeed(speed);
  motor_G.setAcceleration(acceleration);

  //initialisation du moteur doit
  pinMode(reset_D, OUTPUT);    //le resete se fait a l'aita bas
  digitalWrite(reset_D, HIGH);
  pinMode(sleep_D, OUTPUT);    //le sleep se met a l'aita bas poru une carte fonctionelle
  digitalWrite(sleep_D, HIGH);

  motor_D.setSpeed(speed);
  motor_D.setAcceleration(acceleration);



  for(i=0;i<4;i++)reception_trame[i]=0;//ramplit de zero le tableau de réception
  etat=1;//passage a l'état 1 voir la machine d'état
}
void loop() {
  byte b,c,d,e,f,g,h,j,k,l;//byte utiliser pour la mémoire de de la trame t[0]
  long dist;//varible coréspondant a la distance a par courir elle ne comprote pas de sance
  int sense;//sense variable relative au sance de la rotation ou de la direction du go
  //mise a jour capteur
  //mise a jour etat
  //mise a jour actionneur
  switch (etat){//permet de réaliser les différent etat de la machinne d'etat
    case 1://etat 1 (antante de com)
    if (com==1){//si nous avons communication alors on redirige vert les diférant état
      l=reception_trame[0];//stocage de la trame 0
      j=reception_trame[0];//stocage de la trame 0
      k=reception_trame[0];//stocage de la trame 0
      if((l&=1<<1)==(1<<1))etat=2;//passage a l'état2 si il et activer dans la trame
      else if((j&=1<<3)==(1<<3))etat=3;//passage a l'état3 si l'état 2 n est pas activer et l'état 3 est activer dans la trame
      else if((k&=1<<5)==(1<<5))etat=4;//passage a l'état 4 si l'état 3 et 2 n est pas activet et l'état 4 est activer dans la trame
    }
    break;

    case 2://etat 2 (turn1)
      b=reception_trame[0];//stocage de la trame 0
      dist=(long)(coeficien_turn*(float)reception_trame[1]);//calcule du nombre de pas pour les roue sans le signe de la direction
      if ((b&=1<<0)==1<<0)sense=-1;//teste pour conaitre le sance de la rotation
      else sense=1;
      motor_D.move(dist*sense);//activation de la rotation jusque cette valeur de pas moteur droite
      motor_G.move(dist*sense);//activation de la rotation jusque cette valeur de pas moteur gauche
      etat=5;//passe a l etat 5
      etatp=2;//passe l'étatpe précédante a 2
    break;

    case 3://etat 3 (go)
      b=reception_trame[0];//stocage de la trame 0
      dist=(long)(coeficien_go*(float)(reception_trame[3]+(reception_trame[2]<<8)));//calcule du nombre de pas pour les roue sans le signe de la direction
      if ((b&=1<<2)==1<<2)sense=-1;//teste pour conaitre le sance de la translation
      else sense=1;
      motor_D.move(dist*sense);//activation de la rotation jusque cette valeur de pas moteur droite
      motor_G.move(dist*(-sense));//activation de la rotation jusque cette valeur de pas moteur gauche
      etat=5;//passe a l etat 5
      etatp=3;//passe l'étatpe précédante a 3
    break;

    case 4://etat 4 (turn)
      b=reception_trame[0];//stocage de la trame 0
      dist=(long)(coeficien_turn*(float)reception_trame[4]);//calcule du nombre de pas pour les roue sans le signe de la direction
      if ((b&=1<<4)==1<<4)sense=-1;//teste pour conaitre le sance de la translation
      else sense=1;
      motor_D.move(dist*sense);//activation de la rotation jusque cette valeur de pas moteur droite
      motor_G.move(dist*sense);//activation de la rotation jusque cette valeur de pas moteur gauche
      etat=5;//passe a l etat 5
      etatp=4;//passe l'étatpe précédante a 4
    break;
    case 5://attante de fin adr_deplacement
      if(motor_D.isRunning()==false && motor_G.isRunning()==false){//teste des morteur a l'arrée pour pacer au d'éplacemnt suivant
        c=reception_trame[0];//stocage de la trame 0
        d=reception_trame[0];//stocage de la trame 0
        e=reception_trame[0];//stocage de la trame 0
        f=reception_trame[0];//stocage de la trame 0
        g=reception_trame[0];//stocage de la trame 0
        h=reception_trame[0];//stocage de la trame 0
        if(etatp==2 && ((c&=1<<3)==(1<<3)))etat=3;//passe a l'état 3 si état précédant etait 2 et si le go est activer
        else if (etatp==2 && ((d&=1<<5)==(1<<5)))etat=4;//si non passe a l état 4 si etat précédan etait 2 et si turn 2 est activer
        else if (etatp==3 && ((e&=1<<5)==(1<<5)))etat=4;//si non passe a l'état 5 si etat précédan etait 3 et si turn 2 est activer
        else if (etatp==4){//si non passe a l'état 1 si on vient de l'etat 4
          etat=1;//passe a l'état 1
          com=0;// passe a com a 0 car fin du dépalcment
        }
        else if (etatp==3&&((f&1<<5)==0)){//si non passe passe a l'état 1 si pas de turn 2
          etat=1;//passe a l'état 1
          com=0;// passe a com a 0 car fin du dépalcment
        }
        else if (etatp==2&&((g&1<<3)==0)&&((h&1<<5)==0)){//si non passe a l'état 1 si pas de turn 2 et de go
          etat=1;//passe a l'état 1
          com=0;// passe a com a 0 car fin du dépalcment
        }
      }
    break;
  }

  motor_D.run();//lancemant du moteur droit
  motor_G.run();//lancemant du moteur gauche
  //mise a jour afficheur
}
void receiveEvent(int howMany){//fonction d'intérupetion l'or dun envoi du maitre
  byte i;//variable pour le for
  for(i=0;i<howMany;i++)reception_trame[i]=Wire.read();//rampli le tableau si avec les valeur de la transmition
  com=1;// passe la comme a 1 pour l'éxecution de la trame en cour
}

void requestEvent(){//fonciton d'intérupetion l'or d une deamande de trame
  Wire.write(com);//le maitre lira la valeur de com
}
