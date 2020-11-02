#include "Lib.h"

// BMP Image
BMP_Image::BMP_Image(char const * filename){
	// Reading bmp image file
	FILE * bmp = fopen(filename, "rb");
	// Reading image width and height
	fseek(bmp, 18, SEEK_SET);
	unsigned char buffer[4];
	fread(&buffer, sizeof(unsigned int), 1, bmp);
	this->width = ((((((buffer[3] << 8) | buffer[2]) << 8) | buffer[1]) << 8) | buffer[0]);
	fread(&buffer, sizeof(unsigned int), 1, bmp);
	this->height = ((((((buffer[3] << 8) | buffer[2]) << 8) | buffer[1]) << 8) | buffer[0]);
	// Allocating memory
	this->R = new unsigned char * [this->height];
	this->G = new unsigned char * [this->height];
	this->B = new unsigned char * [this->height];
	// This is for R, G and B arrays to be a single piece
	this->R[0] = new unsigned char [this->height * this->width];
	this->G[0] = new unsigned char [this->height * this->width];
	this->B[0] = new unsigned char [this->height * this->width];
	for (size_t i = 1; i < this->height; i++){
		this->R[i] = this->R[i - 1] + this->width;
		this->G[i] = this->G[i - 1] + this->width;
		this->B[i] = this->B[i - 1] + this->width;
	}
	// Going to the main image matrix
	fseek(bmp, 54, SEEK_SET);
	// Counting the amount of additional bytes in each row
	int additionalBytesAmount = (this->width % 4);
	for (size_t i = (this->height - 1); (i >= 0) && (i < this->height); i--){
		for (size_t j = 0; j < this->width; j++){
			fread(&this->B[i][j], sizeof(unsigned char), 1, bmp);
			fread(&this->G[i][j], sizeof(unsigned char), 1, bmp);
			fread(&this->R[i][j], sizeof(unsigned char), 1, bmp);
		}
		fread(buffer, sizeof(unsigned char), additionalBytesAmount, bmp);
	}
	// Closing the bmp image file
	fclose(bmp);
}

BMP_Image::BMP_Image(BMP_Image const & image) {
	this->height = image.height;
	this->width = image.width;
	this->R = new unsigned char * [this->height];
	this->G = new unsigned char * [this->height];
	this->B = new unsigned char * [this->height];
	this->R[0] = new unsigned char [this->height * this->width];
	this->G[0] = new unsigned char [this->height * this->width];
	this->B[0] = new unsigned char [this->height * this->width];
	for (size_t i = 1; i < this->height; i++){
		this->R[i] = this->R[i - 1] + this->width;
		this->G[i] = this->G[i - 1] + this->width;
		this->B[i] = this->B[i - 1] + this->width;
	}
	for (size_t i = 0; i < this->height; i++){
		for (size_t j = 0; j < this->width; j++){
			this->R[i][j] = image.R[i][j];
			this->G[i][j] = image.G[i][j];
			this->B[i][j] = image.B[i][j];
		}
	}
}

BMP_Image const & BMP_Image::operator = (BMP_Image const & image){
	if (&image == this){
		return *this;
	}
	delete [] this->R[0];
	delete [] this->G[0];
	delete [] this->B[0];
	delete [] this->R;
	delete [] this->G;
	delete [] this->B;
	this->height = image.height;
	this->width = image.width;
	this->R = new unsigned char * [this->height];
	this->G = new unsigned char * [this->height];
	this->B = new unsigned char * [this->height];
	this->R[0] = new unsigned char [this->height * this->width];
	this->G[0] = new unsigned char [this->height * this->width];
	this->B[0] = new unsigned char [this->height * this->width];
	for (size_t i = 1; i < this->height; i++){
		this->R[i] = this->R[i - 1] + this->width;
		this->G[i] = this->G[i - 1] + this->width;
		this->B[i] = this->B[i - 1] + this->width;
	}
	for (size_t i = 0; i < this->height; i++){
		for (size_t j = 0; j < this->width; j++){
			this->R[i][j] = image.R[i][j];
			this->G[i][j] = image.G[i][j];
			this->B[i][j] = image.B[i][j];
		}
	}
	return *this;
}

unsigned char BMP_Image::GetR(int i, int j) const {
	if ((i < this->height) && (j < this->width)){
		return this->R[i][j];
	}
	else{
		std::cout << "No such element" << std::endl;
		return -1;
	}
}

