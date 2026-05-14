#include <ESP32Servo.h>

const int L1 = 33;
const int L2 = 32;
const int L3 = 27;
const int L4 = 26;

const int R1 = 22;
const int R2 = 18;
const int R3 = 19;
const int R4 = 21;

const int TRIG = 17;
const int ECHO = 16;
const int SERVO_PIN = 13;

Servo scanServo;

int SERVO_FRONT = 90;
int FRONT_STOP_DIST = 25;

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
  delay(15);
  long d2 = getDistanceRaw();
  delay(15);
  long d3 = getDistanceRaw();

  if (d1 == 0) d1 = 200;
  if (d2 == 0) d2 = 200;
  if (d3 == 0) d3 = 200;

  return (d1 + d2 + d3) / 3;
}

void forward() 
{
  digitalWrite(L1, HIGH); digitalWrite(L2, LOW);
  digitalWrite(L3, HIGH); digitalWrite(L4, LOW);

  digitalWrite(R1, HIGH); digitalWrite(R2, LOW);
  digitalWrite(R3, HIGH); digitalWrite(R4, LOW);
}

void Stop() 
{
  digitalWrite(L1, LOW); digitalWrite(L2, LOW);
  digitalWrite(L3, LOW); digitalWrite(L4, LOW);

  digitalWrite(R1, LOW); digitalWrite(R2, LOW);
  digitalWrite(R3, LOW); digitalWrite(R4, LOW);
}

void setup() 
{
  Serial.begin(115200);

  pinMode(L1, OUTPUT); pinMode(L2, OUTPUT);
  pinMode(L3, OUTPUT); pinMode(L4, OUTPUT);
  pinMode(R1, OUTPUT); pinMode(R2, OUTPUT);
  pinMode(R3, OUTPUT); pinMode(R4, OUTPUT);

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  scanServo.attach(SERVO_PIN);
  scanServo.write(SERVO_FRONT);
  delay(600);
}

void loop() 
{
  int d = getStableDistance();
  Serial.print("FRONT = ");
  Serial.println(d);

  if (d > FRONT_STOP_DIST) {
    forward();
  } else {
    Stop();
  }

  delay(50);
}
