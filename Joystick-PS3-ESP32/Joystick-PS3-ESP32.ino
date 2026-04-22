#include <Ps3Controller.h>

void onConnect()
{
    Serial.println("Connected");
}

void setup()
{
    Serial.begin(115200);
    Ps3.attachOnConnect(onConnect);
    Ps3.begin("00:00:00:00:00:02");
    Serial.println("Ready");
}

void loop()
{
    if(!Ps3.isConnected()) return;

    // ===== ANALOG =====
    Serial.print("LX: ");       Serial.print(Ps3.data.analog.stick.lx);  Serial.print(" | ");
    Serial.print("LY: ");       Serial.print(Ps3.data.analog.stick.ly);  Serial.print(" | ");
    Serial.print("RX: ");       Serial.print(Ps3.data.analog.stick.rx);  Serial.print(" | ");
    Serial.print("RY: ");       Serial.print(Ps3.data.analog.stick.ry);  Serial.print(" | ");

    // ===== FACE BUTTON =====
    Serial.print("TRIANGLE: "); Serial.print(Ps3.data.button.triangle);  Serial.print(" | ");
    Serial.print("CIRCLE: ");   Serial.print(Ps3.data.button.circle);    Serial.print(" | ");
    Serial.print("CROSS: ");    Serial.print(Ps3.data.button.cross);     Serial.print(" | ");
    Serial.print("SQUARE: ");   Serial.print(Ps3.data.button.square);    Serial.print(" | ");

    // ===== DPAD =====
    Serial.print("UP: ");       Serial.print(Ps3.data.button.up);        Serial.print(" | ");
    Serial.print("RIGHT: ");    Serial.print(Ps3.data.button.right);     Serial.print(" | ");
    Serial.print("DOWN: ");     Serial.print(Ps3.data.button.down);      Serial.print(" | ");
    Serial.print("LEFT: ");     Serial.print(Ps3.data.button.left);      Serial.print(" | ");

    // ===== SHOULDER =====
    Serial.print("L1: ");       Serial.print(Ps3.data.button.l1);        Serial.print(" | ");
    Serial.print("L2: ");       Serial.print(Ps3.data.button.l2);        Serial.print(" | ");
    Serial.print("R1: ");       Serial.print(Ps3.data.button.r1);        Serial.print(" | ");
    Serial.print("R2: ");       Serial.print(Ps3.data.button.r2);        Serial.print(" | ");

    // ===== SYSTEM =====
    Serial.print("SELECT: ");   Serial.print(Ps3.data.button.select);    Serial.print(" | ");
    Serial.print("START: ");    Serial.print(Ps3.data.button.start);     Serial.print(" | ");
    Serial.print("PS: ");       Serial.println(Ps3.data.button.ps);

    delay(10);
}
