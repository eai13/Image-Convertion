#include "Lib.h"

int main(){
	// Reading bmp image
	BMP_Image bmpImage("../input_image.bmp");
	// Creating empty yuv image
	YUV_Image yuvImage(10, 10);
	// Converting bmp image to yuv
	yuvImage = bmpImage;
	// Saving yuv image
	yuvImage.SaveFile("output_image.yuv");
	// Reading yuv video
	YUV_Video yuvVideo("../input_video.yuv", 352, 288);
	// Adding yuv image to yuv video
	addImage(yuvVideo, yuvImage, "output_video.yuv");
	return 0;
}











