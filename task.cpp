#include "Lib.h"

int main(){
	BMP_Image rgb("../test2.bmp");
	BMP_Image gray = rgb.ToGray();
	gray.Save("gray2.bmp");
	return 0;
}











