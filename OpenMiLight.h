#ifndef __OPENMILIGHT_H__
#define __OPENMILIGHT_H__

#include "MiLightRadio.h"

class OpenMiLight {
	public:
		OpenMiLight(MiLightRadio &mlr);
		void set_id(uint8_t id1, uint8_t id2, uint8_t id3);
		int begin();
		void send_command(uint8_t message[6]);
		void set_pair(uint8_t group);
		void set_status(uint8_t group, bool status);
		void set_color(uint8_t red, uint8_t green, uint8_t blue);
		void set_color(uint8_t color);
		void set_color_default(uint8_t group);
		void set_brightness(uint8_t percentage);
		
	private:
		MiLightRadio &_mlr;
		uint8_t ID1, ID2, ID3;
		uint8_t oldColor;
		uint8_t oldBrightness;
};

#endif