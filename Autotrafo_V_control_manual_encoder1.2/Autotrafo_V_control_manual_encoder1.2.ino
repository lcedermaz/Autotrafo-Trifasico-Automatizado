#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F,20,4);//Direccion de LCD
#include <Wire.h>

#include "EmonLib.h"             // Include Emon Library
EnergyMonitor emon1;             // Create an instance 1
EnergyMonitor emon2;             // Create an instance 2
EnergyMonitor emon3;             // Create an instance 3

//----Configuraciones del encoder----
  
  const int channelPinA = 2;
  const int channelPinB = 3;
   
  const int timeThreshold = 20;
  long timeCounter = 0;
   
  const int maxSteps = 1024;
  volatile int ISRCounter = 0;
  volatile int ISRCounterM = 0;
  
  int counter1 = 800;
  int counter2 = 800;
  int counter3 = 800;
  
  int ISRCounterMPrev;
  int ISRCounterPrev;
   
  volatile int IsCW = 1;
  volatile long motor_position ; 
  
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

  //----------------------------------------
  
  int switchFase1 = 22;
  int switchFase2 = 23;
  int switchFase3 = 24;

  int switchManAut = 25;
 
//----------------------------------------

  int Fcs_1=33;
  int Fci_1=34;
  
  int Fcs_2=35;
  int Fci_2=36;
  
  int Fcs_3=37;
  int Fci_3=38;
  
//----------------------------------------
  
  int direccion1 = 27;
  int enable1 = 28;

  int direccion2 = 29;
  int enable2 = 30;

  int direccion3 = 31;
  int enable3 = 32;
  
//----------------------------------------

  void leerTension();
  void controlManual();
  void mostrarPatallaM();
  
void setup() {
   //-------------Configuracion LCD-------------

  lcd.init(); 
  lcd.begin(20,4);
  lcd.clear();  
  lcd.backlight();// Indicamos medidas de LCD 
  Wire.begin(); //configura el bus I2C estableciendo arduino como MASTER 

  //----------------------------------------
  //----Configuraciones del encoder----

   pinMode(channelPinA, INPUT_PULLUP);
   Serial.begin(9600);
   attachInterrupt(digitalPinToInterrupt(channelPinA), doEncodeA, CHANGE);
   attachInterrupt(digitalPinToInterrupt(channelPinB), doEncodeB, CHANGE);
  
  //----Configuracion de sensor de voltaje----

  emon1.voltage(2, 352 , 1.7);  // Voltage: input pin, calibration, phase_shift
  emon2.voltage(3, 352 , 1.7);  // Voltage: input pin, calibration, phase_shift
  emon3.voltage(4, 352 , 1.7);  // Voltage: input pin, calibration, phase_shift

  //------------------------------------------

  //----Configuracion Entradas/Salidas------
  
  pinMode(switchFase1, INPUT_PULLUP);
  pinMode(switchFase2, INPUT_PULLUP);
  pinMode(switchFase3, INPUT_PULLUP);
  
  pinMode(enable1, OUTPUT);
  pinMode(direccion1, OUTPUT);

  pinMode(enable2, OUTPUT);
  pinMode(direccion2, OUTPUT);

  pinMode(enable3, OUTPUT);
  pinMode(direccion3, OUTPUT);


  digitalWrite(enable1, HIGH); // (LOW=ACTIVA , HIGH = DESACTIVA)
  digitalWrite(enable2, HIGH);
  digitalWrite(enable3, HIGH);

  digitalWrite(direccion1, LOW); // PRUEBA TB 6560
  digitalWrite(direccion2, LOW);
  digitalWrite(direccion3, LOW); 
  //-----------------------------------

  

}

void loop() {


    controlManual();
       
    leerTension();
    
    mostrarPatallaM();
}


