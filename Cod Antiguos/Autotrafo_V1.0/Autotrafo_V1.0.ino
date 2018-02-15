  float setPointFase1, setPointFase2, setPointFase3, Fase1, Fase2, Fase3;

  int lectura1, lectura2, lectura3, i;

  int CanalA = 2;
  int CanalB = 3;
  
  int switchFase1 = 4;
  int switchFase2 = 5;
  int switchFase3 = 6;

  int pasos = 7;
  
  int direccion1 = 8;
  int enable1 = 9;

  int direccion2 = 10;
  int enable2 = 11;

  int direccion3 = 12;
  int enable3 = 13;

  //----Configuraciones del encoder----
  
  unsigned char estadoCanalA;
  unsigned char estadoCanalB;
  unsigned char estadoPrevioCanalA = 0;

  const int maxPasos = 1023;
  int valorPrevio;
  int valor;

  const int tiempoLimite = 5; 
  unsigned long tiempoActual;
  unsigned long tiempoBucle;
  
  bool sentidoReloj = true;

  //-------------------------------------
  

  void setPoint();
  void leerTension();
  void ajuste();
  

  
void setup() {
  // put your setup code here, to run once:
  pinMode(CanalA, INPUT);
  pinMode(CanalB, INPUT);
  
  pinMode(switchFase1, INPUT);
  pinMode(switchFase2, INPUT);
  pinMode(switchFase3, INPUT);
  
  Serial.begin(9600);

  pinMode(pasos, OUTPUT);
  
  pinMode(enable1, OUTPUT);
  pinMode(direccion1, OUTPUT);

  pinMode(enable2, OUTPUT);
  pinMode(direccion2, OUTPUT);

  pinMode(enable3, OUTPUT);
  pinMode(direccion3, OUTPUT);


  digitalWrite(enable1, HIGH);
  digitalWrite(enable2, HIGH);
  digitalWrite(enable3, HIGH);

  //----Configuraciones del encoder----

  tiempoActual = millis();
  tiempoBucle = tiempoActual;
  valor = 0;
  valorPrevio = 0;

  //----------------------------------

}

void loop() {
  // put your main code here, to run repeatedly:

      setPoint();

      leerTension();
 
      ajuste();

      mostrarPatalla();


}

void setPoint(){

  //----Codigo para leer el encoder------------------------------

  tiempoActual = millis();
  if (tiempoActual >= (tiempoBucle + tiempoLimite))
  {
    estadoCanalA = digitalRead(CanalA);
    estadoCanalB = digitalRead(CanalB);
    if (estadoCanalA != estadoPrevioCanalA)  // Para precision simple if((!stateChannelA) && (prevStateChannelA))
    {
      if (estadoCanalB) // B es HIGH, es CW
      {
        bool sentidoReloj = true;
        if (valor + 1 <= maxPasos) valor++; // Asegurar que no sobrepasamos maxSteps
      }
      else  // B es LOW, es CWW
      {
        bool sentidoReloj = false;
        if (valor - 1 >= 0) valor--; // Asegurar que no tenemos negativos
      }

    }
    estadoPrevioCanalA = estadoCanalA;  // Guardar valores para siguiente

    // Si ha cambiado el valor, mostrarlo
    if (valorPrevio != valor)
    {
      valorPrevio = valor;
    }

    tiempoBucle = tiempoActual;  // Actualizar tiempo
  }

  //-------------------------------------------------------------


  //Dependiendo los switch activos el valor se asiganara al setPoint correspoediente

  if(digitalRead(switchFase1)){
    setPointFase1 = valor * 250.0 / 1024.0;
  }

  if(digitalRead(switchFase2)){
    setPointFase2 = valor * 250.0 / 1024.0;
  }

  if(digitalRead(switchFase3)){
    setPointFase3 = valor * 250.0 / 1024.0;
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
  if( Fase1 != setPointFase1 ){
    if( Fase1 < setPointFase1 && Fase1 < 250.0 ){
      digitalWrite(enable1, LOW);
      digitalWrite(direccion1, HIGH);
      for(i=0; i<30; i++){
        digitalWrite(pasos, HIGH);
        delay(10);
        digitalWrite(pasos, LOW);
        delay(10);
      }
    }
    if( Fase1 > setPointFase1 && Fase1 > 0.0 ){
      digitalWrite(enable1, LOW);
      digitalWrite(direccion1, LOW);
      for(i=0; i<30; i++){
        digitalWrite(pasos, HIGH);
        delay(10);
        digitalWrite(pasos, LOW);
        delay(10);
      }
    }

    digitalWrite(enable1, HIGH);
    
  }

  
  if( Fase2 != setPointFase2 ){
    if( Fase2 < setPointFase1 && Fase2 < 250.0 ){
      digitalWrite(enable2, LOW);
      digitalWrite(direccion2, HIGH);
      for(i=0; i<30; i++){
        digitalWrite(pasos, HIGH);
        delay(10);
        digitalWrite(pasos, LOW);
        delay(10);
      }
    }
    if( Fase2 > setPointFase2 && Fase2 > 0.0 ){
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


  if( Fase3 != setPointFase3 ){
    if( Fase3 < setPointFase3 && Fase3 < 250.0 ){
      digitalWrite(enable3, LOW);
      digitalWrite(direccion3, HIGH);
      for(i=0; i<30; i++){
        digitalWrite(pasos, HIGH);
        delay(10);
        digitalWrite(pasos, LOW);
        delay(10);
      }
    }
    if( Fase3 > setPointFase3 && Fase3 > 0.0 ){
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

  
}

void mostrarPatalla(){
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
  Serial.println();
 
}

