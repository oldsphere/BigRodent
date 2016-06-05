/*
  - TEST ENCODER (SIMPLE) -
  Count the number of pulses and return the results over the serial port.
  This code is a simplified version from testEncoder.ard, which also consider
  different sample times simultaneously.

  Author: Carlos Rubio Abujas <oldsphere@gmail.com>
  Date: 28/102015
*/

#define ENCODER_PIN 3
#define SAMPLETIME 1000
#define NPULSE 100

volatile int pulses = 0;
float time = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("Serial initialized");
  
  Serial.print("Setting encoder interription...");
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN), countPulse, RISING);
  Serial.println("OK"); 
}

void loop() {

  Serial.print("time=");
  Serial.print(time);
  Serial.print(" s omega=");
  Serial.print(calcOmega(pulses,SAMPLETIME));
  Serial.print(" rad/s  nPulses=");
  Serial.println(pulses);
  
  pulses = 0;
  time += SAMPLETIME/1000.0;
  
  delay(SAMPLETIME);
}

float calcOmega(float puls,float sampleTime) {
  // Calculates the radial velocity [rad/s]
  return (puls * (2*PI/NPULSE)) / (sampleTime/1000);  
}

void countPulse() {
   pulses++; 
}
