#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F,20,4);//Direccion de LCD
#include <Wire.h>

float setPointFase1, setPointFase2, setPointFase3, Fase1, Fase2, Fase3;

int lectura1, lectura2, lectura3, i;


  
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

  //----Configuraciones del encoder----
  
const int channelPinA = 2;
const int channelPinB = 3;
 
const int timeThreshold = 20;
long timeCounter = 0;
 
const int maxSteps = 1024;
volatile int ISRCounter = 0;
volatile int ISRCounterM = 0;
int counter = 0;
int ISRCounterMPrev;
 
volatile int IsCW = 1;

  //-------------------------------------
  
  void leerEncoder();
  void setPoint();
  void leerTension();
  void ajuste();
  void cotrolManual();
  void mostrarPatallaA();
  void mostrarPatallaM();
  

  
void setup() {
   //-------------Configuracion LCD-------------

  lcd.init(); 
  lcd.begin(20,4);
  lcd.clear();  
  lcd.backlight();// Indicamos medidas de LCD 
  Wire.begin(); //configura el bus I2C estableciendo arduino como MASTER 

  //----------------------------------------

  //----Configuracion Entradas/Salidas------
  
  pinMode(switchFase1, INPUT);
  pinMode(switchFase2, INPUT);
  pinMode(switchFase3, INPUT);

  pinMode(switchManAut, INPUT);
  
  pinMode(pasos, OUTPUT);
  
  pinMode(enable1, OUTPUT);
  pinMode(direccion1, OUTPUT);

  pinMode(enable2, OUTPUT);
  pinMode(direccion2, OUTPUT);

  pinMode(enable3, OUTPUT);
  pinMode(direccion3, OUTPUT);


  digitalWrite(enable1, LOW); // PRUEBA TB 6560
  digitalWrite(enable2, HIGH);
  digitalWrite(enable3, HIGH);

  //-----------------------------------

  //----Configuraciones del encoder----

   pinMode(channelPinA, INPUT_PULLUP);
   Serial.begin(9600);
   attachInterrupt(digitalPinToInterrupt(channelPinA), doEncodeA, CHANGE);
   attachInterrupt(digitalPinToInterrupt(channelPinB), doEncodeB, CHANGE);

  //----------------------------------

}

void loop() {

  if(digitalRead(switchManAut)){
    
    setPoint();

    leerTension();
 
    ajuste();

    mostrarPatallaA();
    
  }

  else{

    controlManual();

    leerTension();

    mostrarPatallaM();
    
  }

}



void setPoint(){

  //----Codigo para leer el encoder------------------------------

  if (counter != ISRCounter)
   {
      counter = ISRCounter;
      Serial.println(counter);
   }
   delay(100);


  //Dependiendo los switch activos el valor se asiganara al setPoint correspoediente

  if(digitalRead(switchFase1)){
    setPointFase1 = counter * 250.0 / 1024.0;
  }

  if(digitalRead(switchFase2)){
    setPointFase2 = counter * 250.0 / 1024.0;
  }

  if(digitalRead(switchFase3)){
    setPointFase3 = counter * 250.0 / 1024.0;
  }

  //------------------------------------------------------------

  
}

void leerTension(){

  lectura1 = analogRead(1);
  lectura2 = analogRead(2);
  lectura3 = analogRead(3);

  Fase1 = lectura1 * 250.0 / 1024.0;
  Fase2 = lectura2 * 250.0 / 1024.0;
  Fase3 = lectura3 * 250.0 / 1024.0;

  
}

