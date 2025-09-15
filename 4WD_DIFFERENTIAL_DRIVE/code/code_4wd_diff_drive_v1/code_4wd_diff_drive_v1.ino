#include <Ps3Controller.h>
#include <Arduino.h>
#include <ESP32Servo.h>

// Pin definitions for motor
#define IN1 35  // GPIO32 untuk IN1
#define IN2 32  // GPIO33 untuk IN2
#define IN3 25  // GPIO25 untuk IN3
#define IN4 33  // GPIO26 untuk IN4
#define ENA 34  // Ubah ke GPIO yang mendukung PWM
#define ENB 26  // Ubah ke GPIO yang mendukung PWM

// Pin definitions for servo
const int gripperPin = 18;  // Pin untuk gripper
const int lifterPin = 19;   // Pin untuk lifter

// Kecepatan motor (0-255)
int motorSpeed = 200;

// Posisi awal servo
int gripperPos = 90;  // 90 = posisi tengah
int lifterPos = 90;   // 90 = posisi tengah, bisa naik & turun

// Servo objects
Servo gripper;
Servo lifter;

void setup() {
  Serial.begin(115200);

  // Set pin mode for motor
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Set initial state (motor off)
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

  // Attach servo
  gripper.attach(gripperPin);
  lifter.attach(lifterPin);

  // Set initial position for servo
  gripper.write(gripperPos);
  lifter.write(lifterPos);

  // Konfigurasi LEDC PWM untuk motor
  // ledcSetup(0, 5000, 8);
  // ledcSetup(1, 5000, 8);
  // ledcAttachPin(ENA, 0);
  // ledcAttachPin(ENB, 1);

  // Initialize PS3 controller
  Ps3.attach(notify);
  Ps3.attachOnConnect(onConnect);
  Ps3.begin("3c:8a:1f:a0:aa:da");
  Serial.println("PS3 Controller initialized. Waiting for connection...");
}

void loop() {
  if (Ps3.isConnected()) {
    // controlMotors();
    // controlServo();
    // Serial.println("test");
  } else {
    Serial.println("PS3 Controller not connected.");
  }
  delay(10);
}

void controlMotors() {
  if ( Ps3.event.button_down.up) {
    Serial.println("Motor Maju");
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(0, motorSpeed);
    analogWrite(1, motorSpeed);
    // ledcWrite(0, motorSpeed);
    // ledcWrite(1, motorSpeed);
  } else if (Ps3.event.button_down.down) {
    Serial.println("Motor Mundur");
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    analogWrite(0, motorSpeed);
    analogWrite(1, motorSpeed);
    // ledcWrite(0, motorSpeed);
    // ledcWrite(1, motorSpeed);
  } else if ( Ps3.event.button_down.right) {
    Serial.println("Motor kanan");
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    analogWrite(0, motorSpeed);
    analogWrite(1, motorSpeed);
    // ledcWrite(0, motorSpeed);
    // ledcWrite(1, motorSpeed);
  } else if (Ps3.event.button_down.left) {
    Serial.println("Motor kiri");
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(0, motorSpeed);
    analogWrite(1, motorSpeed);
    // ledcWrite(0, motorSpeed);
    // ledcWrite(1, motorSpeed);
  } else {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    analogWrite(0, motorSpeed);
    analogWrite(1, motorSpeed);
    // ledcWrite(0, 0);
    // ledcWrite(1, 0);
  }
}

void controlServo() {
  // Kontrol gripper
  if (Ps3.data.button.cross) {  // Membuka gripper lebih banyak
    if (gripperPos > 0) {
      gripperPos -= 5;  // Kurangi sudut untuk membuka lebih banyak
      Serial.println("Membuka Gripper");
      gripper.write(gripperPos);
    }
  } else if (Ps3.data.button.circle) {  // Menutup gripper lebih banyak
    if (gripperPos < 180) {
      gripperPos += 5;  // Tambah sudut untuk menutup lebih banyak
      Serial.println("Menutup Gripper");
      gripper.write(gripperPos);
    }
  }

  // Kontrol lifter
  if (Ps3.data.button.l2) {  // Mengangkat lifter
    if (lifterPos > 0) {
      lifterPos -= 5;
      Serial.println("Mengangkat Lifter");
      lifter.write(lifterPos);
    }
  } else if (Ps3.data.button.r2) {  // Menurunkan lifter
    if (lifterPos < 180) {
      lifterPos += 5;
      Serial.println("Menurunkan Lifter");
      lifter.write(lifterPos);
    }
  }
}

void notify() {
  // controlMotors();
  if (Ps3.event.button_down.up)
    Serial.println("Started pressing the UP button");
  if (Ps3.event.button_up.up)
    Serial.println("Released the UP button");
}

void onConnect() {
  controlMotors();
  Serial.println("PS3 Controller connected!");
}