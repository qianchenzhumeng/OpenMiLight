#ifndef __OPENMILIGHT_H__
#define __OPENMILIGHT_H__

#include "MiLightRadio.h"

class OpenMiLight {
	public:
		OpenMiLight(MiLightRadio &mlr);
		int begin();
		void send_command(uint8_t message[6]);
		void set_pair(uint8_t group);
		void set_status(uint8_t group, bool status);
		void set_color_default(uint8_t group);
		
	private:
		MiLightRadio &_mlr;
		uint8_t ID1, ID2, ID3;
		uint8_t oldColor;
		uint8_t oldBrightness;
};

#endif