unsigned char BMP_Image::GetG(int i, int j) const {
	if ((i < this->height) && (j < this->width)){
		return this->G[i][j];
	}
	else{
		std::cout << "No such element" << std::endl;
		return -1;
	}
}

unsigned char BMP_Image::GetB(int i, int j) const {
	if ((i < this->height) && (j < this->width)){
		return this->B[i][j];
	}
	else{
		std::cout << "No such element" << std::endl;
		return -1;
	}
}

int BMP_Image::GetHeight() const {
	return this->height;
}

int BMP_Image::GetWidth() const {
	return this->width;
}

BMP_Image BMP_Image::ToGray(void){
	BMP_Image output = *this;
	unsigned char grayValue;
	for (size_t i = 0; i < output.height; i++){
		for (size_t j = 0; j < output.width; j++){
			grayValue = this->R[i][j] * RED_BMP2GRAY + this->G[i][j] * GREEN_BMP2GRAY + this->B[i][j] * BLUE_BMP2GRAY;
			output.R[i][j] = grayValue;
			output.G[i][j] = grayValue;
			output.B[i][j] = grayValue;
		}
	}
	return output;
}

BMP_Image BMP_Image::ToHSV(void){
	BMP_Image output = *this;
	int fMax = 0;
	int fMin = 0;
	unsigned char delta;
	unsigned char channels[3];
	for (size_t i = 0; i < output.height; i++){
		for (size_t j = 0; j < output.width; j++){
			channels[0] = this->R[i][j];
			channels[1] = this->G[i][j];
			channels[2] = this->B[i][j];
			fMax = std::max_element(channels, channels + 3) - channels;
			fMin = std::min_element(channels, channels + 3) - channels;
			delta = channels[fMax] - channels[fMin];
			// Hue
			if (delta == 0) output.R[i][j] = 0;
			else if (fMax == 0) output.R[i][j] = std::fmod(((double)(channels[1] - channels[2]) / (double)(delta)), 6) * ((double)(255) / (double)(6));
			else if (fMax == 1) output.R[i][j] = ((double)(channels[2] - channels[0]) / (double)(delta) + 2) * ((double)(255) / (double)(6));
			else if (fMax == 2) output.R[i][j] = ((double)(channels[0] - channels[1]) / (double)(delta) + 4) * ((double)(255) / (double)(6));
			// Saturation
			if (channels[fMax] == 0) output.G[i][j] = 0;
			else output.G[i][j] = ((double)delta / (double)channels[fMax]) * (double)(255);
			// Value
			output.B[i][j] = channels[fMax];
		}
	}
	return output;
}

BMP_Image BMP_Image::ToRGB(void){
	BMP_Image output = *this;
	double C, X, m;
	double Rt, Gt, Bt;
	double H, S, V;
	for (size_t i = 0; i < output.height; i++){
		for (size_t j = 0; j < output.width; j++){
			H = (double)(this->R[i][j]) / 255 * 360;
			S = (double)(this->G[i][j]) / 255;
			V = (double)(this->B[i][j]) / 255;
			C = S * V;
			X = C * (1 - abs(std::fmod(H / 60, 2) - 1));
			m = V - C;
			if (H > 300){
				Rt = C;
				Gt = 0;
				Bt = X;
			}
			else if (H > 240){
				Rt = X;
				Gt = 0;
				Bt = C;
			}
			else if (H > 180){
				Rt = 0;
				Gt = X;
				Bt = C;
			}
			else if (H > 120){
				Rt = 0;
				Gt = C;
				Bt = X;
			}
			else if (H > 60){
				Rt = X;
				Gt = C;
				Bt = 0;
			}
			else if (H >= 0){
				Rt = C;
				Gt = X;
				Bt = 0;
			}
			output.R[i][j] = (Rt + m) * 255;
			output.G[i][j] = (Gt + m) * 255;
			output.B[i][j] = (Bt + m) * 255;
		}
	}
	return output;
}

