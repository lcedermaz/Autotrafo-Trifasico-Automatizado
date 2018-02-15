// EmonLibrary examples openenergymonitor.org, Licence GNU GPL V3

#include "EmonLib.h"             // Include Emon Library
EnergyMonitor emon1;             // Create an instance

#define LED 5
int ledState = LOW;

int i = 0;
float tenvals = 0.0;
float minval = 1000;
float maxval = 0.0;

//---------TIEMPOS----------------
//------------------------------T1
unsigned long previousMillis1 = 0;
const long interval1 = 100 ;
//------------------------------T2
unsigned long previousMillis2 = 0 ;
const long interval2 = 250;



void setup()
{
  Serial.begin(9600);
  pinMode (LED, OUTPUT);
  digitalWrite (LED, LOW);
  emon1.voltage(0, 352 , 1.7);  // Voltage: input pin, calibration, phase_shift
  //  emon1.current(1, 111.1);       // Current: input pin, calibration.
}

void loop()
{

  unsigned long currentMillis = millis();

  // Gestionar el desbordamiento
  if ((unsigned long)(currentMillis - previousMillis1) >= interval1)
  {
    SensorVoltaje ();
    previousMillis1 = millis();
  }

  if ((unsigned long)(currentMillis - previousMillis2) >= interval2)
  {
    switchLed();
    previousMillis2 =  millis();
  }

}

void switchLed()
{
  if (ledState == LOW)
    ledState = HIGH;
  else
    ledState = LOW;
  digitalWrite(LED, ledState);
}

void SensorVoltaje ()
{
  emon1.calcVI(20, 2000);        // Calculate all. No.of half wavelengths (crossings), time-out
  //  emon1.serialprint();           // Print out all variables (realpower, apparent power, Vrms, Irms, power factor)

  //  float realPower       = emon1.realPower;        //extract Real Power into variable
  //  float apparentPower   = emon1.apparentPower;    //extract Apparent Power into variable
  //  float powerFActor     = emon1.powerFactor;      //extract Power Factor into Variable
  //  float supplyVoltage   = emon1.Vrms;             //extract Vrms into Variable
  //  float Irms            = emon1.Irms;             //extract Irms into Variable

  float Vrms = (emon1.Vrms - 3);
  if (Vrms < 0) {
    Vrms = 0.0;
  }

  tenvals += Vrms;
  if (minval > Vrms) {
    minval = Vrms;
  }
  if (maxval < Vrms) {
    maxval = Vrms;
  }

  i++;

  if (i == 10)
  {
    Serial.print("Avg: ");
    Serial.print((tenvals / 10) + ((240 - (tenvals / 10)) * 0.015)); // Se le aplico una formula de correcion para un error de 3 volt en 40Vca
    Serial.print(" (");
    Serial.print(Vrms);
    Serial.print(") Min: ");
    Serial.print(minval);
    Serial.print(" Max: ");
    Serial.println(maxval);

    i = 0;
    tenvals = 0.0;
    minval = 1000.0;
    maxval = 0.0;
  }
}
