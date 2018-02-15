#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F,20,4);//Direccion de LCD
#include <Wire.h>

#include "EmonLib.h"             // Include Emon Library
EnergyMonitor emon1;             // Create an instance 1
EnergyMonitor emon2;             // Create an instance 2
EnergyMonitor emon3;             // Create an instance 3

//-----------------Seteo de tiempo de refresco Display
unsigned long previousMillis = 0;        
const long interval = 2000 ;

  //----variables para sensor de tension-----

  int i = 0;
  
  float tenvals1 = 0.0;
  float tenvals2 = 0.0;
  float tenvals3 = 0.0;

  float Vrms1 = 0.0;
  float Vrms2 = 0.0;
  float Vrms3 = 0.0;

  //----------------------------------------

  float Fase1, Fase2, Fase3;
  
  int lectura1, lectura2, lectura3;


  
  int switchFase1Up = 22;
  int switchFase1Down = 23;
  int switchFase2Up = 24;
  int switchFase2Down = 25;
  int switchFase3Up = 26;
  int switchFase3Down = 27;

  int FCFase1Sup = 28;
  int FCFase1Inf = 29;
  int FCFase2Sup = 30;
  int FCFase2Inf = 31;
  int FCFase3Sup = 32;
  int FCFase3Inf = 33;

  int pasos = 34;
  
  int direccion1 = 35;
  int enable1 = 36;

  int direccion2 = 37;
  int enable2 = 38;

  int direccion3 = 39;
  int enable3 = 40;


  void leerTension();
  void cotrolManual();
  void mostrarPatallaM();
  

  
void setup() {
   //-------------Configuracion LCD-------------

  lcd.init(); 
  lcd.begin(20,4);
  lcd.clear();  
  lcd.backlight();// Indicamos medidas de LCD 
  Wire.begin(); //configura el bus I2C estableciendo arduino como MASTER 

  //----------------------------------------

  //----Configuracion de sensor de voltaje----

  emon1.voltage(2, 352 , 1.7);  // Voltage: input pin, calibration, phase_shift
  emon2.voltage(3, 352 , 1.7);  // Voltage: input pin, calibration, phase_shift
  emon3.voltage(4, 352 , 1.7);  // Voltage: input pin, calibration, phase_shift

  //------------------------------------------

  //----Configuracion Entradas/Salidas------
  
  pinMode(switchFase1Up, INPUT);
  pinMode(switchFase1Down, INPUT);
  
  pinMode(switchFase2Up, INPUT);
  pinMode(switchFase2Down, INPUT);
  
  pinMode(switchFase3Up, INPUT);
  pinMode(switchFase3Down, INPUT);
  
  pinMode(pasos, OUTPUT);
  
  pinMode(enable1, OUTPUT);
  pinMode(direccion1, OUTPUT);

  pinMode(enable2, OUTPUT);
  pinMode(direccion2, OUTPUT);

  pinMode(enable3, OUTPUT);
  pinMode(direccion3, OUTPUT);


  digitalWrite(enable1, HIGH); // PRUEBA TB 6560
  digitalWrite(enable2, HIGH);
  digitalWrite(enable3, HIGH);

  //-----------------------------------

  

}

void loop() {


    controlManual();
       
   // leerTension();   
   unsigned long currentMillis = millis();   // Conteo de tiempo para la interrupción
   if(currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    mostrarPatallaM();
   }
}


void leerTension(){

  for(i=0; i<10; i++){

    emon1.calcVI(20,2000);         // Calculate all. No.of half wavelengths (crossings), time-out
    emon2.calcVI(20,2000);         // Calculate all. No.of half wavelengths (crossings), time-out
    emon3.calcVI(20,2000);         // Calculate all. No.of half wavelengths (crossings), time-out
  
    Vrms1 = (emon1.Vrms - 3);
    Vrms2 = (emon2.Vrms - 3);
    Vrms3 = (emon3.Vrms - 3);
  
    tenvals1 += Vrms1;
    tenvals2 += Vrms2;
    tenvals3 += Vrms3;

  }
  
  Fase1 = (tenvals1/10) + ((240-(tenvals1/10))*0.015); // Se le aplico una formula de correcion para un error de 3 volt en 40Vca
  Fase2 = (tenvals2/10) + ((240-(tenvals2/10))*0.015); // Se le aplico una formula de correcion para un error de 3 volt en 40Vca
  Fase3 = (tenvals3/10) + ((240-(tenvals3/10))*0.015); // Se le aplico una formula de correcion para un error de 3 volt en 40Vca
 
  
}


