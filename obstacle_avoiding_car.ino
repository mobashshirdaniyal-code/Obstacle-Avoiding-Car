#include <AFMotor.h>
#include <Servo.h>

// ===============================
// MOTOR SETUP
// ===============================

AF_DCMotor motor1(1);  // Front Left
AF_DCMotor motor2(2);  // Rear Left
AF_DCMotor motor3(3);  // Front Right
AF_DCMotor motor4(4);  // Rear Right

// ===============================
// ULTRASONIC SENSOR
// ===============================

#define TRIG_PIN A0
#define ECHO_PIN A5

// ===============================
// SERVO
// ===============================

Servo scanServo;

// Servo signal pin
// For L293D shield, Servo 1 is normally D10
#define SERVO_PIN 10

// ===============================
// SETTINGS
// ===============================

int motorSpeed = 180;
int obstacleDistance = 20;

// Servo angles
int centerAngle = 90;
int leftAngle = 150;
int rightAngle = 30;


// ===============================
// GET DISTANCE
// ===============================

long getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000);

  // No echo
  if (duration == 0) {
    return 400;
  }

  long distance = duration * 0.034 / 2;

  return distance;
}


// ===============================
// MOVE FORWARD
// ===============================

void moveForward() {
  motor1.setSpeed(motorSpeed);
  motor2.setSpeed(motorSpeed);
  motor3.setSpeed(motorSpeed);
  motor4.setSpeed(motorSpeed);

  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
}


// ===============================
// MOVE BACKWARD
// ===============================

void moveBackward() {
  motor1.setSpeed(motorSpeed);
  motor2.setSpeed(motorSpeed);
  motor3.setSpeed(motorSpeed);
  motor4.setSpeed(motorSpeed);

  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
}



// ===============================
// TURN RIGHT
// ===============================

void turnRight() {
  motor1.setSpeed(motorSpeed);
  motor2.setSpeed(motorSpeed);
  motor3.setSpeed(motorSpeed);
  motor4.setSpeed(motorSpeed);

  motor1.run(BACKWARD);
  motor2.run(BACKWARD);

  motor3.run(FORWARD);
  motor4.run(FORWARD);
}


// ===============================
// TURN LEFT
// ===============================

void turnLeft() {
  motor1.setSpeed(motorSpeed);
  motor2.setSpeed(motorSpeed);
  motor3.setSpeed(motorSpeed);
  motor4.setSpeed(motorSpeed);

  motor1.run(FORWARD);
  motor2.run(FORWARD);

  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
}



// ===============================
// STOP
// ===============================

void stopCar() {
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
}


// ===============================
// SCAN LEFT
// ===============================

long lookLeft() {
  scanServo.write(leftAngle);
  delay(500);

  long distance = getDistance();

  Serial.print("Left: ");
  Serial.print(distance);
  Serial.println(" cm");

  return distance;
}


// ===============================
// SCAN RIGHT
// ===============================

long lookRight() {
  scanServo.write(rightAngle);
  delay(500);

  long distance = getDistance();

  Serial.print("Right: ");
  Serial.print(distance);
  Serial.println(" cm");

  return distance;
}


// ===============================
// SET SERVO CENTER
// ===============================

void lookCenter() {
  scanServo.write(centerAngle);
  delay(300);
}


// ===============================
// SETUP
// ===============================

void setup() {
  Serial.begin(9600);

  // Ultrasonic
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Servo
  scanServo.attach(SERVO_PIN);

  // Put servo in center
  scanServo.write(centerAngle);

  // Stop motors
  stopCar();

  delay(1000);
}


// ===============================
// MAIN LOOP
// ===============================

void loop() {
  // Look straight
  lookCenter();

  long distance = getDistance();

  Serial.print("Front: ");
  Serial.print(distance);
  Serial.println(" cm");


  // =================================
  // NO OBSTACLE
  // =================================

  if (distance > obstacleDistance) {
    moveForward();
  }


  // =================================
  // OBSTACLE DETECTED
  // =================================

  else {
    Serial.println("Obstacle detected!");

    // Stop
    stopCar();
    delay(200);

    // Move backward a little
    moveBackward();
    delay(350);

    stopCar();
    delay(200);


    // =================================
    // SCAN LEFT AND RIGHT
    // =================================

    long leftDistance = lookLeft();

    long rightDistance = lookRight();


    // =================================
    // RETURN SERVO TO CENTER
    // =================================

    lookCenter();


    // =================================
    // CHOOSE CLEARER SIDE
    // =================================

    if (leftDistance > rightDistance) {
      Serial.println("Turning LEFT");

      turnLeft();
      delay(600);
    } else {
      Serial.println("Turning RIGHT");

      turnRight();
      delay(600);
    }


    // Stop after turning
    stopCar();
    delay(200);
  }
}