BMP_Image BMP_Image::GammaEqualizer(int chNum, double gamma){
	BMP_Image output = *this;
	unsigned char * arr;
	switch(chNum){
		case 0:
			arr = output.R[0];
			break;
		case 1:
			arr = output.G[0];
			break;
		case 2:
			arr = output.B[0];
			break;
		default:
			arr = output.R[0];
			break;
	};
	double Max = arr[std::max_element(arr, arr + output.width * output.height) - arr];
	double Min = arr[std::min_element(arr, arr + output.width * output.height) - arr];
	std::cout << Min << " " << Max << std::endl;
	for (size_t i = 0; i < output.width * output.height; i++){
		double current = arr[i];
		arr[i] = Max * std::pow((current - Min) / (Max - Min), gamma) + Min;
	}
	return output;
}

void BMP_Image::Save(char const * filename){
	FILE * image = fopen(filename, "wb");
	fwrite("BM", sizeof(char), 2, image);
	int additionalBytesAmount = this->width % 4;
	int channelsAmount = 3;
	unsigned int headerSize = 54;
	unsigned int DIBheaderSize = 40;
	unsigned int fileSize = headerSize + (this->width * channelsAmount + additionalBytesAmount) * this->height;
	fwrite(&fileSize, sizeof(unsigned int), 1, image);
	unsigned int reserve = 0;
	fwrite(&reserve, sizeof(unsigned int), 1, image);
	fwrite(&headerSize, sizeof(unsigned int), 1, image);
	fwrite(&DIBheaderSize, sizeof(unsigned int), 1, image);
	unsigned int wid = (unsigned int)this->width;
	unsigned int hei = (unsigned int)this->height;
	fwrite(&wid, sizeof(unsigned int), 1, image);
	fwrite(&hei, sizeof(unsigned int), 1, image);
	unsigned short planes = 1;
	fwrite(&planes, sizeof(unsigned short), 1, image);
	unsigned short bitsPerPixel = 24;
	fwrite(&bitsPerPixel, sizeof(unsigned short), 1, image);
	fwrite(&reserve, sizeof(unsigned int), 1, image);
	unsigned int matrixSize = fileSize - 54;
	fwrite(&matrixSize, sizeof(unsigned int), 1, image);
	unsigned int resolution = 2835;
	fwrite(&resolution, sizeof(unsigned int), 1, image);
	fwrite(&resolution, sizeof(unsigned int), 1, image);
	fwrite(&reserve, sizeof(unsigned int), 1, image);
	fwrite(&reserve, sizeof(unsigned int), 1, image);
	for (int i = this->height - 1; i >= 0; i--){
		for (size_t j = 0; j < this->width; j++){
			fwrite(&this->B[i][j], sizeof(unsigned char), 1, image);
			fwrite(&this->G[i][j], sizeof(unsigned char), 1, image);
			fwrite(&this->R[i][j], sizeof(unsigned char), 1, image);
		}
		fwrite(&reserve, sizeof(unsigned char), additionalBytesAmount, image);
	}
	fclose(image);
}

BMP_Image::~BMP_Image(){
	// Cleaning memory for no memory leaks
	delete [] this->R[0];
	delete [] this->G[0];
	delete [] this->B[0];
	delete [] this->R;
	delete [] this->G;
	delete [] this->B;
}

// YUV Image
YUV_Image::YUV_Image(int width, int height) : width(width), height(height), halfWidth(width % 2 == 0 ? width / 2 : width / 2 + 1), halfHeight(height % 2 == 0 ? height / 2 : height / 2 + 1) { 
	// Allocating memory
	this->Y = new unsigned char * [this->height];
	this->U = new unsigned char * [this->halfHeight];
	this->V = new unsigned char * [this->halfHeight];
	// This is for Y, U and V arrays to be a single piece
	this->Y[0] = new unsigned char [this->height * this->width];
	this->U[0] = new unsigned char [this->halfHeight * this->halfWidth];
	this->V[0] = new unsigned char [this->halfHeight * this->halfWidth];
	for (size_t i = 1; i < this->height; i++){
		this->Y[i] = this->Y[i - 1] + this->width;
	}
	for (size_t i = 1; i < this->halfHeight; i++){
		this->U[i] = this->U[i - 1] + this->halfWidth;
		this->V[i] = this->V[i - 1] + this->halfWidth;
	}
}

