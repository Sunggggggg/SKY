#include <iostream>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/photo.hpp>

using namespace std;
using namespace cv;

#define SIZE 5

int mask_x[SIZE][SIZE] = {
	{2,2,4,2,2},
	{1,1,2,1,1},
	{0,0,0,0,0},
	{-1,-1,-2,-1,-1},
	{-2,-2,-4,-2,-2}
};

int mask_y[SIZE][SIZE] = {
	{2,1,0,-1,-2},
	{2,1,0,-1,-2},
	{4,2,0,-2,-4},
	{2,1,0,-1,-2},
	{2,1,0,-1,-2}
};



int main(int argc, char** argv) {
	int height, width, n_height, n_width, frame_width, frame_hight;
	int x, y, xx, yy;
	float R_val, G_val, B_val, average_gray;
	Mat img;
	VideoCapture cap(0);

	// board
	if (!cap.isOpened()) {
		printf("Can`t open Camera\n");
		return -1;
	}

	frame_width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
	frame_hight = cap.get(cv::CAP_PROP_FRAME_HEIGHT);

	printf("Open Camera\n");
	cap.read(img);
	if (img.empty()) {
		printf("Can`t read Image\n");
		return -1;
	}
	
	height = img.rows;
	width = img.cols;
	Mat imgGray(height, width, CV_8UC1);

	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			R_val = img.at<Vec3b>(y, x)[2];
			G_val = img.at<Vec3b>(y, x)[1];
			B_val = img.at<Vec3b>(y, x)[0];

			average_gray = (int)((R_val + G_val + B_val) / 3);

			imgGray.at<uchar>(y, x) = average_gray;
		}
	}
	
	Mat imgSobel(height, width, CV_8UC1);
	float sumx, sumy, sum;

	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			sumx = 0, sumy = 0, sum = 0;
			
			for (yy = y - SIZE / 2; yy <= y + SIZE / 2; yy++) {
				for (xx = x - SIZE / 2; xx <= x + SIZE / 2; xx++) {
					if (yy >= 0 && yy < height && xx >= 0 && xx < width) {
						sumx += imgGray.at<uchar>(yy, xx) * mask_x[(yy - (y - SIZE / 2))][(xx - (x - SIZE / 2))];
						sumy += imgGray.at<uchar>(yy, xx) * mask_y[(yy - (y - SIZE / 2))][(xx - (x - SIZE / 2))];
					}
				}
			}

			sum = sqrt(sumx * sumx + sumy * sumy);
			
			if (sum > 255 * 0.9) imgSobel.at<uchar>(y, x) = 255;
			else imgSobel.at<uchar>(y, x) = 0;
			
		}
	}

	imwrite("imgSobel.bmp", imgSobel);
	cap.release();
	
	return 0;
}
