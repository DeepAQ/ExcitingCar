#include <iostream>
#include <opencv2/opencv.hpp>
#include "GPIOlib.h"

using namespace std;
using namespace GPIO;
using namespace cv;

const int TURN_ANGLE = 30;
const float LR_THRESHOLD = 0.05;
const float TOPTOP_THRESHOLD = 0.7;

int main() {
    init();
    turnTo(0);

    VideoCapture capture(0);
    if (!capture.isOpened()) {
        cerr << "Failed to open camera!";
        return 1;
    }
    int dWidth = capture.get(CV_CAP_PROP_FRAME_WIDTH);
    int dHeight = capture.get(CV_CAP_PROP_FRAME_HEIGHT);
    cout << "Frame Size: " << dWidth << "x" << dHeight << endl;

    int MAX_SPEED, TURN_SPEED, BACK_DELAY, TURN_DELAY, FINAL_DISTANCE;
    float TOP_THRESHOLD;
    cin >> MAX_SPEED >> TURN_SPEED >> BACK_DELAY >> TURN_DELAY >> TOP_THRESHOLD >> FINAL_DISTANCE;
    cout << "Starting engine..." << endl;
    resetCounter();

    int state = 0, cycle = 0, totalLeft = 0, totalRight = 0;
    Mat image, imgLeft, imgRight, imgTop, imgTT;
    Rect roiL(0, 0, dWidth / 4, dHeight / 2);
    Rect roiR(dWidth * 3 / 4, 0, dWidth / 4, dHeight / 2);
    Rect roiT(dWidth / 4, 0, dWidth / 2, dHeight / 4);
    Rect roiTT(0, 0, dWidth * 3 / 4, 5);
    while (true) {
        capture >> image;
        if (image.empty())
            break;
        if (cycle < 90) {
            cycle++;
            continue;
        }
        cvtColor(image, image, CV_BGR2GRAY);
        threshold(image, image, 100, 255, THRESH_BINARY);
        imgLeft = image(roiL);
        imgRight = image(roiR);
        imgTop = image(roiT);
        imgTT = image(roiTT);
        float rateL = 1 - countNonZero(imgLeft) * 8.0 / dWidth / dHeight;
        float rateR = 1 - countNonZero(imgRight) * 8.0 / dWidth / dHeight;
        float rateT = 1 - countNonZero(imgTop) * 8.0 / dWidth / dHeight;
        float rateTT = 1 - countNonZero(imgTT) * 1.0 / (dWidth * 0.75 * 5);
        cout << "L=" << rateL << ", R=" << rateR << ", T=" << rateT << ", TT=" << rateTT << endl;
        if (state == 0) {
            if (rateT >= TOP_THRESHOLD && rateTT >= TOPTOP_THRESHOLD) {
                int readingLeft, readingRight;
                getCounter(&readingLeft, &readingRight);
                totalLeft += readingLeft;
                totalRight += readingRight;
                if (totalLeft > FINAL_DISTANCE && totalRight > FINAL_DISTANCE) {
                    cout << ">>> Reached finish line" << endl;
                    delay(1000);
                    init();
                    return 0;
                }
                cout << ">>> Distance " << totalLeft << ", " << totalRight << endl;
                cout << ">>> State set to 1" << endl;
                state = 1;
                init();
                turnTo(0);
                controlLeft(BACKWARD, TURN_SPEED);
                controlRight(BACKWARD, TURN_SPEED);
                delay(BACK_DELAY);
                init();
                controlLeft(BACKWARD, TURN_SPEED);
                controlRight(FORWARD, TURN_SPEED);
                delay(TURN_DELAY);
            } else {
                controlLeft(FORWARD, MAX_SPEED);
                controlRight(FORWARD, MAX_SPEED);
                if (rateL >= LR_THRESHOLD && rateR < LR_THRESHOLD) {
                    turnTo(TURN_ANGLE);
                } else if (rateR >= LR_THRESHOLD && rateL < LR_THRESHOLD) {
                    turnTo(-TURN_ANGLE);
                } else {
                    turnTo(0);
                }
            }
        } else if (state == 1) {
            if (rateT < TOP_THRESHOLD * 0.5) {
                cout << ">>> State set to 0" << endl;
                init();
                state = 0;
            }
        }
        waitKey(1);
    }
    return 0;
}
