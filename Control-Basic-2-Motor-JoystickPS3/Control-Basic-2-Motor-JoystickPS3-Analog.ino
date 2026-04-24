#include <Ps3Controller.h>

// ===== PIN KONFIGURASI L298N =====
#define MOTOR_LEFT_IN1 25
#define MOTOR_LEFT_IN2 26
#define MOTOR_LEFT_EN 27
#define MOTOR_RIGHT_IN1 32
#define MOTOR_RIGHT_IN2 33
#define MOTOR_RIGHT_EN 34

#define MAX_SPEED 255
#define DEADZONE 10

String lastAction = "";

void onConnect() {
    Serial.println("========================================");
    Serial.println("  PS3 Controller Terhubung!");
    Serial.println("========================================");
}

void setMotor(int in1, int in2, int en, int speed) {
    if (speed > 0) {
        digitalWrite(in1, HIGH);
        digitalWrite(in2, LOW);
        analogWrite(en, speed);
    } else if (speed < 0) {
        digitalWrite(in1, LOW);
        digitalWrite(in2, HIGH);
        analogWrite(en, -speed);
    } else {
        digitalWrite(in1, LOW);
        digitalWrite(in2, LOW);
        analogWrite(en, 0);
    }
}

void stopAllMotors() {
    setMotor(MOTOR_LEFT_IN1, MOTOR_LEFT_IN2, MOTOR_LEFT_EN, 0);
    setMotor(MOTOR_RIGHT_IN1, MOTOR_RIGHT_IN2, MOTOR_RIGHT_EN, 0);
}

void setup() {
    Serial.begin(115200);
    Serial.println("========================================");
    Serial.println("  Robot Car - PS3 Analog Control");
    Serial.println("========================================");

    pinMode(MOTOR_LEFT_IN1, OUTPUT);
    pinMode(MOTOR_LEFT_IN2, OUTPUT);
    pinMode(MOTOR_LEFT_EN, OUTPUT);
    Serial.println("[MOTOR] Pin Motor Kiri  -> IN1:25 | IN2:26 | EN:27");

    pinMode(MOTOR_RIGHT_IN1, OUTPUT);
    pinMode(MOTOR_RIGHT_IN2, OUTPUT);
    pinMode(MOTOR_RIGHT_EN, OUTPUT);
    Serial.println("[MOTOR] Pin Motor Kanan -> IN1:32 | IN2:33 | EN:34");

    stopAllMotors();
    Serial.println("[MOTOR] Semua motor OFF (default)");

    Ps3.attachOnConnect(onConnect);
    Ps3.begin("00:00:00:00:00:02");
    Serial.println("[PS3]  Menunggu koneksi controller...");
    Serial.println("========================================");
}

void loop() {
    if (!Ps3.isConnected()) {
        stopAllMotors();
        return;
    }

    // ===== BACA JOYSTICK KIRI =====
    // LY: -128 = atas (maju), +127 = bawah (mundur)
    // LX: -128 = kiri, +127 = kanan
    int ly = Ps3.data.analog.stick.ly; // -128 ~ 127
    int lx = Ps3.data.analog.stick.lx; // -128 ~ 127

    // ===== DEADZONE =====
    if (abs(ly) < DEADZONE) ly = 0;
    if (abs(lx) < DEADZONE) lx = 0;

    // ===== KONVERSI LY KE NILAI SPEED (-255 ~ 255) =====
    // LY -128 = maju (positif), LY +127 = mundur (negatif)
    int baseSpeed = 0;
    if (ly != 0) {
        // Balik tanda karena -128 = atas = maju
        baseSpeed = map(-ly, -128, 127, -MAX_SPEED, MAX_SPEED);
    }

    // ===== KONVERSI LX KE NILAI STEER (-255 ~ 255) =====
    int steer = 0;
    if (lx != 0) {
        steer = map(lx, -128, 127, -MAX_SPEED, MAX_SPEED);
    }

    // ===== MIXING: HITUNG SPEED KIRI & KANAN =====
    int leftSpeed  = baseSpeed + steer;
    int rightSpeed = baseSpeed - steer;

    // ===== CONSTRAIN AGAR TIDAK MELEBIHI ±255 =====
    leftSpeed  = constrain(leftSpeed,  -MAX_SPEED, MAX_SPEED);
    rightSpeed = constrain(rightSpeed, -MAX_SPEED, MAX_SPEED);

    // ===== SET MOTOR =====
    setMotor(MOTOR_LEFT_IN1,  MOTOR_LEFT_IN2,  MOTOR_LEFT_EN,  leftSpeed);
    setMotor(MOTOR_RIGHT_IN1, MOTOR_RIGHT_IN2, MOTOR_RIGHT_EN, rightSpeed);

    // ===== SERIAL PRINT (hanya saat berubah) =====
    String action = "";
    if (ly == 0 && lx == 0) {
        action = "BERHENTI";
    } else {
        if      (ly < 0 && lx == 0) action = "MAJU";
        else if (ly > 0 && lx == 0) action = "MUNDUR";
        else if (ly == 0 && lx < 0) action = "PUTAR KIRI";
        else if (ly == 0 && lx > 0) action = "PUTAR KANAN";
        else if (ly < 0 && lx < 0)  action = "MAJU BELOK KIRI";
        else if (ly < 0 && lx > 0)  action = "MAJU BELOK KANAN";
        else if (ly > 0 && lx < 0)  action = "MUNDUR BELOK KIRI";
        else if (ly > 0 && lx > 0)  action = "MUNDUR BELOK KANAN";
    }

    // if (action != lastAction) {
    //     Serial.println("----------------------------------------");
    //     Serial.println("[GERAK] >> " + action);
    //     Serial.println("[JOYSTICK] LY: " + String(ly) + " | LX: " + String(lx));
    //     Serial.println("[PWM]     L: " + String(leftSpeed) + " | R: " + String(rightSpeed));
    //     lastAction = action;
    // }
    Serial.println("[GERAK] >> " + action + " | LY: " + String(ly) + " | LX: " + String(lx) + " | L: " + String(leftSpeed) + " | R: " + String(rightSpeed));

    delay(50);
}
