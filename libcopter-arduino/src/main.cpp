#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <sg500.hpp>

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

//#define DONT_CONNECT

float convertAnalog(uint16_t value)
{
  return ((((float)value) / 4096) - 0.5f) * 2.0f * 0.8f;
}

float ThrottleOffset;
float PitchOffset;
float YawOffset;
float RollOffset;

void input() {
  throttle_raw = analogRead(36);
  pitch_raw = analogRead(39);
  yaw_raw = analogRead(34);
  roll_raw = analogRead(35);

  throttle = convertAnalog(throttle_raw) + ThrottleOffset;
  pitch = -convertAnalog(pitch_raw) - PitchOffset;
  yaw = convertAnalog(yaw_raw) + YawOffset;
  roll = convertAnalog(roll_raw) + RollOffset;

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
}

void Calibrate() {
  ThrottleOffset = 0.0f;
  PitchOffset = 0.0f;
  YawOffset = 0.0f;
  RollOffset = 0.0f;
  
  for(int i=0;i<10;++i)
  {
    input();
    ThrottleOffset -= throttle;
    PitchOffset -= pitch;
    YawOffset -= yaw;
    RollOffset -= roll;
  }
  ThrottleOffset *= 0.1f;
  PitchOffset *= 0.1f;
  YawOffset *= 0.1f;
  RollOffset *= 0.1f;
  
  Serial.println("Calibration:");
  Serial.print("Toffset: ");Serial.print(ThrottleOffset);Serial.print(", ");
  Serial.print("Yoffset: ");Serial.print(PitchOffset);Serial.print(", ");
  Serial.print("Poffset: ");Serial.print(YawOffset);Serial.print(", ");
  Serial.print("Roffset: ");Serial.print(RollOffset);
}

void led(byte leds) {
  digitalWrite(23, (leds & 1) == 0);
  digitalWrite(19, (leds & 2) == 0);
  digitalWrite(18, (leds & 4) == 0);
  digitalWrite(5, (leds & 8) == 0);
}

byte led_blink = 1;

void connect()
{
  led(0);
#ifndef DONT_CONNECT
  copter.beginInit();
  while(!copter.initReady())
  {
    led(led_blink);
    delay(500);
    led_blink <<= 1;
    if(led_blink > 8)
      led_blink = 1;
    Serial.print(".");
  }
#endif
  led(0xff);
}

void setup() {
  pinMode(25, INPUT_PULLDOWN);
  pinMode(26, INPUT_PULLDOWN);
  pinMode(27, INPUT_PULLDOWN);
  pinMode(14, INPUT_PULLDOWN);
  pinMode(23, OUTPUT);
  pinMode(19, OUTPUT);
  pinMode(18, OUTPUT);
  pinMode(5, OUTPUT);
  Serial.begin(115200);

  Calibrate();

  connect();
}

void loop() {
  input();

#ifndef DONT_CONNECT
  if(!copter.command(roll, pitch, yaw, throttle, takeoff, panic, land, calibrate))
  {
    Serial.println("Send error, restart.");
    //ESP.restart();
    connect();
  }
#endif

  delay(20);
}
