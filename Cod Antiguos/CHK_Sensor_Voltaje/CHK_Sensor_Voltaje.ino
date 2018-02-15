 


/* http://www.nyebarilmu.com */

const int analogInPin = A0; // inisialisasi pin A0 sebagai pembacaan sensor
const int analogOutPin = 3; // inisialisasi pin 3 sebagai pin output

int sensorValue = 00;        // value read from the pot
int outputValue = 00;        // value output to the PWM (analog out)
int voltageOut= 00;
int voltage= 00;

void setup() {
  // inisialisasi komunikasi serial pada 960bps
  Serial.begin(9600); 
}

void loop() {
  // Prosedur pembacaan analog pin
  sensorValue = analogRead(analogInPin);            
  // mapping pembacaan resolusi sensor
  outputValue = map(sensorValue, 00, 1023, 00, 255);  
  // mengubah nilai analog out
  analogWrite(analogOutPin, outputValue);    
 voltageOut = (sensorValue / 1023)*5 ;  
 voltage = (voltageOut+1.78)/0.001;   

  // Mencetak hasil ke monitor serial:
  Serial.print("sensor = " );                       
  Serial.print(sensorValue);      
  Serial.print("output = ");      
  Serial.println(outputValue);   
  Serial.print("Voltage ADC = ");      
  Serial.println(voltageOut);
  Serial.print("Voltage = ");      
  Serial.println(voltage);


  // diberikan nilai tunda 2 ms untuk pembacaan berikutnya
  // Menampilkan pembacaan analog-to-digital converter setelah pembacaan terakhir
  delay(500);                     
}

