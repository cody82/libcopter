#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>

class SG500
{
    WiFiUDP udp;
    const char *udpAddress = "172.16.10.1";
    const int udpPort = 8080;
    const char *ssid = "JJRC-0df227";
    const char *pwd = NULL;

    public:
        SG500();

        void init();

        void command(float roll, float pitch, float yaw, float height, bool launch = false, bool panic = false, bool land = false, bool recalibrate = false);

    protected:
        void makeCommand(byte *command, byte height, byte yaw, byte pitch, byte roll, bool launch, bool panic, bool land, bool recalibrate, bool auto_altitude = true,
            byte yaw_trim = 0x10, byte pitch_trim = 0x10, byte roll_trim = 0x10, bool compass = false, byte percent_raw = 0);

        boolean request(const byte *data, int length);
};
