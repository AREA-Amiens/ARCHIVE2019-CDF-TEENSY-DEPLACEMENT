#include <deplacement.h>
AccelStepper motor_G(1, step_G, dir_G);//declaration du moteur gauche
AccelStepper motor_D(1, step_D, dir_D);//declatation du moteur droit

Servo tour;

byte teste=0;
byte mouvement,recepetion_tram[5],tableau_dep[5],com=0,etat=0,etatp=0,avar=0,NBbyte=0;
int xp=750,yp=300,ap=0;
int go=0, turndepart=0,turndepartp=0,turnarrive=0,turnarrivep=0,turnar,turnarp,turnactu=0;
pos pos1;
pos pos1pre;
pos posdeb;
long posdetec;
int cons=0;
unsigned int temp_balise;
long pas;//varible coréspondant a la distance a par courir elle ne comprote pas de sance
int sense;//sense variable relative au sance de la rotation ou de la direction du go
//com pout savoir si une nouvelle comunication est posible avec le métre
//etat variable de la machine d'état +mise a l'état 0 voir sur la machine d'état
//etatp variable donnant l'état précédant




//le setup ne sexcute qune seul foi lor du demarage de la teensi
void setup(){
byte i,e,f,g;//variable pour le for

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

pinMode(RECALAGEAVANT, INPUT_PULLUP);//haut gauche
pinMode(RECALAGEARRIERE, INPUT_PULLUP);//haut droit

//initialisation CAPTEUR_AV et CAPTEUR_AR
pinMode(CAPTEUR_AV,INPUT_PULLUP);
pinMode(CAPTEUR_AR,INPUT_PULLUP);


tour.attach(5);
MsTimer2::set(800, MouvementTourelle);
MsTimer2::start();


for(i=0;i<3;i++)recepetion_tram[i]=0;//ramplit de zero le tableau de réception

com=0;
while (com!=1) {
  Serial.print("x ");
  Serial.println(pos1.x);
}
com=0;
pos1=position(pos1);
pos1pre.x=pos1.x;//remplisage de xp(positon actuelle de x)
pos1pre.y=pos1.y;//remplisage de yp(positon actuelle de y)  com=0;
etat=1;
//etat=1;//passage a l'état 1 voir la machine d'état
}
void loop() {
//byte utiliser pour la mémoire de de la trame t[0]
//long dist;
//varible coréspondant a la distance a par courir elle ne comprote pas de sance
//sense variable relative au sance de la rotation ou de la direction du go
//byte i;
byte e,f,g;
//mise a jour capteur
//mise a jour etat
if(((etatp==3)&&(digitalRead(CAPTEUR_AV)==LOW)&&(pas*sense>=0))){ //détection capteur avant
  etat=6;
  temp_balise=millis();
}
if(((etatp==3)&&(digitalRead(CAPTEUR_AR)==LOW)&&(pas*sense<=0))){ //detection capteur arrière
  etat=6;
  temp_balise=millis();
}
//mise a jour actionneur

switch (etat){//permet de réaliser les différent etat de la machinne d'etat
  case 1://etat 1 (antante de com)
  if (com==1){//si nous avons communication alors on redirige vert les diférant état + analyser la trame

    mouvement=0;//mise a zero des diférante chose a fair et dans quelle sanse
    e=recepetion_tram[0];//mise en mémoire de la trame 0
    f=recepetion_tram[0];
    g=recepetion_tram[0];
    /*xp=pos1.x;//remplisage de xp(positon actuelle de x)
    yp=pos1.y;//remplisage de yp(positon actuelle de y)*/
    posdeb=pos1;

    pos1=position(pos1);

    go = (int)sqrt((double)pow((pos1.x-posdeb.x),2)+pow((pos1.y-posdeb.y),2));//valeur du go dans le turn go turn
    if (go!=0)mouvement|=(1<<3);//dit pour la suitte qu il faut avancer
    turnar=(int)(recepetion_tram[3]+((f&=(0x80))<<1));//valuer de langle d'arriver
    //determination de l'angle du turn 1 pour rejoindre le nouvaux x et y
    if(go!=0)turndepart=360-(int)((float)atan((double)(((float)(pos1.y-posdeb.y))/((float)(pos1.x-posdeb.x))))*(float)180/pi)-turnactu;
    else turndepart=0;

    mouvement|=1<<5;//active l'étape de turn 2
    if((pos1.y-posdeb.y)<0)turndepart=turndepart-360;//ajous d'angle du a la fonciton tnagnete
    if((pos1.x-posdeb.x)<0)turndepart=turndepart+180;
    turndepart=turndepart%360;//pour suprimet les tour sur lui mêmme

    if(turndepart>180){//pour eviter de fair plus que 180 degrai
      turndepart=360-turndepart;
      mouvement|=1<<0;//activation du sence inverce de rotation
    }

    //if(turndepart>90 || turndepart<-90){ //changement d'angle pour passer en marche arriere


    if(abs(turndepart)>90){ //changement d'angle pour passer en marche arriere
    if(turndepart>90){
      turndepart=(180-turndepart)*-1;
      }
      else {
        if(turndepart<-90){
          turndepart=180-turndepart;
        }
      }
      go=go*-1;
      mouvement|=1<<0;
      }
      turndepart=turndepart%360;

  //  }

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
  if(etatp==6){
    teste++;
      pas=((long)(float)go*coeficien_go)+posdetec;
    }else{
      pas=(long)(coeficien_go*(float)(go)); //calcule du nombre de pas pour les roue sans le signe de la direction
    }
  motor_D.setCurrentPosition(0);
  motor_G.setCurrentPosition(0);
  if (iso_bite(mouvement,2)==1<<2)sense=-1; //teste pour conaitre le sance de la translation
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
    }

    //else turnactu=turnactu-turnarrive;
    turnactu=turnar;
    pas=(long)(coeficien_turn*(float)turnarrive);//calcule du nombre de pas pour les roue sans le signe de la direction
    if (iso_bite(mouvement,4)==1<<4)sense=-1;//turnactu=turnactu+180;}//teste pour conaitre le sance de la translation
    else sense=1;
    motor_D.move(pas*sense);//activation de la rotation jusque cette valeur de pas moteur droite
    motor_G.move(pas*sense);//activation de la rotation jusque cette valeur de pas moteur gauche
    etat=5;//passe a l etat 5
    etatp=4;//passe l'étatpe précédante a 4

    turnactu=turnactu%360;
  break;

  case 5://attante de fin adr_deplacement$
  motor_D.run();//lancemant du moteur droit
 motor_G.run();//lancemant du moteur gauche
    if(motor_D.isRunning()==false && motor_G.isRunning()==false){//teste des morteur a l'arrée pour pacer au d'éplacemnt suivant
      if(etatp==2 && (iso_bite(mouvement,3)==(1<<3)))etat=3;//passe a l'état 3 si état précédant etait 2 et si le go est activer
      else if (etatp==2 && (iso_bite(mouvement,5)==(1<<5)))etat=4;//si non passe a l état 4 si etat précédan etait 2 et si turn 2 est activer
      else if (etatp==3 && (iso_bite(mouvement,5)==(1<<5)))etat=4;//si non passe a l'état 5 si etat précédan etait 3 et si turn 2 est activer
      else if (com==0 && recepetion_tram[4]>15){
        if(recepetion_tram[4]>=16 && recepetion_tram[4]<=19){// test pour recalage
          recalageX(pos1);
          etat=1;//passe a l'état 1
          com=0;
        }
        else {
          recalageY(pos1);
          etat=1;//passe a l'état 1
          com=0;
        }
      }
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

  case 6:// arret des moteur car détetion
  motor_D.stop();//stop moteurs
  motor_G.stop();//stop du moteur droit
  posdetec=motor_G.currentPosition();
  if ((digitalRead(CAPTEUR_AV)==LOW)||(digitalRead(CAPTEUR_AR)==LOW)){
    temp_balise=millis();
  }

  etatp=6;

  if ((digitalRead(CAPTEUR_AV)==HIGH)&&(digitalRead(CAPTEUR_AR)==HIGH)&&(millis()-temp_balise)==2100){
    etat=3;
  }
  break;
}

}

void receiveEvent(int howMany){//fonction d'intérupetion l'or dun envoi du maitre
byte i;//variable pour le for
for(i=0;i<howMany;i++)recepetion_tram[i]=Wire.read();//rampli le tableau si avec les valeur de la transmition
  com=1;
  Serial.println(recepetion_tram[4]);
// passe la comme a 1 pour l'éxecution de la trame en cour
}

void requestEvent(){//fonciton d'intérupetion l'or d une deamande de trame
Wire.write(com);//le maitre lira la valeur de com
}

byte iso_bite(byte analiser, byte decalage){
return (analiser &=1<<decalage);
}
pos position (pos pos1){
byte e,f;
e=recepetion_tram[0];//mise en mémoire de la trame 0
f=recepetion_tram[0];

pos1.x=(int)(recepetion_tram[1]+((e&=(0x70))<<4));//valeur des x et y à attindre
pos1.y=(int)(recepetion_tram[2]+((f&=(0x0F))<<8));

return pos1;
}

void recalageX (pos pos1){
if(recepetion_tram[4]==16){
  motor_D.move((long)(coeficien_go*(float)(700)));
  motor_G.move((long)(coeficien_go*(float)(-700)));
  while(digitalRead(RECALAGEAVANT)==1){
//mettre delay 3s avant run
    motor_D.run();
    motor_G.run();
  }delay(3000);
  motor_D.setCurrentPosition(0);
  motor_G.setCurrentPosition(0);
  pos1.x=2000;
}
else{
  if(recepetion_tram[4]==17){
    motor_D.move((long)(coeficien_go*(float)(-700)));
    motor_G.move((long)(coeficien_go*(float)(700)));
    while(digitalRead(RECALAGEARRIERE)==1){
//mettre delay 3s avant run
      motor_D.run();
      motor_G.run();
    }delay(3000);
      motor_D.setCurrentPosition(0);
      motor_G.setCurrentPosition(0);
      pos1.x=2000;
    }
    else{
      if(recepetion_tram[4]==18){
        motor_D.move((long)(coeficien_go*(float)(700)));
        motor_G.move((long)(coeficien_go*(float)(-700)));
        while(digitalRead(RECALAGEAVANT)==1){
//mettre delay 3s avant run
          motor_D.run();
          motor_G.run();
          }delay(3000);
          motor_D.setCurrentPosition(0);
          motor_G.setCurrentPosition(0);
          pos1.x=0;
        }
        else{
          if(recepetion_tram[4]==19){
            motor_D.move((long)(coeficien_go*(float)(-700)));
            motor_G.move((long)(coeficien_go*(float)(700)));
            while(digitalRead(RECALAGEARRIERE)==1){
//mettre delay 3s avant run
              motor_D.run();
              motor_G.run();
              }delay(3000);
              motor_D.setCurrentPosition(0);
              motor_G.setCurrentPosition(0);
              pos1.x=0;
            }
          }
        }
      }
}

void recalageY (pos pos1){
  if(recepetion_tram[4]==20){
    motor_D.move((long)(coeficien_go*(float)(700)));
    motor_G.move((long)(coeficien_go*(float)(-700)));
    while(digitalRead(RECALAGEAVANT)==1){
//mettre delay 3s avant run
      motor_D.run();
      motor_G.run();
    }delay(3000);
    motor_D.setCurrentPosition(0);
    motor_G.setCurrentPosition(0);
    pos1.y=0;
  }
  else{
    if(recepetion_tram[4]==21){
      motor_D.move((long)(coeficien_go*(float)(-700)));
      motor_G.move((long)(coeficien_go*(float)(700)));
      while(digitalRead(RECALAGEARRIERE)==1){
//mettre delay 3s avant run
        motor_D.run();
        motor_G.run();
        }delay(3000);
        motor_D.setCurrentPosition(0);
        motor_G.setCurrentPosition(0);
        pos1.y=0;
      }
      else{
        if(recepetion_tram[4]==22){
          motor_D.move((long)(coeficien_go*(float)(700)));
          motor_G.move((long)(coeficien_go*(float)(-700)));
          while(digitalRead(RECALAGEAVANT)==1){
//mettre delay 3s avant run
            motor_D.run();
            motor_G.run();
            }delay(3000);
            motor_D.setCurrentPosition(0);
            motor_G.setCurrentPosition(0);
            pos1.y=3000;
          }
          else{
            if(recepetion_tram[4]==23){
              motor_D.move((long)(coeficien_go*(float)(-700)));
              motor_G.move((long)(coeficien_go*(float)(700)));
              while(digitalRead(RECALAGEARRIERE)==1){
//mettre delay 3s avant run
                motor_D.run();
                motor_G.run();
                }delay(3000);
                motor_D.setCurrentPosition(0);
                motor_G.setCurrentPosition(0);
                pos1.y=3000;
              }
            }
          }
        }
}
void MouvementTourelle(){
  if(cons==0){
      tour.write(150);//180 degrès
      cons=150;
      }
    else{
      tour.write(0);
      cons=0;// 15 degrès
    }
}
