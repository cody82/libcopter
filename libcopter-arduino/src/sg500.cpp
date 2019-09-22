#include <sg500.hpp>

SG500::SG500()
{

}

void SG500::init()
{
    byte buffer[1];

    WiFi.begin(ssid, pwd);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    udp.begin(udpPort);

    buffer[0] = 0x28;
    request(buffer, 1);
    buffer[0] = 0x42;
    request(buffer, 1);
}

void SG500::command(float roll, float pitch, float yaw, float height, bool launch, bool panic, bool land, bool recalibrate)
{
    byte buffer[11];

    height = constrain(height, -1, 1);
    yaw = constrain(yaw, -1, 1);
    pitch = constrain(pitch, -1, 1);
    roll = constrain(roll, -1, 1);

    makeCommand(
        buffer,
        (byte)(height * 0x7E + 0x7E),
        (byte)(yaw * 0x3F + 0x3F),
        (byte)(pitch * 0x3F + 0x40),
        (byte)(roll * 0x3F + 0x3F),
        launch, panic, land, recalibrate
    );

    udp.beginPacket(udpAddress, udpPort);
    udp.write(buffer, sizeof(buffer));
    udp.endPacket();
}

void SG500::makeCommand(byte *command, byte height, byte yaw, byte pitch, byte roll, bool launch, bool panic, bool land, bool recalibrate, bool auto_altitude,
            byte yaw_trim, byte pitch_trim, byte roll_trim, bool compass, byte percent_raw)
{
    command[0] = 0xFF;
    command[1] = 0x08;
    command[2] = (byte)(height & 0xFF);
    command[3] = (byte)(yaw & 0x7F);
    command[4] = (byte)(pitch & 0x7F);
    command[5] = (byte)(roll & 0x7F);

    if (auto_altitude) command[6] |= 0x80;
    if (recalibrate) command[6] |= 0x40;

    command[6] |= (byte)(yaw_trim & 0x3F);
    command[7] = (byte)(pitch_trim & 0x3F);
    command[8] = (byte)(roll_trim & 0x3F);

    if (launch) command[9] |= 0x40;
    if (panic) command[9] |= 0x20;
    if (land) command[9] |= 0x80;
    if (compass) command[9] |= 0x10;

    command[9] |= (byte)(percent_raw & 0x03);

    byte sum = 0;
    for (int i = 1; i < 10; i++) sum += command[i];
    command[10] = (byte)(0xFF - sum);
}


void SG500::request(const byte *data, int length)
{
    byte buffer[64];

    udp.beginPacket(udpAddress, udpPort);
    udp.write(data, length);
    udp.endPacket();

    for(int i=0;i<10;++i)
    {
        delay(10);
        udp.parsePacket();
        if(udp.read(buffer, sizeof(buffer)) > 0){
            Serial.print("Server to client: ");
            Serial.println((char *)buffer);
        }
    }
}
