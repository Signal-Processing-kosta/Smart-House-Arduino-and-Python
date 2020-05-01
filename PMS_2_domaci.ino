#include<Servo.h>

const int FOTO_OTP=A1;
const int BELA_D=6;
int svetlost;

double vreme;
double rastojanje;
const int TRIGGER=11;
const int ECHO=12;
const int BUZZER=5;

Servo motor;
const int NTC=A2;
const int SERVO=9;
int temperatura;

const int CRVENA_D=3;
const int ZELENA_D=7;

enum stanje {pocetno,ulazak,alarm};
stanje trenutno_stanje;
char podatak;

void setup() {
  Serial.begin(9600);
  
  pinMode(ZELENA_D,OUTPUT);
  pinMode(CRVENA_D,OUTPUT);
  pinMode(BELA_D,OUTPUT);    //pin za pametna svetla
  pinMode(TRIGGER,OUTPUT);   //pinovi za pametnu garazu
  pinMode(ECHO,INPUT);
  pinMode(BUZZER,OUTPUT);

  motor.attach(SERVO);          //pin za klima uredjaj

  trenutno_stanje=pocetno;
  digitalWrite(BELA_D,LOW);
  digitalWrite(CRVENA_D,LOW);
  digitalWrite(ZELENA_D,LOW);
}

void loop() {
  if (Serial.available()>0 && trenutno_stanje==pocetno){
      podatak=char(Serial.read());
      if (podatak=='u'){    //unesena je ispravna sifra i omgucen ulazak u kucu
        digitalWrite(ZELENA_D,HIGH);
        trenutno_stanje=ulazak;
      }
      else if(podatak=='a'){
        trenutno_stanje=alarm;
      }
    
  }
  else if (trenutno_stanje==alarm){   //stalno ce ulaziti u ovo stanje pa ce dioda da treperi
        tone(BUZZER,10);
        digitalWrite(CRVENA_D,HIGH);
        delay(500);                    
        digitalWrite(CRVENA_D,LOW);
        delay(500);
  }
  else if (trenutno_stanje==ulazak){        // USLI SMO U KUCU
                                            // ***klima uredjaj***
    temperatura=analogRead(NTC);       
    temperatura=map(temperatura,500,600,0,180); //temperatura se skalira na ugao od 0 do 180 stepeni
    motor.write(temperatura);
    delay(50); //daje se vreme motoru da promeni polozaj(u milisekundama)
                                            // ***pametna svetla***
    svetlost=analogRead(FOTO_OTP);         
    svetlost=map(svetlost,200,750,0,255);
    analogWrite(BELA_D,svetlost);   
                                           // ***pametna garaza***
    digitalWrite(TRIGGER,LOW); //obezbedjujemo nulu na pocetku
    delayMicroseconds(2);
    digitalWrite(TRIGGER,HIGH); //pravimo visok naponski nivo trajanja 10us
    delayMicroseconds(10);
    digitalWrite(TRIGGER,LOW);
    
    vreme=pulseIn(ECHO,HIGH); // vraca trajanje visokog naponskog nivoa u us
    
    rastojanje=vreme*0.034/2; //0.034=brzina zvuka u cm/us
  
   if (rastojanje<10)   
     tone(BUZZER,10);
   else 
     noTone(BUZZER);
  }
}

