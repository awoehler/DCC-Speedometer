/*
  Analog Input

  Demonstrates analog input by reading an analog sensor on analog pin 0 and
  turning on and off a light emitting diode(LED) connected to digital pin 13.
  The amount of time the LED will be on and off depends on the value obtained
  by analogRead().

  The circuit:
  - potentiometer
    center pin of the potentiometer to the analog input 0
    one side pin (either one) to ground
    the other side pin to +5V
  - LED
    anode (long leg) attached to digital output 13
    cathode (short leg) attached to ground

  - Note: because most Arduinos have a built-in LED attached to pin 13 on the
    board, the LED is optional.

  created by David Cuartielles
  modified 30 Aug 2011
  By Tom Igoe

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/AnalogInput
*/
float distanceMM = 33.10;
float distance = (distanceMM / 25.4 / 12 / 5280 );
float conversionFactor = distance * 1000 * 60 * 60 * 87;
int sensorPin = A0;    // select the input pin for the potentiometer
int ledPin = 13;      // select the pin for the LED
int sensorValue = 0;  // variable to store the value coming from the sensor
int valuesLength = 5;
int values[5];
int index = 0;
unsigned long startTime = 0;
int threshhold = 165;

void setup() {
  Serial.begin(115200);
  // declare the ledPin as an OUTPUT:
  pinMode(ledPin, OUTPUT);

  for(index=0;index < valuesLength; index++) {
    values[index] = analogRead(sensorPin);
  }
  index = 0;
  Serial.print("Avg: ");
  Serial.println(avg(values,valuesLength));
  Serial.print("Threshold: ");
  Serial.println(threshhold);
}

void loop() {
  // read the value from the sensor:
  values[index] = analogRead(sensorPin);
  if( avg(values,valuesLength) < threshhold ) {
    if( startTime == 0 ) {
      startTime = millis();
    }
  } else {
    //This only runs once after the detection has completed.
    if( startTime > 0 ) {
      unsigned long duration = millis() - startTime;
      Serial.print("Duration: ");
      Serial.println(duration);
      Serial.print("Speed: ");
      Serial.print( conversionFactor / duration );
      Serial.println("mph (Scale)");
      startTime = 0;
    }
  }
  //Set the index to the next slot to store the index value. Modulus division wraps the counter back to the beginning.
  index = (index++)%valuesLength;
}

float avg(int values[], int length){
  float total = 0;
  for(int i=0; i < length; i++ ) {
    total += values[i]/length;
  }
  return total/length;
}
