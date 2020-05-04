#include <iostream>
#include <opencv2/core.hpp>
#include "gearinspector.h"

using namespace std;
using namespace cv;

int main()
{
    cout << "Hello World!" << endl;

    Mat src=imread("new_light3.jpg");
    GearInspector g(src);
    g.calcParameters();

    return 0;
}
