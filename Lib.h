#ifndef TASK_LIB
#define TASK_LIB

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <functional>

// Coefficients for bmp->yuv transform
const double Kr = 0.299;
const double Kg = 0.587;
const double Kb = 0.114;

struct BMP_Image;
struct YUV_Image;
struct YUV_Video;

// Overloading operator for adding yuv image to yuv video
void addImage(YUV_Video & yuv_video, YUV_Image const & yuv_image, char const * newFileName);

// bmp image class
struct BMP_Image{
	public:
		// Constructor
		BMP_Image(char const * filename);
		// Getters for R, G and B arrays
		unsigned char GetR(int i, int j) const;
		unsigned char GetG(int i, int j) const;
		unsigned char GetB(int i, int j) const;
		// Getters for height and width
		int GetHeight() const;
		int GetWidth() const;
		// Destructor
		~BMP_Image();
	private:
		int width;
		int height;
		unsigned char ** R;
		unsigned char ** G;
		unsigned char ** B;
};

struct YUV_Image{
	public:
		// Constructor for creating empty yuv image [width * height] size
		YUV_Image(int width, int height);
		// Constructor for reading image
		YUV_Image(char const * filename, int width, int height);
		// Overloading operator for conversion bmp to yuv
		YUV_Image & operator = (BMP_Image const & bmp);
		// Adding image to the video
		friend void addImage(YUV_Video & yuv_video, YUV_Image const & yuv_image, char const * newFileName);
		// Method for saving yuv image
		void SaveFile(char const * filename) const;
		// Destructor
		~YUV_Image();
	private:
		int width;
		int halfWidth;
		int height;
		int halfHeight;
		unsigned char ** Y;
		unsigned char ** U;
		unsigned char ** V;
};

struct YUV_Video{
	public:
		// Constructor
		YUV_Video(char const * filename, int width, int height);
		// Adding image to the video
		friend void addImage(YUV_Video & yuv_video, YUV_Image const & yuv_image, char const * newFileName);
	private:
		int width;
		int height;
		int halfWidth;
		int halfHeight;
		int framesAmount;
		char const * filename;
};

#endif