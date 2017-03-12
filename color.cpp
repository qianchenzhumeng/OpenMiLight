#include "math.h"
#include "color.h"

#define ONE_THIRD 1.0/3.0
#define ONE_SIXTH 1.0/6.0
#define TWO_THIRD 2.0/3.0

float _v(float m1, float m2, float hue)
{
	hue = ((int)(hue*10) % 10) / 10.0;
    if(hue < ONE_SIXTH)
        return m1 + (m2-m1)*hue*6.0;
    if(hue < 0.5)
        return m2;
    if (hue < TWO_THIRD)
        return m1 + (m2-m1)*(TWO_THIRD-hue)*6.0;
    return m1;
}

rgb hls_to_rgb(float h, float l, float s)
{
	rgb _rgb;
	float m1, m2;
	if(s == 0.0)
	{
		_rgb = { l, l, l };
		return _rgb;
	}
    if(l <= 0.5)
        m2 = l * (1.0+s);
    else
        m2 = l+s-(l*s);
    m1 = 2.0*l - m2;
	_rgb.r = _v(m1, m2, h + ONE_THIRD);
	_rgb.g = _v(m1, m2, h);
	_rgb.b = _v(m1, m2, h - ONE_THIRD);
	return _rgb;
}

uint8_t color_from_hls(float hue, float light, float sat)
{
	hls _hls;
	rgb _rgb;
	if(light > 0.95) //too bright, let's just switch to white
        return 256;
    else if(light < 0.05) //too dark, let's shut it off
        return 0;
    else
	{
		_rgb = hls_to_rgb(hue, light, sat); //Change the order, best way I could think how
        _hls = rgb_to_hls(_rgb.b, _rgb.g, _rgb.r);
        uint8_t c = uint8_t(floor(_hls.h * 255));
        return c;
	}
}

uint8_t color_from_rgb(uint8_t red, uint8_t green, uint8_t blue)
{
	/**/
	float s;
	float r = min(red, 255);
	if (r != 0 && floor(r) != 0)
		r = r / 255.0;
	float g = min(green, 255);
	if (g != 0 && floor(g) != 0)
		g = g / 255.0;
	float b = min(blue, 255);
	if (b != 0 && floor(b) != 0)
		b = b / 255.0;
	hls _hls = rgb_to_hls(r, g, b);
	return color_from_hls(_hls.h, _hls.l, _hls.s);
}

hls rgb_to_hls(float r, float g, float b)
{
	float h, l, s;
	hls _hls;
	float maxc = max(r, max(g, b));
	float minc = min(r, min(g, b));
	
	l = (minc + maxc) / 2.0;
	if(minc == maxc)
	{
		_hls = {0.0, l, 0.0};
		return _hls;
	}
	if (l <= 0.5)
		s = (maxc - minc) / (maxc + minc);
	else
		s = (maxc-minc) / (2.0-maxc-minc);
	float rc = (maxc-r) / (maxc-minc);
    float gc = (maxc-g) / (maxc-minc);
    float bc = (maxc-b) / (maxc-minc);
	if(r == maxc)
        h = bc-gc;
    else if(g == maxc)
        h = 2.0+rc-bc;
    else
        h = 4.0+gc-rc;
	h = ((int)(h / 6.0 * 10) % 10) / 10.0;
	_hls = {h, l, s};
    return _hls;
}
