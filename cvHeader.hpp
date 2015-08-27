#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>
#include <string>

using namespace std;
using namespace cv;

/* Function prototypes */
void blur_image(string);
void canny_video();
void display_image(string);
void object_tracking(string, int);
void print_options();
void video_capture();
void video_from_file(string);
