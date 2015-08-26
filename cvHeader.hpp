#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>

using namespace std;
using namespace cv;

/* Function prototypes */
void blur_image(char *);
void canny_video();
void display_image(char *);
void object_tracking(char *, int);
void video_capture();
void video_from_file(char *);