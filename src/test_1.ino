#include <ESP32Servo.h>

const int TRIG = 17;
const int ECHO = 16;
const int SERVO_PIN = 13;

Servo scanServo;

int SERVO_LEFT = 160;
int SERVO_FRONT = 90;
int SERVO_RIGHT = 20;

long getDistanceRaw() 
{
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  long duration = pulseIn(ECHO, HIGH, 30000);
  if (duration == 0) return 0;

  return duration * 0.034 / 2;
}

int getStableDistance() 
{
  long d1 = getDistanceRaw();
  delay(20);
  long d2 = getDistanceRaw();
  delay(20);
  long d3 = getDistanceRaw();

  if (d1 == 0) d1 = 200;
  if (d2 == 0) d2 = 200;
  if (d3 == 0) d3 = 200;

  return (d1 + d2 + d3) / 3;
}

void setup() 
{
  Serial.begin(115200);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  scanServo.attach(SERVO_PIN);
  delay(500);
}

void loop() 
{
  scanServo.write(SERVO_LEFT);
  delay(500);
  int leftDist = getStableDistance();

  scanServo.write(SERVO_FRONT);
  delay(500);
  int frontDist = getStableDistance();

  scanServo.write(SERVO_RIGHT);
  delay(500);
  int rightDist = getStableDistance();

  Serial.print("LEFT = ");
  Serial.print(leftDist);
  Serial.print(" | FRONT = ");
  Serial.print(frontDist);
  Serial.print(" | RIGHT = ");
  Serial.println(rightDist);

  delay(1000);
}
