#include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>
#include <RF24_config.h>
#include "Filter.h"

#define trigPin 3
#define echoPin 4
#define ledGreen 10

RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";    // Address for RF Module

void setup() {
  // Configure Radio
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();

  Serial.begin(9600);     
  // Configure Ultrasonic Distance Sensor     
  pinMode(trigPin, OUTPUT);   
  pinMode(echoPin, INPUT);

  // Configure Indicators
  pinMode(ledGreen, OUTPUT);
}


void loop() {
  // Acquire Distance Measurement
  long duration, distance;
  digitalWrite(trigPin, LOW);  
  delayMicroseconds(2); 
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); 
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;

  if ((distance > 0) && (distance < 470)) {
    digitalWrite(ledGreen, HIGH); 
  } else {
    digitalWrite(ledGreen, LOW); 
  }

  if (distance > 500) {
    distance = 600;
  }
  // Print distance to serial
  Serial.print(distance);
  Serial.print(" ");

  //   Broadcast over RF  
  String distance2 = String(distance);
  const char text[5];
  distance2.toCharArray(text, 5);
  radio.write(&text, sizeof(text));
  delay(40);
}