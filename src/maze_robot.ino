#include <ESP32Servo.h>

// ========== MOTOR PINS ==========
const int L1 = 33;
const int L2 = 32;
const int L3 = 27;
const int L4 = 26;

const int R1 = 22;
const int R2 = 18;
const int R3 = 19;
const int R4 = 21;

// ========== ULTRASONIC ==========
const int TRIG = 17;
const int ECHO = 16;

// ========== SERVO ==========
const int SERVO_PIN = 13;
Servo scanServo;

// ======= YOUR CALIBRATED ANGLES =======
const int SERVO_LEFT  = 160;
const int SERVO_FRONT = 90;
const int SERVO_RIGHT = 20;

// ======= YOUR CALIBRATED DISTANCES =======
const int FRONT_STOP_DIST = 15;
const int SIDE_OPEN_DIST  = 16;
const int FRONT_OPEN_DIST = 20;

// ======= YOUR CALIBRATED TIMES =======
const int FORWARD_STEP = 80;
const int AFTER_TURN_FORWARD = 120;

const int TURN_LEFT_TIME  = 150;
const int TURN_RIGHT_TIME = 130;
const int UTURN_TIME      = 340;

// ========== STATES ==========
enum RobotState {
  MOVE_FORWARD,
  SCAN_AND_DECIDE,
  TURN_LEFT_STATE,
  TURN_RIGHT_STATE,
  UTURN_STATE
};

RobotState state = MOVE_FORWARD;

// =====================================================
// SETUP
// =====================================================
void setup() {
  pinMode(L1, OUTPUT);
  pinMode(L2, OUTPUT);
  pinMode(L3, OUTPUT);
  pinMode(L4, OUTPUT);

  pinMode(R1, OUTPUT);
  pinMode(R2, OUTPUT);
  pinMode(R3, OUTPUT);
  pinMode(R4, OUTPUT);

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  Serial.begin(115200);

  scanServo.attach(SERVO_PIN);
  scanServo.write(SERVO_FRONT);
  delay(800);

  Stop();
  delay(500);
}

// =====================================================
// LOOP
// =====================================================
void loop() {
  switch (state) {
    case MOVE_FORWARD:
      moveForwardMode();
      break;

    case SCAN_AND_DECIDE:
      scanAndDecide();
      break;

    case TURN_LEFT_STATE:
      doLeftTurn();
      break;

    case TURN_RIGHT_STATE:
      doRightTurn();
      break;

    case UTURN_STATE:
      doUTurn();
      break;
  }
}

// =====================================================
// FORWARD MODE
// =====================================================
void moveForwardMode() {
  scanServo.write(SERVO_FRONT);
  delay(70);

  int frontDist = getStableDistance();

  Serial.print("MOVE | front = ");
  Serial.println(frontDist);

  if (frontDist == 0 || frontDist <= FRONT_STOP_DIST) {
  Stop();
  delay(60);
  state = SCAN_AND_DECIDE;
  return;
  }

  forward();
  delay(FORWARD_STEP);
  Stop();
  delay(15);
}

// =====================================================
// SCAN AND DECIDE
// =====================================================
void scanAndDecide() {
  Stop();
  delay(80);

  int leftDist  = lookLeft();
  int frontDist = lookFront();
  int rightDist = lookRight();

  Serial.print("SCAN | left = ");
  Serial.print(leftDist);
  Serial.print(" | front = ");
  Serial.print(frontDist);
  Serial.print(" | right = ");
  Serial.println(rightDist);

  // left -> straight -> right -> u-turn
  if (leftDist >= SIDE_OPEN_DIST) {
    state = TURN_LEFT_STATE;
  }
  else if (frontDist >= FRONT_OPEN_DIST) {
    state = MOVE_FORWARD;
  }
  else if (rightDist >= SIDE_OPEN_DIST) {
    state = TURN_RIGHT_STATE;
  }
  else {
    state = UTURN_STATE;
  }
}

