#include <sg500.hpp>

SG500::SG500()
{

}


void SG500::beginInit()
{
    WiFi.begin(ssid, pwd);
}

bool SG500::initReady()
{
    if (WiFi.status() != WL_CONNECTED) {
        return false;
    }
    else
    {
        init();
        return true;
    }
}

void SG500::init()
{
    byte buffer[1];

    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    udp.begin(udpPort);

    // reset
    Serial.println("Reset");
    buffer[0] = 0x0F;
    udp.beginPacket(udpAddress, udpPort);
    udp.write(buffer, 1);
    udp.endPacket();

    delay(1000);

    // type
    Serial.println("Type");
    buffer[0] = 0x28;
    while(!request(buffer, 1));
    // version
    Serial.println("Version");
    buffer[0] = 0x42;
    while(!request(buffer, 1));
}

bool SG500::command(float roll, float pitch, float yaw, float height, bool launch, bool panic, bool land, bool recalibrate)
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

    if(!udp.beginPacket(udpAddress, udpPort))
    {
        return false;
    }
    udp.write(buffer, sizeof(buffer));

    if(!udp.endPacket())
    {
        return false;
    }

    return true;
}

void SG500::makeCommand(byte *command, byte height, byte yaw, byte pitch, byte roll, bool launch, bool panic, bool land, bool recalibrate, bool auto_altitude,
            byte yaw_trim, byte pitch_trim, byte roll_trim, bool compass, byte percent_raw)
{
	command[0] = 0xFF;
	command[1] = 0x08;
	command[2] = height & 0xFF;
	command[3] = yaw & 0x7F;
	command[4] = pitch & 0x7F;
	command[5] = roll & 0x7F;

	command[6] = 0;
	if (auto_altitude) command[6] |= 0x80;
	if (recalibrate)   command[6] |= 0x40;

	command[6] |= yaw_trim & 0x3F;
	command[7] = pitch_trim & 0x3F;
	command[8] = roll_trim & 0x3F;

    command[9] = 0;
	if (launch)  command[9] |= 0x40;
	if (panic)   command[9] |= 0x20;
	if (land)    command[9] |= 0x80;
	if (compass) command[9] |= 0x10;

	command[9] |= percent_raw & 0x03;

	// checksum
	uint8_t sum = 0;
	for (int i=1; i<10; i++) sum += uint8_t(command[i]);
	command[10] = 0xFF - sum;
}


boolean SG500::request(const byte *data, int length)
{
    byte buffer[64];
    boolean ok = false;

    while(!ok)
    {
        udp.beginPacket(udpAddress, udpPort);
        udp.write(data, length);
        udp.endPacket();

        delay(100);

        udp.parsePacket();
        if(udp.read(buffer, sizeof(buffer)) > 0){
            Serial.print("Received: ");
            Serial.println((char *)buffer);
            ok = true;
        }
    }

    return ok;
}
