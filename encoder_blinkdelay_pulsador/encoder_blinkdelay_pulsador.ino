#define Led_ROJO 4
#define Led_VERDE 5
#define Led_Pulsador  6
#define Pulsador  7

//---------BLINK DELAY----------------
//------------------------------blink1

unsigned long previousMillis = 0;
const long interval = 10 ;

//------------------------------blink2

unsigned long previousMillis1 = 0 ;
const long interval1 = 2000;

//------------------------------blink3

unsigned long previousMillis2 ;
const long interval2 = 2000 ;

//------------------------------

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
  pinMode(Led_ROJO, OUTPUT);
  pinMode(Led_VERDE, OUTPUT);
  pinMode(Led_Pulsador, OUTPUT);
  pinMode(Pulsador, INPUT);

  pinMode(channelPinA, INPUT_PULLUP);
  pinMode(channelPinB, INPUT_PULLUP);
  
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(channelPinA), doEncodeA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(channelPinB), doEncodeB, CHANGE);
}

void loop() {

  
if (counter != ISRCounter)
  {
    counter = ISRCounter;
    Serial.println(counter);
}
  if (digitalRead(Pulsador)) {
    Motor1();
    digitalWrite (Led_Pulsador, HIGH);
  } else {
    digitalWrite (Led_Pulsador, LOW);
  }
}


void Motor1 () {

  unsigned long currentMillis = millis();

  // Gestionar el desbordamiento
  if ((unsigned long)(currentMillis - previousMillis1) >= interval1)
  {
    if (counter != ISRCounter && IsCW == true ) {
      Subir();
      previousMillis1 = millis();
    }

    if ((unsigned long)(currentMillis - previousMillis2) >= interval2)
    {
      if ( counter != ISRCounter && IsCW == false) {
        Bajar();
        previousMillis2 = millis();
      }

      if ((unsigned long)(currentMillis - previousMillis) >= interval)
      {
        apagarLeds ();
        previousMillis = millis();
      }

    }
  }
  counter = ISRCounter;
}


void Subir () {

  digitalWrite (Led_ROJO, HIGH); // Simulando el enable
  digitalWrite (Led_VERDE, LOW); // Simulando direccion
}

void Bajar () {
  digitalWrite (Led_ROJO, HIGH);
  digitalWrite (Led_VERDE, HIGH);
}

void apagarLeds () {
  digitalWrite (Led_ROJO, LOW);
  digitalWrite (Led_VERDE, LOW);
}


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
