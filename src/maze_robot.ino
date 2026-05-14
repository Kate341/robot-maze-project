/*
=====================================================
ESP32 MAZE SOLVING ROBOT
Author: Tokhtamyssova Amina

Platform:
- ESP32
- Arduino IDE
=====================================================
*/

#include <ESP32Servo.h>

// =====================================================
// MOTOR PIN CONFIGURATION
// =====================================================
// Left motor driver pins
const int L1 = 33;
const int L2 = 32;
const int L3 = 27;
const int L4 = 26;

// Right motor driver pins
const int R1 = 22;
const int R2 = 18;
const int R3 = 19;
const int R4 = 21;

// =====================================================
// ULTRASONIC SENSOR PINS
// =====================================================
// TRIG sends ultrasonic pulse
// ECHO receives reflected signal
const int TRIG = 17;
const int ECHO = 16;

// =====================================================
// SERVO CONFIGURATION
// =====================================================
// Servo is used for directional scanning
const int SERVO_PIN = 13;
Servo scanServo;

// =====================================================
// SERVO ANGLE CALIBRATION
// =====================================================
// Calibrated angles for left/front/right scanning
const int SERVO_LEFT  = 160;
const int SERVO_FRONT = 90;
const int SERVO_RIGHT = 20;

// =====================================================
// DISTANCE THRESHOLDS
// =====================================================
// Minimum front distance before stopping
const int FRONT_STOP_DIST = 15;

// Minimum distance considered as an open side path
const int SIDE_OPEN_DIST  = 16;

// Minimum front distance considered safe for movement
const int FRONT_OPEN_DIST = 20;

// =====================================================
// MOVEMENT TIMING CALIBRATION
// =====================================================
// Forward movement step duration
const int FORWARD_STEP = 80;

// Small forward correction after turning
const int AFTER_TURN_FORWARD = 120;

// Rotation timing calibration
const int TURN_LEFT_TIME  = 150;
const int TURN_RIGHT_TIME = 130;
const int UTURN_TIME      = 340;

// =====================================================
// FINITE STATE MACHINE (FSM)
// =====================================================
// Robot operating states
enum RobotState {

  // Normal forward movement
  MOVE_FORWARD,

  // Scan environment and choose direction
  SCAN_AND_DECIDE,

  // Perform left turn
  TURN_LEFT_STATE,

  // Perform right turn
  TURN_RIGHT_STATE,

  // Perform 180-degree turn in dead ends
  UTURN_STATE
};

// Initial robot state
RobotState state = MOVE_FORWARD;

// =====================================================
// SETUP FUNCTION
// =====================================================
// Runs once during startup
void setup() {

  // Configure left motor pins
  pinMode(L1, OUTPUT);
  pinMode(L2, OUTPUT);
  pinMode(L3, OUTPUT);
  pinMode(L4, OUTPUT);

  // Configure right motor pins
  pinMode(R1, OUTPUT);
  pinMode(R2, OUTPUT);
  pinMode(R3, OUTPUT);
  pinMode(R4, OUTPUT);

  // Configure ultrasonic sensor pins
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  // Initialize serial monitor
  Serial.begin(115200);

  // Attach servo and center it
  scanServo.attach(SERVO_PIN);
  scanServo.write(SERVO_FRONT);

  delay(800);

  // Ensure robot is stopped at startup
  Stop();
  delay(500);
}

// =====================================================
// MAIN LOOP
// =====================================================
// Main finite state machine controller
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
// FORWARD MOVEMENT MODE
// =====================================================
// Robot moves forward until obstacle is detected
void moveForwardMode() {

  // Keep servo facing forward
  scanServo.write(SERVO_FRONT);
  delay(70);

  // Measure front distance
  int frontDist = getStableDistance();

  // Debug output
  Serial.print("MOVE | front = ");
  Serial.println(frontDist);

  // If obstacle detected -> stop and scan
  if (frontDist == 0 || frontDist <= FRONT_STOP_DIST) {

    Stop();
    delay(60);

    state = SCAN_AND_DECIDE;
    return;
  }

  // Continue forward movement
  forward();
  delay(FORWARD_STEP);

  Stop();
  delay(15);
}

