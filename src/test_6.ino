#define L1 33
#define L2 32
#define L3 27
#define L4 26

#define R1 22
#define R2 18
#define R3 19
#define R4 21

void setup() 
{
  pinMode(L1, OUTPUT); pinMode(L2, OUTPUT);
  pinMode(L3, OUTPUT); pinMode(L4, OUTPUT);
  pinMode(R1, OUTPUT); pinMode(R2, OUTPUT);
  pinMode(R3, OUTPUT); pinMode(R4, OUTPUT);
}

void stopRobot() 
{
  digitalWrite(L1, LOW); digitalWrite(L2, LOW);
  digitalWrite(L3, LOW); digitalWrite(L4, LOW);
  digitalWrite(R1, LOW); digitalWrite(R2, LOW);
  digitalWrite(R3, LOW); digitalWrite(R4, LOW);
}

void moveForward() 
{
  digitalWrite(L1, HIGH); digitalWrite(L2, LOW);
  digitalWrite(L3, HIGH); digitalWrite(L4, LOW);

  digitalWrite(R1, HIGH); digitalWrite(R2, LOW);
  digitalWrite(R3, HIGH); digitalWrite(R4, LOW);
}

void loop() 
{
  moveForward();
  delay(1000);
  stopRobot();
  delay(1000);
}
