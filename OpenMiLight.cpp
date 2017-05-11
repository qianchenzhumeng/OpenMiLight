#include "OpenMiLight.h"

OpenMiLight::OpenMiLight(MiLightRadio &mlr)
	: _mlr(mlr)
{
	oldColor = 0x00;
	oldBrightness = 100;
	ID1 = 0xB0;
	ID2 = 0x2C;
	ID3 = 0x8C;
}

int OpenMiLight::begin()
{
	int r;
	r = _mlr.begin();
	return r;
}

void OpenMiLight::send_command(uint8_t message[6])
{
	static uint8_t seq_num = 0;
	uint8_t _message[7];
	seq_num++;
	memcpy(_message, message, 6);
	_message[6] = seq_num++;
	_mlr.write(_message, 7);
	Serial.print("Sending: ");
	for (int i = 0; i < 7; i++) {
		printf("%02X ", _message[i]);
	}
	Serial.print("\r\n");
	delay(50);

	// send the instruction multiple times
	for (int resendcounter = 0; resendcounter < 50; resendcounter++)
	{
		_mlr.resend();
		delay(5);
	}
}

void OpenMiLight::set_pair(uint8_t group)
{
	uint8_t cmd1, cmd2, id3;
	cmd1 = 3;
	cmd2 = 19;
	id3 = ID3 + group -1;
	uint8_t message1[6] = { ID1, ID2, id3, oldColor, 0XD1, cmd1};
	send_command(message1);
	delay(50);
	uint8_t message2[6] = { ID1, ID2, id3, oldColor, 0xD1, cmd2};
	send_command(message2);
}

void OpenMiLight::set_status(uint8_t group, bool status)
{
	uint8_t cmd = 0x00, id3;
	if (status==true)
		cmd = 3;
	if (status==false)
		cmd = 4;
	id3 = ID3 + group -1;
	uint8_t message[6] = { ID1, ID2, id3, oldColor, 0XD1, cmd};
	send_command(message);
}

void OpenMiLight::set_color_default(uint8_t group)
{
	set_pair(group);
}
