#include <iostream>
#include "GPIOlib.h"

using namespace std;
using namespace GPIO;

int main(int argc, char *argv[])
{
    cout << "Init" << endl;
    init();
    cout << "Turn to 0" << endl;
    turnTo(0);
    cout << "Reset counter" << endl;
    resetCounter();
    delay(1000);
    cout << "Forward" << endl;
    controlLeft(FORWARD, 100);
    controlRight(FORWARD, 100);
    int count_l, count_r;
    int total_l = 0, total_r = 0;
    for (int i = 0; i < 20; i++) {
        getCounter(&count_l, &count_r);
        total_l += count_l;
        total_r += count_r;
        cout << "Count: Left " << count_l << ", Right " << count_r << endl;
        double speed_l = count_l / 100.0;
        double speed_r = count_r / 100.0;
        int a_l, a_r;
        if (speed_l < speed_r) {
            a_l = 100;
            a_r = 100 * speed_l / speed_r + 1;
        } else {
            a_r = 100;
            a_l = 100 * speed_r / speed_l + 1;
        }
        cout << "Speed: Left " << a_l << ", Right " << a_r << endl;
        controlLeft(FORWARD, a_l);
        controlRight(FORWARD, a_r);
        delay(50);
    }
    cout << "Stop" << endl;
    init();
    getCounter(&count_l, &count_r);
    total_l += count_l;
    total_r += count_r;
    cout << "Total Count: Left " << total_l << ", Right " << total_r << endl;
    return 0;
}
