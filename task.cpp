#include "Lib.h"

typedef unsigned char uchar;

int main(){
	std::array<uchar, 3> low = {100, 50, 30};
	std::array<uchar, 3> high = {130, 150, 200};
	// Uploading
	BMP_Image morning("../morning.bmp");
	BMP_Image day("../day.bmp");
	BMP_Image night("../night.bmp");
	BMP_Image light_at_night("../light_at_night.bmp");
	BMP_Image morning_orig = morning;
	BMP_Image day_orig = day;
	BMP_Image night_orig = night;
	BMP_Image light_at_night_orig = light_at_night;
	// HSV conversion
	morning = morning.ToHSV();
	day = day.ToHSV();
	night = night.ToHSV();
	light_at_night = light_at_night.ToHSV();
	// Gamma Eqaualizer
	morning = morning.GammaEqualizer(1, 2.25);
	day = day.GammaEqualizer(1, 2.25);
	night = night.GammaEqualizer(1, 2.25);
	light_at_night = light_at_night.GammaEqualizer(1, 2.25);
	// Threshold binarization
	morning = morning.ThresBin(low, high);
	day = day.ThresBin(low, high);
	night = night.ThresBin(low, high);
	light_at_night = light_at_night.ThresBin(low, high);
	// Saving thresholded bit images
	morning.Save("bin_morning.bmp");
	day.Save("bin_day.bmp");
	night.Save("bin_night.bmp");
	light_at_night.Save("bin_light_at_night.bmp");
	// Opening operation
	morning = morning.Opening(5, 21);
	day = day.Opening(5, 21);
	night = night.Opening(5, 21);
	light_at_night = light_at_night.Opening(5, 21);
	// Saving opened images
	morning.Save("op_morning.bmp");
	day.Save("op_day.bmp");
	night.Save("op_night.bmp");
	light_at_night.Save("op_light_at_night.bmp");
	// Detecting the cup
	Dot center[4];
	for (int i = 0; i < 4; i++){
		center[i].x = 0;
		center[i].y = 0;
	}
	int amount[4] = {0, 0, 0, 0};
	for (int i = 0; i < morning.GetHeight(); i++){
		for (int j = 0; j < morning.GetWidth(); j++){
			if (morning.GetR(i, j) == 255) {
				center[0].x += j;
				center[0].y += i;
				amount[0]++;
			}				
			if (day.GetR(i, j) == 255){
				center[1].x += j;
				center[1].y += i;
				amount[1]++;
			}
			if (night.GetR(i, j) == 255){
				center[2].x += j;
				center[2].y += i;
				amount[2]++;
			}
			if (light_at_night.GetR(i, j) == 255){
				center[3].x += j;
				center[3].y += i;
				amount[3]++;
			}
		}
	}
	for (int i = 0; i < 4; i++){
		center[i].x = center[i].x / amount[i];
		center[i].y = center[i].y / amount[i];
	}
	morning_orig = morning_orig.Square(center[0], 200, 200, 11);
	day_orig = day_orig.Square(center[1], 200, 200, 11);
	night_orig = night_orig.Square(center[2], 200, 200, 11);
	light_at_night_orig = light_at_night_orig.Square(center[3], 200, 200, 11);
	morning_orig.Save("det_morning.bmp");
	day_orig.Save("det_day.bmp");
	night_orig.Save("det_night.bmp");
	light_at_night_orig.Save("det_light_at_night.bmp");
	return 0;
}