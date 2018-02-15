#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F, 20, 4); //Direccion de LCD
#include <Wire.h>

#include "EmonLib.h"             // Include Emon Library
EnergyMonitor emon1;             // Create an instance 1
EnergyMonitor emon2;             // Create an instance 2
EnergyMonitor emon3;             // Create an instance 3


//----variables para sensor de tension-----

int i = 0;

float tenvals1 = 0.0;
float tenvals2 = 0.0;
float tenvals3 = 0.0;

float Vrms1 = 0.0;
float Vrms2 = 0.0;
float Vrms3 = 0.0;

//----------------------------------------

float setPointFase1 = 200;
float setPointFase2 = 200;
float setPointFase3 = 200;
float Fase1, Fase2, Fase3;

int lectura1, lectura2, lectura3;

int switchFase1 = 22;
int switchFase2 = 23;
int switchFase3 = 24;

int switchManAut = 25;

int pasos = 26;

int direccion1 = 27;
int enable1 = 28;

int direccion2 = 29;
int enable2 = 30;

int direccion3 = 31;
int enable3 = 32;

//--------------Finales de Carrera------

int Fcs_1 = 33;
int Fci_1 = 34;

int Fcs_2 = 35;
int Fci_2 = 36;

int Fcs_3 = 37;
int Fci_3 = 38;

//----Configuraciones del encoder----

const int channelPinA = 2;
const int channelPinB = 3;

const int timeThreshold = 20;
long timeCounter = 0;

const int maxSteps = 255;
volatile int ISRCounter = 0;
volatile int ISRCounterM = 0;

int counter = 0 ;
int counter1 = 819; // Porque no 0? por el cambio de giro?
int counter2 = 819;
int counter3 = 819;

int ISRCounterMPrev;
int ISRCounterPrev;

volatile bool IsCW = true ; //volatile int IsCW = 1;

//-------------------------------------
void leerEncoder();
void setPoint();
void leerTension();
void ajuste();
void cotrolManual();
void mostrarPantalla_A();
void mostrarPantalla_M();

//--------------------------------------------------//
//---------Tiempos no bloqueantes-------------------//
//------------------------------T_0
unsigned long previousMillis = 0;
const long interval = 1000 ;
//------------------------------T_1
unsigned long previousMillis1 = 0;
const long interval1 = 1000 ;

//------------------------------T_2.1-------> Tiempo para el ajuste atuomático 
unsigned long previousMillis2_1 = 0;
const long interval2_1 = 1000 ;
//------------------------------T_2.2
unsigned long previousMillis2_2 = 0;
const long interval2_2 = 1000 ;
//------------------------------T_2.3
unsigned long previousMillis2_3 = 0;
const long interval2_3 = 1000 ;

//------------------------------T_3
unsigned long previousMillis3 = 0;
const long interval3 = 1000 ;
//------------------------------T_4
unsigned long previousMillis4 = 0;
const long interval4 = 1000 ;
//------------------------------T_5
unsigned long previousMillis5 = 0;
const long interval5 = 1000 ;
//------------------------------T_6
unsigned long previousMillis6 = 0;
const long interval6 = 500 ;


void setup() {
  //-------------Configuracion LCD-------------

  lcd.init();
  lcd.begin(20, 4);
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

  pinMode(switchFase1, INPUT_PULLUP);
  pinMode(switchFase2, INPUT_PULLUP);
  pinMode(switchFase3, INPUT_PULLUP);

  pinMode(switchManAut, INPUT_PULLUP);

  pinMode(pasos, OUTPUT);

  pinMode(enable1, OUTPUT);
  pinMode(direccion1, OUTPUT);

  pinMode(enable2, OUTPUT);
  pinMode(direccion2, OUTPUT);

  pinMode(enable3, OUTPUT);
  pinMode(direccion3, OUTPUT);

  //----------------

  pinMode (Fcs_1, INPUT);
  pinMode (Fci_1, INPUT);

  pinMode (Fcs_2, INPUT);
  pinMode (Fci_2, INPUT);;

  pinMode (Fcs_3, INPUT);
  pinMode (Fci_3, INPUT);

  //----------------

  digitalWrite(enable1, HIGH); // LOW = ACTIVA
  digitalWrite(enable2, HIGH);
  digitalWrite(enable3, HIGH);


  //-----------------------------------

  //----Configuraciones del encoder----

  pinMode(channelPinA, INPUT_PULLUP);
  pinMode(channelPinB, INPUT_PULLUP);

  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(channelPinA), doEncodeA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(channelPinB), doEncodeB, CHANGE);

  //----------------------------------

}

