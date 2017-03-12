#ifndef __COLOR__H
#define __COLOR__H

#include <Arduino.h>

typedef struct
{
	float h;
	float l;
	float s;
}hls;

typedef struct
{
	float r;
	float g;
	float b;
}rgb;

uint8_t color_from_rgb(uint8_t red, uint8_t green, uint8_t blue);
hls rgb_to_hls(float r, float g, float b);
rgb hls_to_rgb(float h, float l, float s);
uint8_t color_from_hls(float hue, float light, float sat);

#endif