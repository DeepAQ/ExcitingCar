#include <iostream>

#include <opencv2/opencv.hpp>

#define PI 3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170680

using namespace cv;
using namespace std;

int main()
{
    VideoCapture capture(0);
    if (!capture.isOpened())
    {
	cerr << "Failed to open camera!";
	return 1;
    }
    int dWidth = capture.get(CV_CAP_PROP_FRAME_WIDTH);
    int dHeight = capture.get(CV_CAP_PROP_FRAME_HEIGHT);
    cout << "Frame Size: " << dWidth << "x" << dHeight << endl;

    Mat image, imgLeft, imgRight;
    Rect roiL(0, 0, dWidth / 2, dHeight);
    Rect roiR(dWidth / 2, 0, dWidth / 2, dHeight);
    while (true)
    {
        capture >> image;
	if (image.empty()) break;
	imshow("Camera", image);

	cvtColor(image, image, CV_BGR2GRAY);
	threshold(image, image, 64, 255, THRESH_BINARY);
	imshow("Processed", image);

	imgLeft = image(roiL);
	imgRight = image(roiR);
	float rateL = 1 - countNonZero(imgLeft) * 2.0 / dWidth / dHeight;
	float rateR = 1 - countNonZero(imgRight) * 2.0 / dWidth / dHeight;
	cout << "L=" << rateL << ", R=" << rateR << endl;
	waitKey(1);
    }
    return 0;
}

