#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <SG500.hpp>

uint16_t throttle_raw;
uint16_t pitch_raw;
uint16_t yaw_raw;
uint16_t roll_raw;

uint16_t touch1;
uint16_t touch2;
uint16_t touch3;
uint16_t touch4;
boolean button1;
boolean button2;
boolean button3;
boolean button4;

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

  touch1 = touchRead(27);
  button1 = touch1 < 30;
  touch2 = touchRead(14);
  button2 = touch2 < 30;
  touch3 = touchRead(12);
  button3 = touch3 < 30;
  touch4 = touchRead(13);
  button4 = touch4 < 30;


  throttle = convertAnalog(throttle_raw);
  pitch = convertAnalog(pitch_raw);
  yaw = convertAnalog(yaw_raw);
  roll = convertAnalog(roll_raw);

  Serial.print("T: ");Serial.print(throttle);Serial.print(", ");
  Serial.print("Y: ");Serial.print(yaw);Serial.print(", ");
  Serial.print("P: ");Serial.print(pitch);Serial.print(", ");
  Serial.print("R: ");Serial.print(roll);Serial.print(", ");
  Serial.print("B1: ");Serial.print(button1);Serial.print(", ");
  Serial.print("B2: ");Serial.print(button2);Serial.print(", ");
  Serial.print("B3: ");Serial.print(button3);Serial.print(", ");
  Serial.print("B4: ");Serial.print(button4);
  Serial.println();

  /*takeoff = digitalRead(25);
  land = digitalRead(26);
  panic = digitalRead(27);
  calibrate = digitalRead(14);
*/

  takeoff = 0;
  land = 0;
  panic = 0;
  calibrate = 0;

  copter.command(roll, pitch, yaw, throttle, takeoff, panic, land, calibrate);

  delay(20);
}