// =====================================================
// ENVIRONMENT SCANNING AND DECISION MAKING
// =====================================================
// Robot scans left/front/right and selects direction
void scanAndDecide() {

  Stop();
  delay(80);

  // Scan all directions
  int leftDist  = lookLeft();
  int frontDist = lookFront();
  int rightDist = lookRight();

  // Debug information
  Serial.print("SCAN | left = ");
  Serial.print(leftDist);

  Serial.print(" | front = ");
  Serial.print(frontDist);

  Serial.print(" | right = ");
  Serial.println(rightDist);

  // Navigation priority:
  // left -> forward -> right -> u-turn

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

    // Dead end detected
    state = UTURN_STATE;
  }
}

// =====================================================
// TURNING ACTIONS
// =====================================================

// Perform left rotation
void doLeftTurn() {

  Serial.println("ACTION | LEFT");

  scanServo.write(SERVO_FRONT);
  delay(50);

  leftSpin();
  delay(TURN_LEFT_TIME);

  Stop();
  delay(60);

  // Small forward correction
  forward();
  delay(AFTER_TURN_FORWARD);

  Stop();
  delay(40);

  state = MOVE_FORWARD;
}

// Perform right rotation
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

// Perform 180-degree turn
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
// SERVO LOOK FUNCTIONS
// =====================================================

// Scan front direction
int lookFront() {

  scanServo.write(SERVO_FRONT);
  delay(220);

  return getStableDistance();
}

// Scan left direction
int lookLeft() {

  scanServo.write(SERVO_LEFT);
  delay(220);

  return getStableDistance();
}

// Scan right direction
int lookRight() {

  scanServo.write(SERVO_RIGHT);
  delay(220);

  return getStableDistance();
}

// =====================================================
// DISTANCE MEASUREMENT SYSTEM
// =====================================================

// Take multiple measurements and average valid values
int getStableDistance() {

  long d1 = getDistanceRaw();
  delay(12);

  long d2 = getDistanceRaw();
  delay(12);

  long d3 = getDistanceRaw();

  long sum = 0;
  int count = 0;

  // Validate first reading
  if (d1 > 0) {
    sum += d1;
    count++;
  }

  // Validate second reading
  if (d2 > 0) {
    sum += d2;
    count++;
  }

  // Validate third reading
  if (d3 > 0) {
    sum += d3;
    count++;
  }

  // No valid readings detected
  if (count == 0) {

    return 0;
  }

  // Return averaged distance
  return (int)(sum / count);
}

// Raw ultrasonic distance measurement
long getDistanceRaw() {

  // Generate ultrasonic pulse
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);

  digitalWrite(TRIG, LOW);

  // Measure echo duration
  long duration = pulseIn(ECHO, HIGH, 30000);

  // Timeout protection
  if (duration == 0) return 0;

  // Convert time to distance
  long distance = duration * 0.034 / 2;

  return distance;
}

// =====================================================
// MOTOR CONTROL FUNCTIONS
// =====================================================

// Move robot forward
void forward() {

  digitalWrite(L1, HIGH);
  digitalWrite(L2, LOW);

  digitalWrite(L3, HIGH);
  digitalWrite(L4, LOW);

  digitalWrite(R1, HIGH);
  digitalWrite(R2, LOW);

  digitalWrite(R3, HIGH);
  digitalWrite(R4, LOW);
}

// Rotate robot left
void leftSpin() {

  digitalWrite(L1, LOW);
  digitalWrite(L2, HIGH);

  digitalWrite(L3, LOW);
  digitalWrite(L4, HIGH);

  digitalWrite(R1, HIGH);
  digitalWrite(R2, LOW);

  digitalWrite(R3, HIGH);
  digitalWrite(R4, LOW);
}

// Rotate robot right
void rightSpin() {

  digitalWrite(L1, HIGH);
  digitalWrite(L2, LOW);

  digitalWrite(L3, HIGH);
  digitalWrite(L4, LOW);

  digitalWrite(R1, LOW);
  digitalWrite(R2, HIGH);

  digitalWrite(R3, LOW);
  digitalWrite(R4, HIGH);
}

// Stop all motors
void Stop() {

  digitalWrite(L1, LOW);
  digitalWrite(L2, LOW);

  digitalWrite(L3, LOW);
  digitalWrite(L4, LOW);

  digitalWrite(R1, LOW);
  digitalWrite(R2, LOW);

  digitalWrite(R3, LOW);
  digitalWrite(R4, LOW);
}