void loop() {

  if (counter != ISRCounter)
  {
    counter = ISRCounter;    // Ver si funciona el encoder
    Serial.println(counter2);
  }

  if (!digitalRead(switchManAut)) {

    unsigned long currentMillis = millis();

    // Gestionamos los Tiempos
    if ((unsigned long)(currentMillis - previousMillis) >= interval)    //----T_0
    {
      setPoint();
      previousMillis = millis();
    }
    if ((unsigned long)(currentMillis - previousMillis1) >= interval1)  //----T_1
    {
      leerTension();
      previousMillis1 = millis();
    }
    //---------------------------

    if ((unsigned long)(currentMillis - previousMillis2_1) >= interval2_1)  //----T_2.1
    {
      ajusteMotor1();
      previousMillis2_1 = millis();
    }
    
    digitalWrite(enable1, HIGH);
    
    if ((unsigned long)(currentMillis - previousMillis2_2) >= interval2_2)  //----T_2.2
    {
      ajusteMotor2();
      previousMillis2_2 = millis();
    }
    
    digitalWrite(enable2, HIGH);
    
    if ((unsigned long)(currentMillis - previousMillis2_3) >= interval2_3)  //----T_2.3
    {
      ajusteMotor3();
      previousMillis2_3 = millis();
    }
    
    digitalWrite(enable3, HIGH);

    //----------------------------

    if ((unsigned long)(currentMillis - previousMillis3) >= interval3)  //----T_3
    {
      mostrarPantalla_A();
      previousMillis3 = millis();
    }
  }

  else {

    unsigned long currentMillis = millis();

    // Gestionamos los Tiempos
    if ((unsigned long)(currentMillis - previousMillis4) >= interval4)  //----T_4
    {
      controlManual();
      previousMillis4 = millis();
    }
    if ((unsigned long)(currentMillis - previousMillis5) >= interval5)  //----T_5
    {
      leerTension();
      previousMillis5 = millis();
    }
    if ((unsigned long)(currentMillis - previousMillis6) >= interval6)  //----T_6
    {
      mostrarPantalla_M();
      previousMillis6 = millis();
    }
  }
}


void setPoint() {

  //----Codigo para leer el encoder------------------------------

  if (ISRCounter > ISRCounterPrev) {

    if (digitalRead(switchFase1) && counter1 < 1024) {
      counter1 = counter1 + 4;
      setPointFase1 = counter1 * 250.0 / 1024.0;
    }

    if (digitalRead(switchFase2) && counter2 < 1024) {
      counter2 = counter2 + 4;
      setPointFase2 = counter2 * 250.0 / 1024.0;
    }

    if (digitalRead(switchFase3) && counter3 < 1024) {
      counter3 = counter3 + 4;
      setPointFase3 = counter3 * 250.0 / 1024.0;
    }
    ISRCounterPrev = ISRCounter;
  }

  if (ISRCounter < ISRCounterPrev) {

    if (digitalRead(switchFase1) && counter1 > 0) {
      counter1 = counter1 - 4;
      setPointFase1 = counter1 * 250.0 / 1024.0;
    }

    if (digitalRead(switchFase2) && counter2 > 0) {
      counter2 = counter2 - 4;
      setPointFase2 = counter2 * 250.0 / 1024.0;
    }

    if (digitalRead(switchFase3) && counter3 > 0) {
      counter3 = counter3 - 4;
      setPointFase3 = counter3 * 250.0 / 1024.0;
    }

    ISRCounterPrev = ISRCounter;
  }


  //------------------------------------------------------------

}