void ajuste(){

  //------ajuste fase 1------------
  
  float LimSup1 , LimInf1; // Corresponde a Fase 1
  LimSup1 = setPointFase1*1.01 ; // Limite Superior de seteo
  LimInf1 = setPointFase1*0.99 ; // Limite Inferior de seteo
 
  if( Fase1 < LimInf1 || Fase1 > LimSup1  ){ 
    if( Fase1 < LimInf1 && Fase1 < 250 ){
      digitalWrite(enable1, HIGH); // Se cambio el estado según configuración del tb6560
      digitalWrite(direccion1, HIGH);
      for(i=0; i<30; i++){
        digitalWrite(pasos, HIGH);
        delay(10);
        digitalWrite(pasos, LOW);
        delay(10);
      }
    }
    if( Fase1 > LimSup1 && Fase1 > 0.0 ){  
      digitalWrite(enable1, HIGH);
      digitalWrite(direccion1, LOW);
      for(i=0; i<30; i++){
        digitalWrite(pasos, HIGH);
        delay(10);
        digitalWrite(pasos, LOW);
        delay(10);
      }
    }

    digitalWrite(enable1, LOW);
    
  }

 //-------------------------------

 //------ajuste fase 2------------
  
  float LimSup2 , LimInf2; // Corresponde a Fase 1
  LimSup2 = setPointFase2*1.01 ; // Limite Superior de seteo
  LimInf2 = setPointFase2*0.99 ; // Limite Inferior de seteo
 
  if( Fase2 < LimInf2 || Fase2 > LimSup2  ){ 
    if( Fase2 < LimInf2 && Fase2 < 250 ){
      digitalWrite(enable2, LOW);
      digitalWrite(direccion2, HIGH);
      for(i=0; i<30; i++){
        digitalWrite(pasos, HIGH);
        delay(10);
        digitalWrite(pasos, LOW);
        delay(10);
      }
    }
    if( Fase2 > LimSup2 && Fase2 > 0.0 ){  
      digitalWrite(enable2, LOW);
      digitalWrite(direccion2, LOW);
      for(i=0; i<30; i++){
        digitalWrite(pasos, HIGH);
        delay(10);
        digitalWrite(pasos, LOW);
        delay(10);
      }
    }

    digitalWrite(enable2, HIGH);
    
  }

  //-------------------------------

  //------ajuste fase 3------------
  
  float LimSup3 , LimInf3; // Corresponde a Fase 1
  LimSup3 = setPointFase3*1.01 ; // Limite Superior de seteo
  LimInf3 = setPointFase3*0.99 ; // Limite Inferior de seteo
 
  if( Fase3 < LimInf3 || Fase3 > LimSup3  ){ 
    if( Fase3 < LimInf3 && Fase3 < 250 ){
      digitalWrite(enable3, LOW);
      digitalWrite(direccion3, HIGH);
      for(i=0; i<30; i++){
        digitalWrite(pasos, HIGH);
        delay(10);
        digitalWrite(pasos, LOW);
        delay(10);
      }
    }
    if( Fase3 > LimSup3 && Fase3 > 0.0 ){  
      digitalWrite(enable3, LOW);
      digitalWrite(direccion3, LOW);
      for(i=0; i<30; i++){
        digitalWrite(pasos, HIGH);
        delay(10);
        digitalWrite(pasos, LOW);
        delay(10);
      }
    }

    digitalWrite(enable3, HIGH);
    
  }

  //------------------------------------
  
}