YUV_Image::YUV_Image(char const * filename, int width, int height) : width(width), height(height), halfWidth(width % 2 == 0 ? width / 2 : width / 2 + 1), halfHeight(height % 2 == 0 ? height / 2 : height / 2 + 1){
	// Reading yuv image file
	FILE * yuv = fopen(filename, "rb");
	// Allocating memory
	this->Y = new unsigned char * [this->height];
	this->U = new unsigned char * [this->halfHeight];
	this->V = new unsigned char * [this->halfHeight];
	// This is for Y, U and V arrays to be a single piece
	this->Y[0] = new unsigned char [this->height * this->width];
	this->U[0] = new unsigned char [this->halfHeight * this->halfWidth];
	this->V[0] = new unsigned char [this->halfHeight * this->halfWidth];
	for (size_t i = 1; i < this->height; i++){
		this->Y[i] = this->Y[i - 1] + this->width;
	}
	for (size_t i = 1; i < this->halfHeight; i++){
		this->U[i] = this->U[i - 1] + this->halfWidth;
		this->V[i] = this->V[i - 1] + this->halfWidth;
	}
	// Reaing data from yuv image file
	fread(Y[0], sizeof(unsigned char), this->width * this->height, yuv);
	fread(U[0], sizeof(unsigned char), this->halfWidth * this->halfHeight, yuv);
	fread(V[0], sizeof(unsigned char), this->halfWidth * this->halfHeight, yuv);
	// Closing the yuv image file
	fclose(yuv);
}

YUV_Image::~YUV_Image(){
	// Cleaning memory for no memory leaks
	delete [] this->Y[0];
	delete [] this->U[0];
	delete [] this->V[0];
	delete [] this->Y;
	delete [] this->U;
	delete [] this->V;
}

YUV_Image & YUV_Image::operator = (BMP_Image const & bmp){
	// Cleaning current Y, U and V data
	delete [] this->Y[0];
	delete [] this->Y;
	delete [] this->U[0];
	delete [] this->U;
	delete [] this->V[0];
	delete [] this->V;
	// Setting new height and width
	this->height = bmp.GetHeight();
	this->width = bmp.GetWidth();
	this->halfHeight = (this->height % 2 == 0 ? this->height / 2 : this->height / 2 + 1);
	this->halfWidth = (this->width % 2 == 0 ? this->width / 2 : this->width / 2 + 1);
	// Allocating memory
	this->Y = new unsigned char * [this->height];
	this->U = new unsigned char * [this->halfHeight];
	this->V = new unsigned char * [this->halfHeight];
	// This is for Y, U and V arrays to be a single piece
	this->Y[0] = new unsigned char [this->height * this->width];
	this->U[0] = new unsigned char [this->halfHeight * this->halfWidth];
	this->V[0] = new unsigned char [this->halfHeight * this->halfWidth];
	for (size_t i = 1; i < this->height; i++){
		this->Y[i] = this->Y[i - 1] + this->width;
	}
	for (size_t i = 1; i < this->halfHeight; i++){
		this->U[i] = this->U[i - 1] + this->halfWidth;
		this->V[i] = this->V[i - 1] + this->halfWidth;
	}
	// Setting the amount of threads for effective threading
	// We set at least 2 rows for each thread
	int coresAmount = std::thread::hardware_concurrency();
	if (coresAmount == 0){
		coresAmount = 1;
	}
	if (this->height / 2 < coresAmount){
		if (this->height % 2 == 0) coresAmount = this->height / 2;
		else coresAmount = this->height / 2 + 1;
	}
	int rowsInThread = this->height / coresAmount;//((this->height % coresAmount == 0) ? (this->height / coresAmount) : (this->height / coresAmount + 1));
	if (rowsInThread % 2 == 1){
		rowsInThread--;
	}
	// Lambda function for thread
	std::function<void(int)> lambda = [&](int n){
		for (size_t i = n * rowsInThread; i < (n + 1) * rowsInThread; i++){
			for (size_t j = 0; j < this->width; j++){
				if (bmp.GetR(i, j) != -1){
					this->Y[i][j] = (unsigned char)(((double)(bmp.GetR(i, j))) * RED_BMP2YUV + ((double)(bmp.GetG(i, j))) * GREEN_BMP2YUV + ((double)(bmp.GetB(i, j))) * BLUE_BMP2YUV);
					if ((i % 2 == 0) && (j % 2 == 0)){
						this->U[i / 2][j / 2] = (unsigned char)((bmp.GetB(i, j) - this->Y[i][j]) / 2) + 128;
						this->V[i / 2][j / 2] = (unsigned char)((bmp.GetR(i, j) - this->Y[i][j]) / 2) + 128;
					}
				}
				else{
					std::cout << "No such element " << i << " " << j << std::endl;
				}
			}
		}
	};
	// Vector of threads
	std::vector<std::thread> thrd(0);
	// Creating threads
	for (size_t i = 0; i < coresAmount - 1; i++){
		thrd.push_back(std::thread(lambda, i));
	}
	// The last thread
	for (size_t i = rowsInThread * (coresAmount - 1); i < this->height; i++){
		for (size_t j = 0; j < this->width; j++){
			this->Y[i][j] = (unsigned char)(((double)(bmp.GetR(i, j))) * RED_BMP2YUV + ((double)(bmp.GetG(i, j))) * GREEN_BMP2YUV + ((double)(bmp.GetB(i, j))) * BLUE_BMP2YUV);
			if ((i % 2 == 0) && (j % 2 == 0)){
				this->U[i / 2][j / 2] = (unsigned char)((bmp.GetB(i, j) - this->Y[i][j]) / 2) + 128;
				this->V[i / 2][j / 2] = (unsigned char)((bmp.GetR(i, j) - this->Y[i][j]) / 2) + 128;
			}
		}
	}
	// Unite all threads
	for (size_t i = 0; i < coresAmount - 1; i++){
		thrd[i].join();
	}
	return *this;
}

