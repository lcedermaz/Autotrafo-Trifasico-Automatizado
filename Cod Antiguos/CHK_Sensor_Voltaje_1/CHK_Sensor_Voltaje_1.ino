// EmonLibrary examples openenergymonitor.org, Licence GNU GPL V3

#include "EmonLib.h"             // Include Emon Library
EnergyMonitor emon1;             // Crear una instancia
//EnergyMonitor emon2;             // Crear una instancia
//EnergyMonitor emon3;             // Crear una instancia

 int i= 0;
 
 float voltaje1[5]={0};
 float suma, supplyVoltageL1;

 int set1=0;
 int error1=0;
// int error2=0;
// int error3=0;
void setup()
{  
 /* pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);*/
  Serial.begin(9600);
  
  emon1.voltage(4, 217.00, 1.7);  // Voltaje: pin de entrada , calibración , phase_shift
 // emon2.voltage(3, 217.26, 1.7);  // Voltaje: pin de entrada , calibración , phase_shift
  //emon3.voltage(2, 219.26, 1.7);  // Voltaje: pin de entrada , calibración , phase_shift
  
}

void loop()
{
  Serial.println ("---------------------------------------------------------------------");
//  emon1.calcVI(20,2000);         // Calcular todos. longitudes de onda medias No.of ( cruces) , tiempo de espera
//  emon1.serialprint();       // Imprime todas las variables ( RealPower , potencia aparente , Vrms , Irms , factor de potencia )
  // the loop routine runs over and over again forever:

 
//  int sensorValue = analogRead(A5);
//  set1 = (sensorValue * (5.0 / 1023.0))*64;
//  float supplyVoltageL1   = emon1.Vrms;             // Extraer Vrms en Variable
  
  for(i=0; i<5; i++){
    emon1.calcVI(20,2000);
    voltaje1[i]= emon1.Vrms;
  }
  
  for(i=0; i<5; i++){
    suma = suma + voltaje1[i];
  }
  
 supplyVoltageL1 = suma / 5;
  
  //error1= supplyVoltageL1 - set1;
  //Serial.println (error1);
 /* if (error1>1)
  {
    digitalWrite(2,HIGH);
    digitalWrite(5,LOW);
  }
  
 else
 {
  digitalWrite(2,LOW);
  }
  if(error1<-1)
    {
       digitalWrite(2,LOW);
       digitalWrite(5,HIGH);
      }
      else
 {
  digitalWrite(5,LOW);
  }
    
    
*/      

//  Serial.print (" R--------------");
//  Serial.print("set  ");
  Serial.println(supplyVoltageL1); //set1
  
/*  emon2.calcVI(20,2000);         // Calculate all. No.of half wavelengths (crossings), time-out
  emon2.serialprint();       // Print out all variables (realpower, apparent power, Vrms, Irms, power factor)
  
  
  int supplyVoltageL2   = emon2.Vrms;             //extract Vrms into Variable

  error2= supplyVoltageL2 - set1;
  
  if (error2>1)
  {
    digitalWrite(3,HIGH);
    digitalWrite(7,LOW);
  }
  
 else
 {
  digitalWrite(3,LOW);
  }
  if(error2<-1)
    {
       digitalWrite(3,LOW);
       digitalWrite(7,HIGH);
      }
      else
 {
  digitalWrite(7,LOW);
  }


  Serial.print (" S--------------");
  Serial.print("set  ");
  Serial.println(set1);
  emon3.calcVI(20,2000);         // Calculate all. No.of half wavelengths (crossings), time-out
  emon3.serialprint();     // Print out all variables (realpower, apparent power, Vrms, Irms, power factor)
  
  
  int supplyVoltageL3   = emon3.Vrms;             //extract Vrms into Variable

  error3= supplyVoltageL3 - set1;
  
  if (error3>1)
  {
    digitalWrite(4,HIGH);
    digitalWrite(6,LOW);
  }
  
 else
 {
  digitalWrite(4,LOW);
  }
  if(error3<-1)
    {
       digitalWrite(4,LOW);
       digitalWrite(6,HIGH);
      }
      else
 {
  digitalWrite(6,LOW);
  }
  
  Serial.print (" T--------------");
  Serial.print("set  ");
  Serial.println(set1);
*/

delay(200);

}