void controlManual(){

   

      //-------control fase 1 sentido horario--------

      if(digitalRead(switchFase1Up) && digitalRead(FCFase1Sup)){

        digitalWrite(enable1, LOW);
        digitalWrite(direccion1, LOW);

       for(i=0; i<30; i++){
        digitalWrite(pasos, HIGH);
        delay(10);
        digitalWrite(pasos, LOW);
        delay(10);
       }

       digitalWrite(enable1, HIGH);
        
      }

      //---------------------------------------------

      //-------control fase 2 sentido horario--------

      if(digitalRead(switchFase2Up) && digitalRead(FCFase2Sup)){

        digitalWrite(enable2, LOW);
        digitalWrite(direccion2, LOW);

       for(i=0; i<30; i++){
        digitalWrite(pasos, HIGH);
        delay(10);
        digitalWrite(pasos, LOW);
        delay(10);
       }

       digitalWrite(enable2, HIGH);
        
      }

      //---------------------------------------------

      //-------control fase 3 sentido horario--------

      if(digitalRead(switchFase3Up) && digitalRead(FCFase3Sup)){

        digitalWrite(enable3, LOW);
        digitalWrite(direccion3, LOW);

       for(i=0; i<30; i++){
        digitalWrite(pasos, HIGH);
        delay(10);
        digitalWrite(pasos, LOW);
        delay(10);
       }

       digitalWrite(enable3, HIGH);
        
      }

      //---------------------------------------------

      
   

      //-----control fase 1 sentido antihorario------

      if(digitalRead(switchFase1Down) && digitalRead(FCFase1Inf)){

        digitalWrite(enable1, LOW);
        digitalWrite(direccion1, HIGH);

       for(i=0; i<30; i++){
        digitalWrite(pasos, HIGH);
        delay(10);
        digitalWrite(pasos, LOW);
        delay(10);
       }

       digitalWrite(enable1, HIGH);
        
      }

      //---------------------------------------------

      //-----control fase 2 sentido antihorario------

      if(digitalRead(switchFase2Down) && digitalRead(FCFase2Inf)){

        digitalWrite(enable2, LOW);
        digitalWrite(direccion2, HIGH);

       for(i=0; i<30; i++){
        digitalWrite(pasos, HIGH);
        delay(10);
        digitalWrite(pasos, LOW);
        delay(10);
       }

       digitalWrite(enable2, HIGH);
        
      }

      //---------------------------------------------

      //-----control fase 3 sentido antihorario------

      if(digitalRead(switchFase3Down) && digitalRead(FCFase3Inf)){

        digitalWrite(enable3, LOW);
        digitalWrite(direccion3, HIGH);

       for(i=0; i<30; i++){
        digitalWrite(pasos, HIGH);
        delay(10);
        digitalWrite(pasos, LOW);
        delay(10);
       }

       digitalWrite(enable2, HIGH);
        
      }

  
}



void mostrarPatallaM(){

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("MANUAL");
  lcd.setCursor(0,3);
  lcd.print("LAMyEN");

  lcd.setCursor(7,0);
  lcd.print("Fase1:");
  lcd.print(Fase1);
  lcd.print("V");
  lcd.setCursor(7,1);
  lcd.print("Fase2:");
  lcd.print(Fase2);
  lcd.print("V");
  lcd.setCursor(7,2);
  lcd.print("Fase3:");
  lcd.print(Fase3);
  lcd.print("V");
 
}

