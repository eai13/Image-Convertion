#include "Lib.h"

int main(){
	BMP_Image morning("../morning.bmp");
	BMP_Image day("../day.bmp");
	BMP_Image night("../night.bmp");
	BMP_Image light_at_night("../light_at_night.bmp");
	morning = morning.ToHSV();
	day = day.ToHSV();
	night = night.ToHSV();
	light_at_night = light_at_night.ToHSV();
	morning = morning.GammaEqualizer(1, 2.25);
	day = day.GammaEqualizer(1, 2.25);
	night = night.GammaEqualizer(1, 2.25);
	light_at_night = light_at_night.GammaEqualizer(1, 2.25);
	morning = morning.ToRGB();
	day = day.ToRGB();
	night = night.ToRGB();
	light_at_night = light_at_night.ToRGB();
	morning.Save("morning.bmp");
	day.Save("day.bmp");
	night.Save("night.bmp");
	light_at_night.Save("light_at_night.bmp");
	// BMP_Image rgb2 = rgb1;
	// BMP_Image rgb3 = rgb1;
	// rgb1 = rgb1.ToHSV();
	// rgb2 = rgb2.ToHSV();
	// rgb3 = rgb3.ToHSV();
	// rgb1 = rgb1.GammaEqualizer(1, 1.5);
	// rgb2 = rgb2.GammaEqualizer(1, 2.25);
	// rgb3 = rgb3.GammaEqualizer(1, 3.00);
	// rgb1 = rgb1.ToRGB();
	// rgb2 = rgb2.ToRGB();
	// rgb3 = rgb3.ToRGB();
	// rgb1.Save("rgb1.bmp");
	// rgb2.Save("rgb2.bmp");
	// rgb3.Save("rgb3.bmp");
	return 0;
}











