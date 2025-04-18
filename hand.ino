#include <Servo.h>

Servo middleFinger;
Servo indexFinger;
Servo ringFinger;

const int ringFingerPin = 4;
const int middleFingerPin = 3;
const int indexFingerPin = 5;

// Latest known angle values
int currentIndexAngle = -1;
int currentMiddleAngle = -1;
int currentRingAngle = -1;

void setup() {
  ringFinger.attach(ringFingerPin);
  middleFinger.attach(middleFingerPin);
  indexFinger.attach(indexFingerPin);
  Serial.begin(9600);

  Serial.println("🖐️ Robotic hand ready for commands.");
}

void loop() {
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    input.trim();

    // Parse format: I:045;M:090
    int iPos = input.indexOf("I:");
    int mPos = input.indexOf("M:");
    int rPos = input.indexOf("R:");

    if (iPos != -1 && mPos != -1) {
      int newIndexAngle = input.substring(iPos + 2, iPos + 5).toInt();
      int newMiddleAngle = input.substring(mPos + 2, mPos + 5).toInt();
      int newRingAngle = input.substring(rPos + 2, rPos + 5).toInt();

      // Move only if index angle changed
      if (newIndexAngle != currentIndexAngle) {
        moveSmooth(indexFinger, currentIndexAngle == -1 ? newIndexAngle : currentIndexAngle, newIndexAngle);
        currentIndexAngle = newIndexAngle;
      }

      // Move only if middle angle changed
      if (newMiddleAngle != currentMiddleAngle) {
        moveSmooth(middleFinger, currentMiddleAngle == -1 ? newMiddleAngle : currentMiddleAngle, newMiddleAngle);
        currentMiddleAngle = newMiddleAngle;
      }

      // Move ring finger
      if (newRingAngle != currentRingAngle) {
        moveSmooth(ringFinger, currentRingAngle == -1 ? newRingAngle : currentRingAngle, newRingAngle);
        currentRingAngle = newRingAngle;
      }
    }
  }
}

// Smooth movement from current to target
void moveSmooth(Servo &servo, int startAngle, int endAngle) {
  if (startAngle == -1) {
    servo.write(endAngle);  // First time, jump to target
    return;
  }

  if (startAngle < endAngle) {
    for (int pos = startAngle; pos <= endAngle; pos++) {
      servo.write(pos);
    }
  } else {
    for (int pos = startAngle; pos >= endAngle; pos--) {
      servo.write(pos);
    }
  }
}