void leerTension() {

  emon1.calcVI(20, 1000);        // Calculate all. No.of half wavelengths (crossings), time-out
  emon2.calcVI(20, 1000);        // Calculate all. No.of half wavelengths (crossings), time-out
  emon3.calcVI(20, 1000);        // Calculate all. No.of half wavelengths (crossings), time-out

  Vrms1 = (emon1.Vrms);
  Vrms2 = (emon2.Vrms);
  Vrms3 = (emon3.Vrms);


  Fase1 = Vrms1;
  Fase2 = Vrms2 * 0.988;
  Fase3 = Vrms3 * 0.988;

}

void ajusteMotor1() {

  //------ajuste fase 1------------

  float LimSup1 , LimInf1; // Corresponde a Fase 1
  LimSup1 = setPointFase1 * 1.01 ; // Limite Superior de seteo
  LimInf1 = setPointFase1 * 0.99 ; // Limite Inferior de seteo

  if ( Fase1 < LimInf1 || Fase1 > LimSup1 ) {
    if ( Fase1 < LimInf1 && Fase1 < 250 && digitalRead(Fcs_1)) {
      digitalWrite(enable1, LOW); // Se cambio el estado según configuración del tb6560
      digitalWrite(direccion1, LOW);

    }
    if ( Fase1 > LimSup1 && Fase1 > 0.0 && digitalRead(Fci_1)) {
      digitalWrite(enable1, LOW);
      digitalWrite(direccion1, HIGH);
    }
    
  }
}
//-------------------------------

void ajusteMotor2() {

  //------ajuste fase 2------------

  float LimSup2 , LimInf2; // Corresponde a Fase 1
  LimSup2 = setPointFase2 * 1.01 ; // Limite Superior de seteo
  LimInf2 = setPointFase2 * 0.99 ; // Limite Inferior de seteo

  if ( Fase2 < LimInf2 || Fase2 > LimSup2  ) {
    if ( Fase2 < LimInf2 && Fase2 < 250 && digitalRead(Fcs_2) ) {
      digitalWrite(enable2, LOW);
      digitalWrite(direccion2, LOW);
    }
    if ( Fase2 > LimSup2 && Fase2 > 0.0 && digitalRead(Fci_2) ) {
      digitalWrite(enable2, LOW);
      digitalWrite(direccion2, HIGH);
    }
    
  }
}
//-------------------------------

void ajusteMotor3() {

  //------ajuste fase 3------------

  float LimSup3 , LimInf3; // Corresponde a Fase 1
  LimSup3 = setPointFase3 * 1.01 ; // Limite Superior de seteo
  LimInf3 = setPointFase3 * 0.99 ; // Limite Inferior de seteo

  if ( Fase3 < LimInf3 || Fase3 > LimSup3  ) {
    if ( Fase3 < LimInf3 && Fase3 < 250 && digitalRead(Fcs_3) ) {
      digitalWrite(enable3, LOW);
      digitalWrite(direccion3, LOW);

    }
    if ( Fase3 > LimSup3 && Fase3 > 0.0 && digitalRead(Fci_3) ) {
      digitalWrite(enable3, LOW);
      digitalWrite(direccion3, HIGH);
    }
    
  }

  //------------------------------------

}

