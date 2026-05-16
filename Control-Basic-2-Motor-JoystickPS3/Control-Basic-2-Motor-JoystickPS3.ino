#include <Ps3Controller.h>

// ===== PIN KONFIGURASI L298N =====
// Motor Kiri
#define MOTOR_LEFT_IN1 26
#define MOTOR_LEFT_IN2 27
#define MOTOR_LEFT_EN 28
// Motor Kanan
#define MOTOR_RIGHT_IN1 12
#define MOTOR_RIGHT_IN2 13
#define MOTOR_RIGHT_EN 34

// ===== PWM PARAMETER =====
#define MAX_SPEED 100

String lastAction = "";

void onConnect() {
    Serial.println("========================================");
    Serial.println("  PS3 Controller Terhubung!");
    Serial.println("  MAC: " + String(Ps3.getAddress()));
    Serial.println("  Siap digunakan.");
    Serial.println("========================================");
}

void onDisconnect() {
    Serial.println("========================================");
    Serial.println("  PS3 Controller Terputus!");
    Serial.println("========================================");
}

void setup() {
    Serial.begin(115200);
    Serial.println("========================================");
    Serial.println("  Robot Car - PS3 Controller");
    Serial.println("  Inisialisasi sistem...");
    Serial.println("========================================");

    // ===== SETUP PIN MOTOR =====
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

    // ===== SETUP PS3 CONTROLLER =====
    Ps3.attachOnConnect(onConnect);
    Ps3.begin("00:00:00:00:00:02");
    Serial.println("[PS3]  Bluetooth aktif. MAC: 00:00:00:00:00:02");
    Serial.println("[PS3]  Menunggu koneksi controller...");
    Serial.println("========================================");
}

void loop() {
    if (!Ps3.isConnected()) {
        stopAllMotors();
        return;
    }

    // ===== KONTROL MOTOR MENGGUNAKAN D-PAD =====
    if (Ps3.data.button.up) {
            moveForward();
            Serial.println("[GERAK] >> MAJU  | L:" + String(MAX_SPEED) + " R:" + String(MAX_SPEED));
    }
    else if (Ps3.data.button.down) {
            moveBackward();
            Serial.println("[GERAK] >> MUNDUR | L:" + String(MAX_SPEED) + " R:" + String(MAX_SPEED));
    }
    else if (Ps3.data.button.left) {
            turnLeft();
            Serial.println("[GERAK] >> BELOK KIRI  | L:" + String(MAX_SPEED/2) + " R:" + String(MAX_SPEED));
    }
    else if (Ps3.data.button.right) {
            turnRight();
            Serial.println("[GERAK] >> BELOK KANAN | L:" + String(MAX_SPEED) + " R:" + String(MAX_SPEED/2));

    }
    else {
            stopAllMotors();
            Serial.println("[GERAK] >> BERHENTI | L:0 R:0");
    }

}

// ===== FUNGSI KONTROL MOTOR =====
void moveForward() {
    digitalWrite(MOTOR_LEFT_IN1, LOW);
    digitalWrite(MOTOR_LEFT_IN2, HIGH);
//    analogWrite(MOTOR_LEFT_EN, MAX_SPEED);

    digitalWrite(MOTOR_RIGHT_IN1, LOW);
    digitalWrite(MOTOR_RIGHT_IN2, HIGH);
//    analogWrite(MOTOR_RIGHT_EN, MAX_SPEED);
}

void moveBackward() {
    digitalWrite(MOTOR_LEFT_IN1, HIGH);
    digitalWrite(MOTOR_LEFT_IN2, LOW);
//    analogWrite(MOTOR_LEFT_EN, MAX_SPEED / 2);

    digitalWrite(MOTOR_RIGHT_IN1, HIGH);
    digitalWrite(MOTOR_RIGHT_IN2, LOW);
//    analogWrite(MOTOR_RIGHT_EN, MAX_SPEED);
}

void turnLeft() {
    digitalWrite(MOTOR_LEFT_IN1, LOW);
    digitalWrite(MOTOR_LEFT_IN2, HIGH);
//    analogWrite(MOTOR_LEFT_EN, MAX_SPEED);

    digitalWrite(MOTOR_RIGHT_IN1, LOW);
    digitalWrite(MOTOR_RIGHT_IN2, HIGH);
//    analogWrite(MOTOR_RIGHT_EN, MAX_SPEED);
}

void turnRight() {
    digitalWrite(MOTOR_LEFT_IN1, HIGH);
    digitalWrite(MOTOR_LEFT_IN2, LOW);
//    analogWrite(MOTOR_LEFT_EN, MAX_SPEED);

    digitalWrite(MOTOR_RIGHT_IN1, HIGH);
    digitalWrite(MOTOR_RIGHT_IN2, LOW);
//    analogWrite(MOTOR_RIGHT_EN, MAX_SPEED);
}

void stopAllMotors() {
    digitalWrite(MOTOR_LEFT_IN1, LOW);
    digitalWrite(MOTOR_LEFT_IN2, LOW);
//    analogWrite(MOTOR_LEFT_EN, 0);

    digitalWrite(MOTOR_RIGHT_IN1, LOW);
    digitalWrite(MOTOR_RIGHT_IN2, LOW);
//    analogWrite(MOTOR_RIGHT_EN, 0);
}
