#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
//SCREEN
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//IR
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
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Clear the buffer
  display.clearDisplay();
  display.display();

  for(index=0;index < valuesLength; index++) {
    values[index] = analogRead(sensorPin);
  }
  index = 0;
  threshhold = avg(values,valuesLength) - 30;
  Serial.print("Avg: ");
  Serial.println(avg(values,valuesLength));
  Serial.print("Threshold: ");
  Serial.println(threshhold);

  display.clearDisplay();

  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font

  display.println("Threshhold:");
  display.println(threshhold);
  display.println("Avg:");
  display.println(avg(values,valuesLength));
  display.display();

}

void loop() {
  // read the value from the sensor:
  values[index] = analogRead(sensorPin);
  float  average = avg(values,valuesLength);
  //Serial.println(average);
  if( average < threshhold ) {
    if( startTime == 0 ) {
      startTime = millis();
    }
  } else {
    //This only runs once after the detection has completed.
    if( startTime > 0 ) {
      unsigned long duration = millis() - startTime;
//      Serial.print("Duration: ");
//      Serial.println(duration);
//      Serial.print("Speed: ");
//      Serial.print( conversionFactor / duration );
//      Serial.println("mph (Scale)");

      display.clearDisplay();    
      display.setCursor(0, 0);     // Start at top-left corner

      display.println("Speed (scale mph):");
      display.println(conversionFactor / duration);
      display.println("duration:");
      display.println(duration);
      display.display();
      
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
