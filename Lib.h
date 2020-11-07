#ifndef TASK_LIB
#define TASK_LIB

#define RED 0
#define GRN 1
#define BLU 2

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <thread>
#include <functional>
#include <algorithm>
#include <cmath>

// Coefficients for bmp->yuv transform
const double RED_BMP2YUV = 0.299;
const double GREEN_BMP2YUV = 0.587;
const double BLUE_BMP2YUV = 0.114;

// Coefficients for bmp->gray transform
const double RED_BMP2GRAY = 0.299;
const double GREEN_BMP2GRAY = 0.587;
const double BLUE_BMP2GRAY = 0.114;

struct BMP_Image;
struct YUV_Image;
struct YUV_Video;

// Overloading operator for adding yuv image to yuv video
void addImage(YUV_Video & yuv_video, YUV_Image const & yuv_image, char const * newFileName);

struct Dot{
	Dot(int x, int y) : x(x), y(y) {}
	Dot() {}
	int x;
	int y;
};

// bmp image class
struct BMP_Image{
	public:
		// Constructors
		BMP_Image(char const * filename);
		BMP_Image(BMP_Image const & image);
		// Assignment
		BMP_Image const & operator = (BMP_Image const & image);
		//Conversions
		BMP_Image ToGray(void);
		BMP_Image ToHSV(void);
		BMP_Image ToRGB(void);
		// Operations on Images
		BMP_Image GammaEqualizer(double gamma);
		BMP_Image ThresBin(std::array<unsigned char, 3> low, std::array<unsigned char, 3> high);
		BMP_Image Erosion(int structure);
		BMP_Image Dilation(int structure);
		BMP_Image Opening(int erosStruct, int dilStruct);
		BMP_Image Closing(int erosStruct, int dilStruct);
		// Geometric Figures
		BMP_Image Square(Dot center, int halfWidth, int halfHeight, int thickness);
		BMP_Image Point(Dot center, int thickness);
		BMP_Image Line(Dot dot1, Dot dot2, int thickness);
		// Getters for R, G and B arrays
		unsigned char GetR(int i, int j) const;
		unsigned char GetG(int i, int j) const;
		unsigned char GetB(int i, int j) const;
		// Getters for height and width
		int GetHeight() const;
		int GetWidth() const;
		// Saving function
		void Save(char const * filename);
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