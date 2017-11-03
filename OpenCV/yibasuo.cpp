#include <cstdlib>
#include <iostream>
#include <vector>

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>

#define PI 3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170680

using namespace cv;
using namespace std;

const string CAM_PATH="/dev/video0";

int main()
{
    VideoCapture capture(CAM_PATH);
    if (!capture.isOpened())
    {
	capture.open(atoi(CAM_PATH.c_str()));
    }
    double dWidth = capture.get(CV_CAP_PROP_FRAME_WIDTH);
    double dHeight = capture.get(CV_CAP_PROP_FRAME_HEIGHT);
    cout << "Frame Size: " << dWidth << "x" << dHeight << endl;

    Mat image;
    while (true)
    {
        capture >> image;
	if (image.empty()) break;
	imshow(CAM_PATH, image);
	waitKey(1);
    }
    return 0;
}

