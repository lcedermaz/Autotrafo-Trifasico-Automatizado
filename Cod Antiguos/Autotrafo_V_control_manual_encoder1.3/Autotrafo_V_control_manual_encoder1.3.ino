
//--------Asignaciones de pines-------
//-------Interruptores de Fases
#define SW_Fase1 22
#define SW_Fase2 23
#define SW_Fase3 24

//------Finales de carreras
#define Fcs_1 33
#define Fci_1 34

#define Fcs_2 35
#define Fci_2 36

#define Fcs_3 37
#define Fci_3 38

//---------Drivers PaP
#define Dir_1 27
#define En_1 28

#define Dir_2 29
#define En_2 30

#define Dir_3 31
#define En_3 32

//---------Tiempos no bloqueantes----------------
//------------------------------Blink1
unsigned long previousMillis = 0;
const long interval = 1000 ;
//------------------------------blink2
unsigned long previousMillis2 = 0 ;
const long interval2 = 1000;
//------------------------------blink3
unsigned long previousMillis3 ;
const long interval3= 100 ;

//---------Encoder

const int channelPinA = 2;
const int channelPinB = 3;

const int timeThreshold = 5;
long timeCounter = 0;

const int maxSteps = 255;
volatile int ISRCounter = 0;
int counter = 0;


bool IsCW = true;

void setup()
{
  //-----------Configuracion de Entradas
  pinMode(SW_Fase1, INPUT_PULLUP);
  pinMode(SW_Fase1, INPUT_PULLUP);
  pinMode(SW_Fase1, INPUT_PULLUP);

  pinMode(Fcs_1, INPUT);
  pinMode(Fci_1, INPUT);

  pinMode(Fcs_2, INPUT);
  pinMode(Fci_2, INPUT);

  pinMode(Fcs_3, INPUT);
  pinMode(Fci_3, INPUT);

  //-----------Configuracion de Salidas
  pinMode(Dir_1, OUTPUT);
  pinMode(En_1, OUTPUT);

  pinMode(Dir_2, OUTPUT);
  pinMode(En_2, OUTPUT);

  pinMode(Dir_3, OUTPUT);
  pinMode(En_3, OUTPUT);

  //----------Encoder
  pinMode(channelPinA, INPUT_PULLUP);
  pinMode(channelPinB, INPUT_PULLUP);

  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(channelPinA), doEncodeA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(channelPinB), doEncodeB, CHANGE);
}

void loop() {


  if (counter != ISRCounter)
  {
    counter = ISRCounter;    // Ver si funciona el encoder
    Serial.println(counter);
  }

  //--------Motor1-----------
  if (digitalRead(SW_Fase1) && digitalRead(Fcs_1) || digitalRead(Fci_1)) {
    Motor1();
  }

  //--------Motor2-----------
  if (digitalRead(SW_Fase2) && digitalRead(Fcs_2) || digitalRead(Fci_2)) {
    Motor2();
  }

  //--------Motor3-----------
  if (digitalRead(SW_Fase3) && digitalRead(Fcs_3) || digitalRead(Fci_3)) {
    Motor3();
  }

}

//*****************************
//----------MOTOR 1------------
//*****************************

void Motor1 () {

  unsigned long currentMillis = millis();

  // Gestionar el desbordamiento
  if ((unsigned long)(currentMillis - previousMillis) >= interval)
  {
    if (counter != ISRCounter && IsCW == true ) {
      Subir1();
      previousMillis = millis();
    }

    if ((unsigned long)(currentMillis - previousMillis2) >= interval2)
    {
      if ( counter != ISRCounter && IsCW == false) {
        Bajar1();
        previousMillis2 = millis();
      }

      if ((unsigned long)(currentMillis - previousMillis3) >= interval3)
      {
        Apagado1 ();
        previousMillis3 = millis();
      }

    }
  }
  counter = ISRCounter;
}

void Subir1 () {
  digitalWrite (Dir_1, HIGH);
  digitalWrite (En_1, LOW); // LOW= ACTIVO
}

void Bajar1 () {
  digitalWrite (Dir_1, LOW);
  digitalWrite (En_1, LOW);
}

void Apagado1 () {
  digitalWrite (En_1, HIGH);
}

//*****************************
//----------MOTOR 2------------
//*****************************


void Motor2 () {

  unsigned long currentMillis = millis();

  // Gestionar el desbordamiento
  if ((unsigned long)(currentMillis - previousMillis) >= interval)
  {
    if (counter != ISRCounter && IsCW == true ) {
      Subir2();
      previousMillis = millis();
    }

    if ((unsigned long)(currentMillis - previousMillis2) >= interval2)
    {
      if ( counter != ISRCounter && IsCW == false) {
        Bajar2();
        previousMillis2 = millis();
      }

      if ((unsigned long)(currentMillis - previousMillis3) >= interval3)
      {
        Apagado2 ();
        previousMillis3 = millis();
      }

    }
  }
  counter = ISRCounter;
}

void Subir2 () {
  digitalWrite (Dir_2, HIGH);
  digitalWrite (En_2, LOW); // LOW= ACTIVO
}

void Bajar2 () {
  digitalWrite (Dir_2, LOW);
  digitalWrite (En_2, LOW);
}

void Apagado2 () {
  digitalWrite (En_2, HIGH);
}


//*****************************
//----------MOTOR 3------------
//*****************************

void Motor3 () {

  unsigned long currentMillis = millis();

  // Gestionar el desbordamiento
  if ((unsigned long)(currentMillis - previousMillis) >= interval)
  {
    if (counter != ISRCounter && IsCW == true ) {
      Subir3();
      previousMillis = millis();
    }

    if ((unsigned long)(currentMillis - previousMillis2) >= interval2)
    {
      if ( counter != ISRCounter && IsCW == false) {
        Bajar3();
        previousMillis2 = millis();
      }

      if ((unsigned long)(currentMillis - previousMillis3) >= interval3)
      {
        Apagado3 ();
        previousMillis3 = millis();
      }

    }
  }
  counter = ISRCounter;
}

void Subir3 () {
  digitalWrite (Dir_3, HIGH);
  digitalWrite (En_3, LOW); // LOW= ACTIVO
}

void Bajar3 () {
  digitalWrite (Dir_3, LOW);
  digitalWrite (En_3, LOW);
}

void Apagado3 () {
  digitalWrite (En_3, HIGH);
}

//*****************************
//----------ENCODER------------
//*****************************

void doEncodeA()
{
  if (millis() > timeCounter + timeThreshold)
  {
    if (digitalRead(channelPinA) == digitalRead(channelPinB))
    {
      IsCW = true;

      if (ISRCounter + 1 <= maxSteps) ISRCounter++;
    }
    else
    {
      IsCW = false;
      if (ISRCounter - 1 > 0) ISRCounter--;
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
      if (ISRCounter + 1 <= maxSteps) ISRCounter++;
    }
    else
    {
      IsCW = false;
      if (ISRCounter - 1 > 0) ISRCounter--;
    }
    timeCounter = millis();
  }
}
