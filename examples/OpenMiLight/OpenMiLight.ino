#include <OpenMiLight.h>
#include <SPI.h>
#include <RF24.h>
#include <PL1167_nRF24.h>
#include <MiLightRadio.h>

#define CE_PIN D2
#define CSN_PIN D8

RF24 radio(CE_PIN, CSN_PIN);
PL1167_nRF24 prf(radio);
MiLightRadio mlr(prf);
OpenMiLight MiLight(mlr);

void setup()
{
	Serial.begin(115200);
	MiLight.set_id(0xB0, 0x2C, 0x8C);
	MiLight.begin();
}

void loop()
{
	if (mlr.available()) {
		uint8_t packet[7];
		size_t packet_length = sizeof(packet);
		mlr.read(packet, packet_length);
		printf("\nReceived packet: ");
		for (int i = 0; i < packet_length; i++) {
			printf("%02X", packet[i]);
		}
	}
	
	if (Serial.available())
	{
		uint8_t c = Serial.read();
		switch (c)
		{
		case '1':
			MiLight.set_pair(1);
			break;
		case '2':
			MiLight.set_color_default(1);
			break;
		case '3':
			MiLight.set_status(1, false);
			break;
		case '4':
			MiLight.set_status(1, true);
			break;
		case '5':
			MiLight.set_color(0);
			break;
		case '6':
			MiLight.set_color(50);
			break;
		case '7':
			MiLight.set_brightness(1);
			break;
		case '8':
			MiLight.set_brightness(50);
			break;
		case '9':
			MiLight.set_brightness(100);
			break;
		}
	}

}