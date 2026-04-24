#include <Ps3Controller.h>

// ===== KONFIGURASI PIN MOTOR KIRI =====
#define MOTOR_L_IN1 25
#define MOTOR_L_IN2 26
#define MOTOR_L_EN  27

// ===== KONFIGURASI PIN MOTOR KANAN =====
#define MOTOR_R_IN1 32
#define MOTOR_R_IN2 33
#define MOTOR_R_EN  34

// ===== PARAMETER KONTROL =====
#define MAX_SPEED 255
#define DEADZONE 20
#define STEERING_SENSITIVITY 100  // Sensitivitas belok (0-127)

// ===== VARIABLE GLOBAL =====
int speedL = 0;
int speedR = 0;

void onConnect()
{
    Serial.println("✓ PS3 Controller Connected!");
}

void setup()
{
    Serial.begin(115200);
    delay(1000);
    
    // ===== SETUP PIN =====
    pinMode(MOTOR_L_IN1, OUTPUT);
    pinMode(MOTOR_L_IN2, OUTPUT);
    pinMode(MOTOR_L_EN, OUTPUT);
    pinMode(MOTOR_R_IN1, OUTPUT);
    pinMode(MOTOR_R_IN2, OUTPUT);
    pinMode(MOTOR_R_EN, OUTPUT);
    
    // ===== SETUP PS3 CONTROLLER =====
    Ps3.attachOnConnect(onConnect);
    Ps3.begin("00:00:00:00:00:02");
    
    Serial.println("==========================================");
    Serial.println("  PS3 Controller Motor Control");
    Serial.println("  Kontrol: Left Analog Stick (LX, LY)");
    Serial.println("  LY UP = MAJU | LY DOWN = MUNDUR");
    Serial.println("  LX LEFT = BELOK KIRI | LX RIGHT = BELOK KANAN");
    Serial.println("==========================================");
    Serial.println("Ready!");
}

void loop()
{
    if (!Ps3.isConnected()) {
        stopMotors();
        return;
    }

    // ===== BACA ANALOG STICK =====
    int lx = Ps3.data.analog.stick.lx;  // -128 (kiri) sampai +127 (kanan)
    int ly = Ps3.data.analog.stick.ly;  // -128 (bawah) sampai +127 (atas)
    
    // ===== BALIK LY KARENA AXIS TERBALIK =====
    ly = -ly;  // Sekarang: +127 (atas/maju) sampai -128 (bawah/mundur)
    
    // ===== DEADZONE =====
    if (abs(lx) < DEADZONE) lx = 0;
    if (abs(ly) < DEADZONE) ly = 0;
    
    // ===== HITUNG KECEPATAN MOTOR =====
    calculateMotorSpeed(lx, ly);
    
    // ===== KONTROL MOTOR =====
    controlMotor(MOTOR_L_IN1, MOTOR_L_IN2, MOTOR_L_EN, speedL);
    controlMotor(MOTOR_R_IN1, MOTOR_R_IN2, MOTOR_R_EN, speedR);
    
    // ===== DEBUG OUTPUT =====
    printDebugInfo(lx, ly);
    
    delay(10);
}

/**
 * Hitung kecepatan motor berdasarkan analog stick input
 * dengan support belok sambil maju/mundur
 */
void calculateMotorSpeed(int lx, int ly)
{
    // ===== KECEPATAN DASAR (MAJU/MUNDUR) =====
    speedL = map(ly, -128, 127, -MAX_SPEED, MAX_SPEED);
    speedR = map(ly, -128, 127, -MAX_SPEED, MAX_SPEED);
    
    // ===== STEERING (BELOK) =====
    // Map steering dari -100 (belok kiri penuh) sampai +100 (belok kanan penuh)
    int steering = map(lx, -128, 127, -STEERING_SENSITIVITY, STEERING_SENSITIVITY);
    
    // Constrain steering agar tidak melebihi batas
    steering = constrain(steering, -STEERING_SENSITIVITY, STEERING_SENSITIVITY);
    
    if (steering < 0) {
        // ===== BELOK KIRI =====
        // Motor kiri lebih lambat
        speedL = speedL + steering;
    } else if (steering > 0) {
        // ===== BELOK KANAN =====
        // Motor kanan lebih lambat
        speedR = speedR - steering;
    }
    
    // ===== CONSTRAIN SPEED AGAR TIDAK MELEBIHI MAX_SPEED =====
    speedL = constrain(speedL, -MAX_SPEED, MAX_SPEED);
    speedR = constrain(speedR, -MAX_SPEED, MAX_SPEED);
}

/**
 * Kontrol motor dengan PWM dan arah putaran
 */
void controlMotor(int pinIN1, int pinIN2, int pinEN, int speed)
{
    if (speed > 0) {
        // ===== PUTAR MAJU =====
        digitalWrite(pinIN1, HIGH);
        digitalWrite(pinIN2, LOW);
        analogWrite(pinEN, speed);
    } else if (speed < 0) {
        // ===== PUTAR MUNDUR =====
        digitalWrite(pinIN1, LOW);
        digitalWrite(pinIN2, HIGH);
        analogWrite(pinEN, abs(speed));
    } else {
        // ===== BERHENTI =====
        digitalWrite(pinIN1, LOW);
        digitalWrite(pinIN2, LOW);
        analogWrite(pinEN, 0);
    }
}

/**
 * Hentikan semua motor
 */
void stopMotors()
{
    digitalWrite(MOTOR_L_IN1, LOW);
    digitalWrite(MOTOR_L_IN2, LOW);
    analogWrite(MOTOR_L_EN, 0);
    
    digitalWrite(MOTOR_R_IN1, LOW);
    digitalWrite(MOTOR_R_IN2, LOW);
    analogWrite(MOTOR_R_EN, 0);
}

/**
 * Print informasi debug ke Serial Monitor
 */
void printDebugInfo(int lx, int ly)
{
    Serial.print("LX: ");
    Serial.print(lx, 4);
    Serial.print(" | LY: ");
    Serial.print(ly, 4);
    Serial.print(" | SpeedL: ");
    Serial.print(speedL, 4);
    Serial.print(" | SpeedR: ");
    Serial.print(speedR);
    
    // ===== TAMPILKAN ARAH =====
    Serial.print(" | Arah: ");
    if (ly > 0 && lx == 0) {
        Serial.print("⬆ MAJU");
    } else if (ly < 0 && lx == 0) {
        Serial.print("⬇ MUNDUR");
    } else if (ly > 0 && lx < 0) {
        Serial.print("↖ MAJU KIRI");
    } else if (ly > 0 && lx > 0) {
        Serial.print("↗ MAJU KANAN");
    } else if (ly < 0 && lx < 0) {
        Serial.print("↙ MUNDUR KIRI");
    } else if (ly < 0 && lx > 0) {
        Serial.print("↘ MUNDUR KANAN");
    } else if (lx < 0 && ly == 0) {
        Serial.print("⤴ PUTAR KIRI");
    } else if (lx > 0 && ly == 0) {
        Serial.print("⤵ PUTAR KANAN");
    } else {
        Serial.print("⏹ STOP");
    }
    
    Serial.println();
}