void controlManual(){

   //----Codigo para leer el encoder------------------------------



  //-------------------------------------------------------------

  if(ISRCounterM != ISRCounterMPrev){
    
    if(IsCW == 1){

      //-------control fase 1 sentido horario--------

      if(digitalRead(switchFase1)){

        digitalWrite(enable1, LOW);
        digitalWrite(direccion1, HIGH);

       for(i=0; i<30; i++){
        digitalWrite(pasos, HIGH);
        delay(10);
        digitalWrite(pasos, LOW);
        delay(10);
       }
        
      }

      //---------------------------------------------

      //-------control fase 2 sentido horario--------

      if(digitalRead(switchFase2)){

        digitalWrite(enable2, LOW);
        digitalWrite(direccion2, HIGH);

       for(i=0; i<30; i++){
        digitalWrite(pasos, HIGH);
        delay(10);
        digitalWrite(pasos, LOW);
        delay(10);
       }
        
      }

      //---------------------------------------------

      //-------control fase 3 sentido horario--------

      if(digitalRead(switchFase3)){

        digitalWrite(enable3, LOW);
        digitalWrite(direccion3, HIGH);

       for(i=0; i<30; i++){
        digitalWrite(pasos, HIGH);
        delay(10);
        digitalWrite(pasos, LOW);
        delay(10);
       }
        
      }

      //---------------------------------------------

      
    }

    else{

      //-----control fase 1 sentido antihorario------

      if(digitalRead(switchFase1)){

        digitalWrite(enable1, LOW);
        digitalWrite(direccion1, LOW);

       for(i=0; i<30; i++){
        digitalWrite(pasos, HIGH);
        delay(10);
        digitalWrite(pasos, LOW);
        delay(10);
       }
        
      }

      //---------------------------------------------

      //-----control fase 2 sentido antihorario------

      if(digitalRead(switchFase2)){

        digitalWrite(enable2, LOW);
        digitalWrite(direccion2, LOW);

       for(i=0; i<30; i++){
        digitalWrite(pasos, HIGH);
        delay(10);
        digitalWrite(pasos, LOW);
        delay(10);
       }
        
      }

      //---------------------------------------------

      //-----control fase 3 sentido antihorario------

      if(digitalRead(switchFase3)){

        digitalWrite(enable3, LOW);
        digitalWrite(direccion3, LOW);

       for(i=0; i<30; i++){
        digitalWrite(pasos, HIGH);
        delay(10);
        digitalWrite(pasos, LOW);
        delay(10);
       }
        
      }

 
      
    }

    digitalWrite(enable1, HIGH);
    digitalWrite(enable2, HIGH);
    digitalWrite(enable3, HIGH);
    
  }

  ISRCounterMPrev = ISRCounterM;
  
}

void mostrarPatallaA(){
  
  //lcd.clear();
  //lcd.setCursor(0,0);
  //lcd.print("AUTOMATICO");
  lcd.setCursor(0,0);
  lcd.print("VF_1:");
  lcd.print(Fase1); 
  lcd.setCursor(0,1);
  lcd.print("Sp:");
  lcd.print(setPointFase1); 

  /* Serial.println("AUTOMATICO");
  Serial.print("Tension Fase1: ");
  Serial.println(Fase1);
  Serial.print("Set Point Fase1: ");
  Serial.println(setPointFase1);
  Serial.println();
  

  Serial.print("Tension Fase2: ");
  Serial.println(Fase2);
  Serial.print("Set Point Fase2: ");
  Serial.println(setPointFase2);
  Serial.println();
  
  Serial.print("Tension Fase3: ");
  Serial.println(Fase3);
  Serial.print("Set Point Fase3: ");
  Serial.println(setPointFase3);
  Serial.println(); */
 
}

void mostrarPatallaM(){
  Serial.println("MANUAL");
  Serial.print("Tension Fase1: ");
  Serial.println(Fase1);
  Serial.println();
  

/*  Serial.print("Tension Fase2: ");
  Serial.println(Fase2);
  Serial.println();
  
  Serial.print("Tension Fase3: ");
  Serial.println(Fase3);
  Serial.println();*/
 
}




void doEncodeA()
{
   if (millis() > timeCounter + timeThreshold)
   {
      if (digitalRead(channelPinA) == digitalRead(channelPinB))
      {
         IsCW = 1;
         if (ISRCounter + 1 <= maxSteps) ISRCounter++;
         ISRCounterM++;
      }
      else
      {
         IsCW = 2;
         if (ISRCounter - 1 > 0) ISRCounter--;
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
         if (ISRCounter + 1 <= maxSteps) ISRCounter++;
         ISRCounterM++;
      }
      else
      {
         IsCW = 2;
         if (ISRCounter - 1 > 0) ISRCounter--;
         ISRCounterM--;
      }
      timeCounter = millis();
   }
}

