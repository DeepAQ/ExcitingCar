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
const int CANNY_LOWER_BOUND=50;
const int CANNY_UPPER_BOUND=250;
const int HOUGH_THRESHOLD=150;

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

	Mat contours;
	Canny(image, contours, CANNY_LOWER_BOUND, CANNY_UPPER_BOUND);

	imshow(CAM_PATH, image);
	imshow("Canny",contours);

	vector<Vec2f> lines;
	HoughLines(contours, lines, 1, PI/180, HOUGH_THRESHOLD);
	Mat result(image.size(), CV_8U, Scalar(255));
	image.copyTo(result);
	cout << "Found " << lines.size() << " lines" << endl;

	float maxRad = -2 * PI, minRad = 2 * PI;
	for(vector<Vec2f>::const_iterator it=lines.begin(); it!=lines.end(); ++it) {
		float rho = (*it)[0];
		float theta = (*it)[1];
		if ((theta>0.09&&theta<1.48)||(theta>1.62&&theta<3.05))
			cout << "Line: (" << rho << ", " << theta << ")" << endl;
	}

	waitKey(1);
    }
    return 0;
}

