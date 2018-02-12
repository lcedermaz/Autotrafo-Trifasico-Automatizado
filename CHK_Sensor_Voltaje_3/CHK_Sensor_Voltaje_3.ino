const int sensorPin = A0;
int sensorValue;
float value;

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  sensorValue = analogRead(sensorPin);
  value = fmap(sensorValue, 0, 1023, -426.2080, +426.2080);

  Serial.println(value);

  delay(1000);
}

// cambio de escala entre floats
float fmap(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
