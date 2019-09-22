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
  return ((((float)value) / 4096) - 0.5f) * 2.0f;
}

void setup() {
  Serial.begin(115200);
  copter.init();
}

void loop() {
  throttle_raw = analogRead(36);
  pitch_raw = analogRead(39);
  yaw_raw = analogRead(34);
  roll_raw = analogRead(35);

  throttle = convertAnalog(throttle_raw);
  pitch = convertAnalog(pitch_raw);
  yaw = convertAnalog(yaw_raw);
  roll = convertAnalog(roll_raw);

  takeoff = digitalRead(25);
  land = digitalRead(26);
  panic = digitalRead(27);
  calibrate = digitalRead(14);

  copter.command(roll, pitch, yaw, throttle, takeoff, panic, land, calibrate);

  delay(20);
}