void YUV_Image::SaveFile(char const * filename) const {
	// Creating yuv image file
	FILE * yuv = fopen(filename, "wb");
	// Writing data to the file
	fwrite(this->Y[0], sizeof(unsigned char), this->height * this->width, yuv);
	fwrite(this->U[0], sizeof(unsigned char), this->halfHeight * this->halfWidth, yuv);
	fwrite(this->V[0], sizeof(unsigned char), this->halfHeight * this->halfWidth, yuv);
	// Closing the yuv image file
	fclose(yuv);
}

// YUV Video
YUV_Video::YUV_Video(char const * filename, int width, int height) : filename(filename), width(width), height(height), halfWidth(width % 2 == 0 ? width / 2 : width / 2 + 1), halfHeight(height % 2 == 0 ? height / 2 : height / 2 + 1){
	// Reading yuv video file
	FILE * yuvVideo = fopen(filename, "rb");
	// Finding frames amount
	fseek(yuvVideo, 0, SEEK_END);
	this->framesAmount = ftell(yuvVideo) / (this->width * this->height + this->halfWidth * this->halfHeight * 2);
	fseek(yuvVideo, 0, SEEK_SET);
	// Closing the yuv video image
	fclose(yuvVideo);
}

void addImage(YUV_Video & yuv_video, YUV_Image const & yuv_image, char const * newFileName){
	if ((yuv_image.height < yuv_video.height) && (yuv_image.width < yuv_video.width)){
		FILE * yuvVideo = fopen(yuv_video.filename, "rb");
		FILE * yuvV = fopen(newFileName, "wb");
		int frameSize = yuv_video.width * yuv_video.height + 2 * yuv_video.halfHeight * yuv_video.halfWidth;
		unsigned char * buffer = new unsigned char [frameSize];
		int U_offset = yuv_video.width * yuv_video.height;
		int V_offset = U_offset + yuv_video.halfHeight * yuv_video.halfWidth;
		// Adding yuv image to yuv video
		for (size_t i = 0; i < yuv_video.framesAmount; i++){
			fread(buffer, sizeof(unsigned char), frameSize, yuvVideo);
			for (size_t j = 0; j < yuv_image.height; j++){
				for (size_t k = 0; k < yuv_image.width; k++){
					buffer[j * yuv_video.width + k] = yuv_image.Y[j][k];
				}
			}
			for (size_t j = 0; j < yuv_image.halfHeight; j++){
				for (size_t k = 0; k < yuv_image.halfWidth; k++){
					buffer[U_offset + j * yuv_video.halfWidth + k] = yuv_image.U[j][k];
				}
			}
			for (size_t j = 0; j < yuv_image.halfHeight; j++){
				for (size_t k = 0; k < yuv_image.halfWidth; k++){
					buffer[V_offset + j * yuv_video.halfWidth + k] = yuv_image.V[j][k];
				}
			}
			fwrite(buffer, sizeof(unsigned char), frameSize, yuvV);
		}
		fclose(yuvVideo);
		fclose(yuvV);
		delete [] buffer;
	}
	else{
		std::cout << "Image is larger than video frame" << std::endl;
	}
}