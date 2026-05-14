#include <ESP32Servo.h>

#define SERVO_PIN 13

Servo head;

void setup() 
{
  head.attach(SERVO_PIN);
  delay(1000);
}

void loop() 
{
  head.write(90);
  delay(1500);

  head.write(160);
  delay(1500);

  head.write(90);
  delay(1500);

  head.write(20);
  delay(1500);
}