void controlManual() {

  //----Codigo para leer el encoder------------------------------

  //-------------------------------------------------------------

  if (ISRCounterM != ISRCounterMPrev) {

    if (IsCW == true) {

      //-------control fase 1 sentido horario--------

      if (digitalRead(switchFase1) && digitalRead(Fcs_1)) {

        digitalWrite(enable1, LOW);  // LOW = ACTIVA
        digitalWrite(direccion1, LOW);// LOW = sube? o baja? estromboliko....
        delay(500);
      }

      //---------------------------------------------

      //-------control fase 2 sentido horario--------

      if (digitalRead(switchFase2) && digitalRead(Fcs_2)) {

        digitalWrite(enable2, LOW);
        digitalWrite(direccion2, LOW);
        delay(500);

      }

      //---------------------------------------------

      //-------control fase 3 sentido horario--------

      if (digitalRead(switchFase3) && digitalRead(Fcs_3)) {

        digitalWrite(enable3, LOW);
        digitalWrite(direccion3, LOW);
        delay(500);
      }

      //---------------------------------------------
    }

    else {

      //-----control fase 1 sentido antihorario------

      if (digitalRead(switchFase1) && digitalRead(Fci_1)) {

        digitalWrite(enable1, LOW);
        digitalWrite(direccion1, HIGH);
        delay(500);
      }

      //---------------------------------------------

      //-----control fase 2 sentido antihorario------

      if (digitalRead(switchFase2) && digitalRead(Fci_2)) {

        digitalWrite(enable2, LOW);
        digitalWrite(direccion2, HIGH);
        delay(500);
      }

      //---------------------------------------------

      //-----control fase 3 sentido antihorario------

      if (digitalRead(switchFase3) && digitalRead(Fci_3)) {

        digitalWrite(enable3, LOW);
        digitalWrite(direccion3, HIGH);
        delay(500);
      }

    }

    digitalWrite(enable1, HIGH);
    digitalWrite(enable2, HIGH);
    digitalWrite(enable3, HIGH);

  }
  ISRCounterMPrev = ISRCounterM;
}

void mostrarPantalla_A() {

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" AUTO ");
  //  lcd.setCursor(0, 1);
  //  lcd.print("MATICO");
  lcd.setCursor(0, 3);
  lcd.print("LAMyEN");

  lcd.setCursor(7, 0);
  lcd.print("SetP.:");
  lcd.setCursor(7, 1);
  lcd.print(setPointFase1);
  lcd.print("V");
  lcd.setCursor(7, 2);
  lcd.print(setPointFase2);
  lcd.print("V");
  lcd.setCursor(7, 3);
  lcd.print(setPointFase3);
  lcd.print("V");

  lcd.setCursor(14, 0);
  lcd.print("Fases:");
  lcd.setCursor(14, 1);
  lcd.print(Fase1);
  lcd.print("V");
  lcd.setCursor(14, 2);
  lcd.print(Fase2);
  lcd.print("V");
  lcd.setCursor(14, 3);
  lcd.print(Fase3);
  lcd.print("V");


}

void mostrarPantalla_M() {

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("MANUAL");
  lcd.setCursor(0, 3);
  lcd.print("LAMyEN");

  lcd.setCursor(7, 0);
  lcd.print("FaseR:");
  lcd.print(Fase1);
  lcd.print("V");
  lcd.setCursor(7, 1);
  lcd.print("FaseS:");
  lcd.print(Fase2);
  lcd.print("V");
  lcd.setCursor(7, 2);
  lcd.print("FaseT:");
  lcd.print(Fase3);
  lcd.print("V");

}




void doEncodeA()
{
  if (millis() > timeCounter + timeThreshold)
  {
    if (digitalRead(channelPinA) == digitalRead(channelPinB))
    {
      IsCW = true;
      ISRCounter++;
      ISRCounterM++;
    }
    else
    {
      IsCW = false;
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
      IsCW = true;
      ISRCounter++;
      ISRCounterM++;
    }
    else
    {
      IsCW = false;
      ISRCounter--;
      ISRCounterM--;
    }
    timeCounter = millis();
  }
}

