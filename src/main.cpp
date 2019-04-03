#include "deplacement.h"
//v2
//deplacement scotch
AccelStepper motor_G(1, step_G, dir_G);//declaration du moteur gauche
AccelStepper motor_D(1, step_D, dir_D);//declatation du moteur droit

byte mouvement,recepetion_tram[4],tableau_dep[5],com,etat=0,etatp=0,avar=0;
int xp=750,yp=300,ap=0;
int go=0, turndepart=0,turndepartp=0,turnarrive=0,turnarrivep=0,turnar,turnarp,turnactu=0;
pos renvoie;

//com pout savoir si une nouvelle comunication est posible avec le métre
//etat variable de la machine d'état +mise a l'état 0 voir sur la machine d'état
//etatp variable donnant l'état précédant



//le setup ne sexcute qune seul foi lor du demarage de la teensi
void setup(){
  byte i;//variable pour le for
<<<<<<< HEAD
  renvoie.x=750;
  renvoie.y=2700;
=======
  //cotée rouge
//  a.x=750;
//  a.y=300;
//cotée bleu
  a.x=750;
  a.y=2700;
>>>>>>> bda7aa49729a0443804657aa8814fef07c5c0f78
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



  for(i=0;i<3;i++)recepetion_tram[i]=0;//ramplit de zero le tableau de réception
  etat=1;//passage a l'état 1 voir la machine d'état
}
void loop() {
  //byte utiliser pour la mémoire de de la trame t[0]
  //long dist;
  long pas;//varible coréspondant a la distance a par courir elle ne comprote pas de sance
  int sense;//sense variable relative au sance de la rotation ou de la direction du go
  //byte i;
  byte e,f,g;
  //mise a jour capteur
  //mise a jour etat
  //mise a jour actionneur



  switch (etat){//permet de réaliser les différent etat de la machinne d'etat
    case 1://etat 1 (antante de com)
    if (com==1){//si nous avons communication alors on redirige vert les diférant état + analyser la trame

      mouvement=0;//mise a zero des diférante chose a fair et dans quelle sanse
      e=recepetion_tram[0];//mise en mémoire de la trame 0
      f=recepetion_tram[0];
      g=recepetion_tram[0];
      xp=renvoie.x;//remplisage de xp(positon actuelle de x)
      yp=renvoie.y;//remplisage de yp(positon actuelle de y)

      renvoie=position(renvoie);

      go = (int)sqrt((double)pow((renvoie.x-xp),2)+pow((renvoie.y-yp),2));//valeur du go dans le turn go turn
      if (go!=0)mouvement|=(1<<3);//dit pour la suitte qu il faut avancer
      turnar=(int)(recepetion_tram[3]+((f&=(0x80))<<1));//valuer de langle d'arriver
      //determination de l'angle du turn 1 pour rejoindre le nouvaux x et y
      if(go!=0)turndepart=360-(int)((float)atan((double)(((float)(renvoie.y-yp))/((float)(renvoie.x-xp))))*(float)180/pi)-turnactu;
      else turndepart=0;

      mouvement|=1<<5;//active l'étape de turn 2
      if((renvoie.y-yp)<0)turndepart=turndepart-360;//ajous d'angle du a la fonciton tnagnete
      if((renvoie.x-xp)<0)turndepart=turndepart+180;
      turndepart=turndepart%360;//pour suprimet les tour sur lui mêmme

      if(turndepart>180){//pour eviter de fair plus que 180 degrai
        turndepart=360-turndepart;
        mouvement|=1<<0;//activation du sence inverce de rotation
      }
      Serial.println(turndepart);

<<<<<<< HEAD
      if(turndepart>90 || turndepart<-90){ //changement d'angle pour passer en marche arriere
        if(turndepart>90){
=======

      if(turndepart>90){ //changement d'angle pour passer en marche arriere

>>>>>>> bda7aa49729a0443804657aa8814fef07c5c0f78
        turndepart=(180-turndepart)*-1;
        }
        else {
          if (turndepart<-90) {
            turndepart=(180+turndepart);
          }
        }
        go=go*-1;
        mouvement|=1<<0;
<<<<<<< HEAD
        Serial.println("ok");
        //Serial.println("ok")
=======
        avar=1;
>>>>>>> bda7aa49729a0443804657aa8814fef07c5c0f78
      }

      if(turndepart!=0)mouvement|=1<<1;//active l'étape de turn 1 + sence du turn

      if(iso_bite(mouvement,1)==(1<<1))etat=2;//passage a l'état2 si il et activer dans la trame
      else if(iso_bite(mouvement,3)==(1<<3))etat=3;//passage a l'état3 si l'état 2 n est pas activer et l'état 3 est activer dans la trame
      else etat=4;//passage a l'état 4 si l'état 3 et 2 n est pas activet et l'état 4 est activer dans la trame
    }
    break;

    case 2://etat 2 (turndepart)
      pas=(long)(coeficien_turn*(float)turndepart);//calcule du nombre de pas pour les roue sans le signe de la direction
      if (iso_bite(mouvement,0)==1<<0){sense=-1;turnactu=turnactu+turndepart;}//teste pour conaitre le sance de la rotation
      else {
        sense=1;turnactu=turnactu+turndepart;
        }
      motor_D.move(pas*sense);//activation de la rotation jusque cette valeur de pas moteur droite
      motor_G.move(pas*sense);//activation de la rotation jusque cette valeur de pas moteur gauche
      etat=5;//passe a l etat 5
      etatp=2;//passe l'étatpe précédante a 2
    break;

    case 3://etat 3 (go)
      pas=(long)(coeficien_go*(float)(go));//calcule du nombre de pas pour les roue sans le signe de la direction
      if (iso_bite(mouvement,2)==1<<2)sense=-1;//teste pour conaitre le sance de la translation
      else sense=1;
      motor_D.move(pas*sense);//activation de la rotation jusque cette valeur de pas moteur droite
      motor_G.move(pas*(-sense));//activation de la rotation jusque cette valeur de pas moteur gauche
      etat=5;//passe a l etat 5
      etatp=3;//passe l'étatpe précédante a 3
    break;

    case 4://etat 4 (turn)
      turnarrive=360-turnactu+turnar;
      turnarrive=turnarrive%360;
      if(turnarrive>180){//pour eviter de fair plus que 180 degrai
        turnarrive=360-turnarrive;
        mouvement|=1<<4;//activation du sence inverce de rotation
<<<<<<< HEAD
      }
      turnactu=turnarrive;

  /*    if(turndepart>90){ //changement d'angle pour passer en marche arriere
        turndepart=(180-turndepart)*-1;
        go=go*-1;
        mouvement|=1<<0;
      }*/
=======
        //turnactu=turnactu+180+turnarrive;
      }
      //else turnactu=turnactu-turnarrive;
      turnactu=turnar;
>>>>>>> bda7aa49729a0443804657aa8814fef07c5c0f78
      pas=(long)(coeficien_turn*(float)turnarrive);//calcule du nombre de pas pour les roue sans le signe de la direction
      if (iso_bite(mouvement,4)==1<<4)sense=-1;//turnactu=turnactu+180;}//teste pour conaitre le sance de la translation
      else sense=1;
      motor_D.move(pas*sense);//activation de la rotation jusque cette valeur de pas moteur droite
      motor_G.move(pas*sense);//activation de la rotation jusque cette valeur de pas moteur gauche
      etat=5;//passe a l etat 5
      etatp=4;//passe l'étatpe précédante a 4

      turnactu=turnactu%360;
    break;

    case 5://attante de fin adr_deplacement
      if(motor_D.isRunning()==false && motor_G.isRunning()==false){//teste des morteur a l'arrée pour pacer au d'éplacemnt suivant
        if(etatp==2 && (iso_bite(mouvement,3)==(1<<3)))etat=3;//passe a l'état 3 si état précédant etait 2 et si le go est activer
        else if (etatp==2 && (iso_bite(mouvement,5)==(1<<5)))etat=4;//si non passe a l état 4 si etat précédan etait 2 et si turn 2 est activer
        else if (etatp==3 && (iso_bite(mouvement,5)==(1<<5)))etat=4;//si non passe a l'état 5 si etat précédan etait 3 et si turn 2 est activer
        else if (etatp==4){//si non passe a l'état 1 si on vient de l'etat 4
          etat=1;//passe a l'état 1
          com=0;// passe a com a 0 car fin du dépalcment
        }
        else if (etatp==3&&(iso_bite(mouvement,5)==0)){//si non passe passe a l'état 1 si pas de turn 2
          etat=1;//passe a l'état 1
          com=0;// passe a com a 0 car fin du dépalcment
        }
        else if (etatp==2&&(iso_bite(mouvement,3)==0)&&(iso_bite(mouvement,5)==0)){//si non passe a l'état 1 si pas de turn 2 et de go
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
  for(i=0;i<howMany;i++)recepetion_tram[i]=Wire.read();//rampli le tableau si avec les valeur de la transmition
  com=1;// passe la comme a 1 pour l'éxecution de la trame en cour
}

void requestEvent(){//fonciton d'intérupetion l'or d une deamande de trame
  Wire.write(com);//le maitre lira la valeur de com
}

byte iso_bite(byte analiser, byte decalage){
  return (analiser &=1<<decalage);
}
pos position (pos renvoie){
  byte e,f;
  e=recepetion_tram[0];//mise en mémoire de la trame 0
  f=recepetion_tram[0];

  renvoie.x=(int)(recepetion_tram[1]+((e&=(0x70))<<4));//valeur des x et y à attindre
  renvoie.y=(int)(recepetion_tram[2]+((f&=(0x0F))<<8));

  return renvoie;
}
