#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <SG500.hpp>

uint16_t throttle_raw;
uint16_t pitch_raw;
uint16_t yaw_raw;
uint16_t roll_raw;

float throttle;
float pitch;
float yaw;
float roll;


int takeoff;
int land;
int panic;
int calibrate;

SG500 copter;

float convertAnalog(uint16_t value)
{
  return ((((float)value) / 4096) - 0.5f) * 2.0f * 0.8f;
}

void setup() {
  pinMode(25, INPUT_PULLUP);
  pinMode(26, INPUT_PULLUP);
  pinMode(27, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
  pinMode(23, OUTPUT);
  pinMode(19, OUTPUT);
  pinMode(18, OUTPUT);
  pinMode(5, OUTPUT);
  digitalWrite(23, 1);
  digitalWrite(19, 1);
  digitalWrite(18, 1);
  digitalWrite(5, 1);
  Serial.begin(115200);
  copter.init();
  digitalWrite(23, 0);
}

void loop() {
  throttle_raw = analogRead(36);
  pitch_raw = analogRead(39);
  yaw_raw = analogRead(34);
  roll_raw = analogRead(35);

  throttle = convertAnalog(throttle_raw) + 0.02f;
  pitch = -convertAnalog(pitch_raw) - 0.02f;
  yaw = convertAnalog(yaw_raw) + 0.02f;
  roll = convertAnalog(roll_raw) + 0.03f;

  takeoff = digitalRead(25) == 0;
  land = digitalRead(26) == 0;
  panic = digitalRead(27) == 0;
  calibrate = digitalRead(14) == 0;

  Serial.print("T: ");Serial.print(throttle);Serial.print(", ");
  Serial.print("Y: ");Serial.print(yaw);Serial.print(", ");
  Serial.print("P: ");Serial.print(pitch);Serial.print(", ");
  Serial.print("R: ");Serial.print(roll);Serial.print(", ");
  Serial.print("Takeoff: ");Serial.print(takeoff);Serial.print(", ");
  Serial.print("Land: ");Serial.print(land);Serial.print(", ");
  Serial.print("Panic: ");Serial.print(panic);Serial.print(", ");
  Serial.print("Calibrate: ");Serial.print(calibrate);
  Serial.println();

  copter.command(roll, pitch, yaw, throttle, takeoff, panic, land, calibrate);

  delay(20);
}
