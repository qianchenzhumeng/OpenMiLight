#include "OpenMiLight.h"
#include "color.h"


OpenMiLight::OpenMiLight(MiLightRadio &mlr)
	: _mlr(mlr)
{
	oldColor = 0x00;
	oldBrightness = 100;
	ID1 = 0;
	ID2 = 0;
	ID3 = 0;
}

void OpenMiLight::set_id(uint8_t id1, uint8_t id2, uint8_t id3)
{
	ID1 = id1;
	ID2 = id2;
	ID3 = id3;
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
	uint8_t cmd1, cmd2;
	cmd1 = 3 + 2 * (group - 1);
	cmd2 = 19 + 2 * (group - 1);
	uint8_t message1[6] = { ID1, ID2, ID3, oldColor, 0XD1, cmd1};
	send_command(message1);
	delay(50);
	uint8_t message2[6] = { ID1, ID2, ID3, oldColor, 0xD1, cmd2};
	send_command(message2);
}

void OpenMiLight::set_status(uint8_t group, bool status)
{
	uint8_t cmd = 0x00;
	if (status==true)
		cmd = 3 + 2 * (group - 1);
	if (status==false)
		cmd = 4 + 2 * (group - 1);
	uint8_t message[6] = { ID1, ID2, ID3, oldColor, 0XD1, cmd};
	send_command(message);
}

void OpenMiLight::set_color(uint8_t red, uint8_t green, uint8_t blue)
{
	uint8_t cmd = 0x0F;
	uint8_t color = color_from_rgb(red, green, blue);
	uint8_t message[6] = { ID1, ID2, ID3, color, 0xA8, cmd};
	send_command(message);
	oldColor = color;
}

void OpenMiLight::set_color(uint8_t color)
{
	uint8_t newcolor = map(color, 0, 99, 0x00, 0xFF);

	uint8_t message[6] = { ID1, ID2, ID3, newcolor, oldBrightness, 0x0F};
	send_command(message);
	oldColor = newcolor;
}

void OpenMiLight::set_color_default(uint8_t group)
{
	set_pair(group);
}

void OpenMiLight::set_brightness(uint8_t percentage)
{
	uint8_t cmd = 0x0E;
	uint8_t brightnessi = map(percentage, 0, 100, 1, 28);
	uint8_t newbright = 0x90;
	if (brightnessi >= 1 && brightnessi <= 19) {
		newbright = 0x88 - (brightnessi * 0x08);
	}

	if (brightnessi > 19 && brightnessi <= 28) {
		newbright = 0xF8 - ((brightnessi - 20) * 0x08);
	}

	uint8_t message[6] = { ID1, ID2, ID3, oldColor++, newbright, cmd};
	send_command(message);
	oldBrightness = newbright;
}
