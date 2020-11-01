#include "Lib.h"

int main(){
	std::cout << "1" << std::endl;
	BMP_Image rgb("../test2.bmp");
	std::cout << "2" << std::endl;
	BMP_Image gray = rgb.ToGray();
	std::cout << rgb.GetHeight() << std::endl;
	std::cout << gray.GetHeight() << std::endl;
	std::cout << "3" << std::endl;
	gray.Save("gray2.bmp");
	std::cout << "4" << std::endl;
	return 0;
}











