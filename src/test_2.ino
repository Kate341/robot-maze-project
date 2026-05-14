const int L1 = 33;
const int L2 = 32;
const int L3 = 27;
const int L4 = 26;

const int R1 = 22;
const int R2 = 18;
const int R3 = 19;
const int R4 = 21;

int TURN_90_TIME = 120;

void leftSpin() 
{
  digitalWrite(L1, LOW);  digitalWrite(L2, HIGH);
  digitalWrite(L3, LOW);  digitalWrite(L4, HIGH);

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

  Stop();
  delay(1000);
}

void loop() 
{
  Serial.println("LEFT TURN");
  leftSpin();
  delay(TURN_90_TIME);
  Stop();
  while (true) {}
}