// =====================================================
// ACTIONS
// =====================================================
void doLeftTurn() {
  Serial.println("ACTION | LEFT");

  scanServo.write(SERVO_FRONT);
  delay(50);

  leftSpin();
  delay(TURN_LEFT_TIME);
  Stop();
  delay(60);

  forward();
  delay(AFTER_TURN_FORWARD);
  Stop();
  delay(40);

  state = MOVE_FORWARD;
}

void doRightTurn() {
  Serial.println("ACTION | RIGHT");

  scanServo.write(SERVO_FRONT);
  delay(50);

  rightSpin();
  delay(TURN_RIGHT_TIME);
  Stop();
  delay(60);

  forward();
  delay(AFTER_TURN_FORWARD);
  Stop();
  delay(40);

  state = MOVE_FORWARD;
}

void doUTurn() {
  Serial.println("ACTION | UTURN");

  scanServo.write(SERVO_FRONT);
  delay(50);

  rightSpin();
  delay(UTURN_TIME);
  Stop();
  delay(80);

  forward();
  delay(AFTER_TURN_FORWARD);
  Stop();
  delay(40);

  state = MOVE_FORWARD;
}

// =====================================================
// LOOK FUNCTIONS
// =====================================================
int lookFront() {
  scanServo.write(SERVO_FRONT);
  delay(220);
  return getStableDistance();
}

int lookLeft() {
  scanServo.write(SERVO_LEFT);
  delay(220);
  return getStableDistance();
}

int lookRight() {
  scanServo.write(SERVO_RIGHT);
  delay(220);
  return getStableDistance();
}

// =====================================================
// DISTANCE
// =====================================================
int getStableDistance() {
  long d1 = getDistanceRaw();
  delay(12);
  long d2 = getDistanceRaw();
  delay(12);
  long d3 = getDistanceRaw();

  long sum = 0;
  int count = 0;

  if (d1 > 0) {
    sum += d1;
    count++;
  }

  if (d2 > 0) {
    sum += d2;
    count++;
  }

  if (d3 > 0) {
    sum += d3;
    count++;
  }

  // Если датчик вообще ничего не увидел 3 раза подряд
  if (count == 0) {
    return 0;   // безопаснее считать, что данные плохие
  }

  return (int)(sum / count);
}

long getDistanceRaw() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  long duration = pulseIn(ECHO, HIGH, 30000);
  if (duration == 0) return 0;

  long distance = duration * 0.034 / 2;
  return distance;
}

// =====================================================
// MOVEMENT
// =====================================================
void forward() {
  digitalWrite(L1, HIGH); digitalWrite(L2, LOW);
  digitalWrite(L3, HIGH); digitalWrite(L4, LOW);

  digitalWrite(R1, HIGH); digitalWrite(R2, LOW);
  digitalWrite(R3, HIGH); digitalWrite(R4, LOW);
}

void leftSpin() {
  digitalWrite(L1, LOW);  digitalWrite(L2, HIGH);
  digitalWrite(L3, LOW);  digitalWrite(L4, HIGH);

  digitalWrite(R1, HIGH); digitalWrite(R2, LOW);
  digitalWrite(R3, HIGH); digitalWrite(R4, LOW);
}

void rightSpin() {
  digitalWrite(L1, HIGH); digitalWrite(L2, LOW);
  digitalWrite(L3, HIGH); digitalWrite(L4, LOW);

  digitalWrite(R1, LOW);  digitalWrite(R2, HIGH);
  digitalWrite(R3, LOW);  digitalWrite(R4, HIGH);
}

void Stop() {
  digitalWrite(L1, LOW); digitalWrite(L2, LOW);
  digitalWrite(L3, LOW); digitalWrite(L4, LOW);

  digitalWrite(R1, LOW); digitalWrite(R2, LOW);
  digitalWrite(R3, LOW); digitalWrite(R4, LOW);
}
