#include <Ps3Controller.h>

// ===== PIN KONFIGURASI L298N =====
// Motor Kiri
#define MOTOR_LEFT_IN1 25
#define MOTOR_LEFT_IN2 26
#define MOTOR_LEFT_EN 27

// Motor Kanan
#define MOTOR_RIGHT_IN1 32
#define MOTOR_RIGHT_IN2 33
#define MOTOR_RIGHT_EN 34

// ===== PWM PARAMETER =====
#define PWM_CHANNEL_LEFT 0
#define PWM_CHANNEL_RIGHT 1
#define PWM_FREQUENCY 5000
#define PWM_RESOLUTION 8
#define MAX_SPEED 255

void onConnect()
{
    Serial.println("PS3 Controller Connected!");
}

void setup()
{
    Serial.begin(115200);
    
    // ===== SETUP PIN MOTOR =====
    // Motor Kiri
    pinMode(MOTOR_LEFT_IN1, OUTPUT);
    pinMode(MOTOR_LEFT_IN2, OUTPUT);
    pinMode(MOTOR_LEFT_EN, OUTPUT);
    
    // Motor Kanan
    pinMode(MOTOR_RIGHT_IN1, OUTPUT);
    pinMode(MOTOR_RIGHT_IN2, OUTPUT);
    pinMode(MOTOR_RIGHT_EN, OUTPUT);
    
    // ===== SETUP PWM =====
    ledcSetup(PWM_CHANNEL_LEFT, PWM_FREQUENCY, PWM_RESOLUTION);
    ledcSetup(PWM_CHANNEL_RIGHT, PWM_FREQUENCY, PWM_RESOLUTION);
    ledcAttachPin(MOTOR_LEFT_EN, PWM_CHANNEL_LEFT);
    ledcAttachPin(MOTOR_RIGHT_EN, PWM_CHANNEL_RIGHT);
    
    // ===== SETUP PS3 CONTROLLER =====
    Ps3.attachOnConnect(onConnect);
    Ps3.begin("00:00:00:00:00:02");
    Serial.println("Ready - Waiting for PS3 Controller Connection...");
}

void loop()
{
    if(!Ps3.isConnected()) {
        stopAllMotors();
        return;
    }

    // ===== KONTROL MOTOR MENGGUNAKAN D-PAD =====
    // UP = Maju
    if(Ps3.data.button.up)
    {
        moveForward();
        Serial.println("Maju");
    }
    // DOWN = Mundur
    else if(Ps3.data.button.down)
    {
        moveBackward();
        Serial.println("Mundur");
    }
    // LEFT = Belok Kiri
    else if(Ps3.data.button.left)
    {
        turnLeft();
        Serial.println("Belok Kiri");
    }
    // RIGHT = Belok Kanan
    else if(Ps3.data.button.right)
    {
        turnRight();
        Serial.println("Belok Kanan");
    }
    // Tidak ada tombol ditekan = Berhenti
    else
    {
        stopAllMotors();
    }

    delay(50);
}

// ===== FUNGSI KONTROL MOTOR =====

// Maju - Kedua motor maju
void moveForward()
{
    // Motor Kiri
    digitalWrite(MOTOR_LEFT_IN1, HIGH);
    digitalWrite(MOTOR_LEFT_IN2, LOW);
    ledcWrite(PWM_CHANNEL_LEFT, MAX_SPEED);
    
    // Motor Kanan
    digitalWrite(MOTOR_RIGHT_IN1, HIGH);
    digitalWrite(MOTOR_RIGHT_IN2, LOW);
    ledcWrite(PWM_CHANNEL_RIGHT, MAX_SPEED);
}

// Mundur - Kedua motor mundur
void moveBackward()
{
    // Motor Kiri
    digitalWrite(MOTOR_LEFT_IN1, LOW);
    digitalWrite(MOTOR_LEFT_IN2, HIGH);
    ledcWrite(PWM_CHANNEL_LEFT, MAX_SPEED);
    
    // Motor Kanan
    digitalWrite(MOTOR_RIGHT_IN1, LOW);
    digitalWrite(MOTOR_RIGHT_IN2, HIGH);
    ledcWrite(PWM_CHANNEL_RIGHT, MAX_SPEED);
}

// Belok Kiri - Motor kiri lambat, motor kanan cepat
void turnLeft()
{
    // Motor Kiri (lambat)
    digitalWrite(MOTOR_LEFT_IN1, HIGH);
    digitalWrite(MOTOR_LEFT_IN2, LOW);
    ledcWrite(PWM_CHANNEL_LEFT, MAX_SPEED / 2);
    
    // Motor Kanan (cepat)
    digitalWrite(MOTOR_RIGHT_IN1, HIGH);
    digitalWrite(MOTOR_RIGHT_IN2, LOW);
    ledcWrite(PWM_CHANNEL_RIGHT, MAX_SPEED);
}

// Belok Kanan - Motor kanan lambat, motor kiri cepat
void turnRight()
{
    // Motor Kiri (cepat)
    digitalWrite(MOTOR_LEFT_IN1, HIGH);
    digitalWrite(MOTOR_LEFT_IN2, LOW);
    ledcWrite(PWM_CHANNEL_LEFT, MAX_SPEED);
    
    // Motor Kanan (lambat)
    digitalWrite(MOTOR_RIGHT_IN1, HIGH);
    digitalWrite(MOTOR_RIGHT_IN2, LOW);
    ledcWrite(PWM_CHANNEL_RIGHT, MAX_SPEED / 2);
}

// Berhenti - Semua motor mati
void stopAllMotors()
{
    // Motor Kiri
    digitalWrite(MOTOR_LEFT_IN1, LOW);
    digitalWrite(MOTOR_LEFT_IN2, LOW);
    ledcWrite(PWM_CHANNEL_LEFT, 0);
    
    // Motor Kanan
    digitalWrite(MOTOR_RIGHT_IN1, LOW);
    digitalWrite(MOTOR_RIGHT_IN2, LOW);
    ledcWrite(PWM_CHANNEL_RIGHT, 0);
}
