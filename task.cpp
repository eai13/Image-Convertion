#include "Lib.h"

int main(){
	BMP_Image rgb("../test2.bmp");
	BMP_Image hsv = rgb.ToHSV();
	hsv.Save("hsv.bmp");
	return 0;
}