void leerTension(){


    emon1.calcVI(20,2000);         // Calculate all. No.of half wavelengths (crossings), time-out
    emon2.calcVI(20,2000);         // Calculate all. No.of half wavelengths (crossings), time-out
    emon3.calcVI(20,2000);         // Calculate all. No.of half wavelengths (crossings), time-out
  
    Vrms1 = (emon1.Vrms - 3);
    Vrms2 = (emon2.Vrms - 3);
    Vrms3 = (emon3.Vrms - 3);
  
    tenvals1 += Vrms1;
    tenvals2 += Vrms2;
    tenvals3 += Vrms3;

  
  Fase1 = (tenvals1/10) + ((240-(tenvals1/10))*0.015); // Se le aplico una formula de correcion para un error de 3 volt en 40Vca
  Fase2 = (tenvals2/10) + ((240-(tenvals2/10))*0.015); // Se le aplico una formula de correcion para un error de 3 volt en 40Vca
  Fase3 = (tenvals3/10) + ((240-(tenvals3/10))*0.015); // Se le aplico una formula de correcion para un error de 3 volt en 40Vca
 
  
}


void controlManual(){

   //----Codigo para leer el encoder------------------------------


  //-------------------------------------------------------------

  if(ISRCounter > 0 ){
      
    //-------control fase 1 sentido horario--------

      if(digitalRead(switchFase1)&& digitalRead(Fcs_1)){
        digitalWrite(enable1, LOW); //(LOW=ACTIVA , HIGH = DESACTIVA)
        digitalWrite(direccion1, HIGH);
        delay (1000);   
      }

      //---------------------------------------------

      //-------control fase 2 sentido horario--------

      if(digitalRead(switchFase2)&& digitalRead(Fcs_2)){

        digitalWrite(enable2, LOW);
        digitalWrite(direccion2, HIGH);
        delay(1000);     
      }

      //---------------------------------------------

      //-------control fase 3 sentido horario--------

      if(digitalRead(switchFase3)&& digitalRead(Fcs_3)){

        digitalWrite(enable3, LOW);
        digitalWrite(direccion3, HIGH);
        delay(1000);  
        }
}

      //---------------------------------------------

    else if (ISRCounter < 0 ){

      //-----control fase 1 sentido antihorario------

       if(digitalRead(switchFase1)&& digitalRead(Fci_1)){
        digitalWrite(enable1, LOW); //(LOW=ACTIVA , HIGH = DESACTIVA)
        digitalWrite(direccion1, LOW);
        delay (1000);     
      }

      //---------------------------------------------

      //-----control fase 2 sentido antihorario------

      if(digitalRead(switchFase2)&& digitalRead(Fci_2)){
        digitalWrite(enable1, LOW); //(LOW=ACTIVA , HIGH = DESACTIVA)
        digitalWrite(direccion1, LOW);
        delay (1000);     
      }

      //---------------------------------------------

      //-----control fase 3 sentido antihorario------

      if(digitalRead(switchFase3)&& digitalRead(Fci_3)){
        digitalWrite(enable1, LOW); //(LOW=ACTIVA , HIGH = DESACTIVA)
        digitalWrite(direccion1, LOW);
        delay (1000); 
          }    
    }

    digitalWrite(enable1, HIGH);
    digitalWrite(enable2, HIGH);
    digitalWrite(enable3, HIGH);
    
  }

  //ISRCounterMPrev = ISRCounterM;
  


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

void doEncodeA()
{
   if (millis() > timeCounter + timeThreshold)
   {
      if (digitalRead(channelPinA) == digitalRead(channelPinB))
      {
         IsCW = 1;
         ISRCounter++;
         ISRCounterM++;
      }
      else
      {
         IsCW = 2;
         ISRCounter--;
         ISRCounterM--;
      }
      timeCounter = millis();
   }
}
 
void doEncodeB()
{
   if (millis() > timeCounter + timeThreshold)
   {
      if (digitalRead(channelPinA) != digitalRead(channelPinB))
      {
         IsCW = 1;
         ISRCounter++;
         ISRCounterM++;
      }
      else
      {
         IsCW = 2;
         ISRCounter--;
         ISRCounterM--;
      }
      timeCounter = millis();
   }
}

