#include "cvHeader.hpp"

/* ------ Blur an image ----- */
void blur_image(char *imgName)
{
	printf("Blurring the image '%s'.\n", imgName);

	Mat src, dst;
	char window_name1[] = "Unprocessed Image";
	char window_name2[] = "Processed Image";

	src = imread(imgName, 1);

	namedWindow(window_name1, WINDOW_AUTOSIZE);
	imshow("Unprocessed Image", src);

	dst = src.clone();
	GaussianBlur(src,dst,Size(15,15),0,0);

	namedWindow(window_name2, WINDOW_AUTOSIZE);
	imshow("Processed Image", dst);

	waitKey();
}

/* ----- Canny edge detection on camera ----- */
void canny_video()
{
	printf("Canny Edge Detector from Camera.\n");

	VideoCapture cap(0);
	if (!cap.isOpened())
		return;

	Mat edges;
	namedWindow("edges", 1);
	while (true) {
		Mat frame;
		cap >> frame;
		cvtColor(frame, edges, CV_BGR2GRAY);
		GaussianBlur(edges, edges, Size(7,7), 1.5, 1.5);
		Canny(edges, edges, 0, 30, 3);
		imshow("edges", edges);
		if (waitKey(30) >= 0) break;
	}
}

/* ----- Display an image ----- */
void display_image(char *imgName)
{
	printf("Displaying image '%s'.\n", imgName);

	IplImage* img = cvLoadImage(imgName);
	cvNamedWindow("Image", CV_WINDOW_AUTOSIZE);
	cvShowImage("Image", img);
	cvWaitKey(0);
	cvReleaseImage(&img);
	cvDestroyWindow("Image");
}

/* ----- Convert an integer to a string ----- */
string intToString(int number)
{
	std::stringstream ss;
	ss << number;
	return ss.str();
}

/* ----- Search for movement in a video ----- */
void searchForMovement(Mat thresholdImage, Mat &cameraFeed)
{
	bool ObjectDetected = false;
	Mat temp;
	int theObject = {0,0};
	Rect objectBoundingRectangle = Rect(0,0,0,0);
	thresholdImage.copyTo(temp);
	vector< vector< Point> > contours;
	vector< Vec4i> heirarchy;
	findContours(temp, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	if (contours.size() > 0) ObjectDetected = true;
	else ObjectDetected = false;

	if (ObjectDetected) {
		vector< vector< Point> > largestContourVec;
		largestContourVec.push_back(contours.at(contours.size()-1));
		objectBoundingRectangle = boundingRect(largestContourVec.at(0));
		int xpos = objectBoundingRectangle.x + objectBoundingRectangle.width/2;
		int ypos = objectBoundingRectangle.y + objectBoundingRectangle.height/2;

		theObject[0] = xpos , theObject[1] = ypos;
	}

	int x = theObject[0];
	int y = theObject[1];

	circle(cameraFeed, Point(x,y), 20, Scalar(0,255,0), 2);
	line(cameraFeed, Point(x,y), Point(x,y-25), Scalar(0,255,0), 2);
	line(cameraFeed, Point(x,y), Point(x,y+25), Scalar(0,255,0), 2);
	line(cameraFeed, Point(x,y), Point(x-25,y), Scalar(0,255,0), 2);
	line(cameraFeed, Point(x,y), Point(x+25,y), Scalar(0,255,0), 2);

	putText(cameraFeed,"Tracking object at (" +
		intToString(x)+","+intToString(y)+")", Point(x,y), 1, 1, Scalar(255,0,0), 2);
}

/* ----- Track an object in a video ----- */
void object_tracking(char *vidName, int sens)
{
	printf("Detecting a moving object in '%s' with %d sensitivity value", vidName, sens);

	bool ObjectDetected = false, debugMode = false, trackingEnabled = true, pause = false;
	Mat frame1, frame2;
	Mat grayImage1, grayImage2;
	Mat differenceImage, thresholdImage;
	VideoCapture capture;
	const static int BLUR_SIZE = 10;

	while (true) {
		capture.open(vidName);
		if (!capture.isOpened()) {
			cout << "Error acquiring video feed\n";
			getchar(); return;
		}
		while (capture.get(CV_CAP_PROP_POS_FRAME) < capture.get(CV_CAP_PROP_FRAME_COUNT)-1) {
			capture.read(frame1); capture.read(frame2);
			cvtColor(frame1, grayImage1, COLOR_BGR2GRAY);
			cvtColor(frame2, grayImage2, COLOR_BGR2GRAY);
			absdiff(grayImage1, grayImage2, differenceImage);
			thresholdImage(differenceImage, thresholdImage, sens, 255, THRESH_BINARY);
			
			if (debugMode == true) {
				imshow("Difference Image", differenceImage);
				imshow("Threshold Image", thresholdImage);
			} else {
				destroyWindow("Difference Image");
				destroyWindow("Threshold Image");
			}

			blur(thresholdImage, thresholdImage, Size(BLUR_SIZE,BLUR_SIZE));
			threshold(thresholdImage, thresholdImage, sens, 255, THRESH_BINARY);

			if (debugMode == true)
				imshow("Final Threshold Image", thresholdImage);
			else
				destroyWindow("Final Threshold Image");

			if (trackingEnabled)
				searchForMovement(thresholdImage, frame1);

			imshow("Frame1", frame1);

			switch (waitKey(10))
			{
				case 27:
					return;
				case 116:
					trackingEnabled = !trackingEnabled;
					if (trackingEnabled == false) cout << "Tracking disabled." << endl;
					else cout << "Tracking enabled." << endl;
					break;
				case 100:
					debugMode = !debugMode;
					if (debugMode == false) cout << "Debug mode disabled." << endl;
					else cout << "Debug mode enabled." << endl;
					break;
				case 112:
					pause = !pause;
					if (pause == true) { cout << "Code paused, press 'p' again to resume." << endl;
						while (pause == true)
						{
							switch (waitKey())
							{
								case 112:
									pause = false;
									cout << "Code resumed." << endl;
									break;
							}
						}
					}
			}
		}
		capture.release();
	}
}

/* ----- Print all the options available ----- */
void print_options()
{
	printf("Options (command line arguments):\n");
	printf("\tblur - Blur Image (image)\n");
	printf("\tcannyVideo - Canny Edge Detector from Camera\n");
	printf("\tdisplayImage - Display Image (image)\n");
	printf("\tobjectDetection - Detect Moving Object in Video (video,threshold)\n");
	printf("\tvideoCapture - Capture Video from Camera\n");
	printf("\tvideoFromFile - Display Video from File (video)\n");
	printf("\n");
}

/* ----- Capture video from camera ----- */
void video_capture()
{
	printf("Capturing video\n");

	VideoCapture cap(0);
	if ( !cap.isOpened() )
	{
		cout << "Cannot open the video cam" << endl;
		return;
	}

	double dWidth = cap.get ( CV_CAP_PROP_FRAME_WIDTH );
	double dHeight = cap.get ( CV_CAP_PROP_FRAME_HEIGHT );

	cout << "Frame size: " << dWidth << " x " << dHeight << endl;
	namedWindow ( "MyVideo", CV_WINDOW_AUTOSIZE );
	
	while ( true )
	{
		Mat frame;
		bool bSuccess = cap.read ( frame );
		if ( !bSuccess )
		{
			cout << "Cannot read a frame from video stream" << endl;
			break;
		}
		imshow ( "MyVideo", frame );
		if ( waitKey(30) == 27 )
		{
			cout << "esc key is pressed by user" << endl;
			break;
		}
	}
}

/* ----- Capture video from a file ----- */
void video_from_file(char *vidName)
{
	printf("Displaying the video from file '%s'.\n");

	cvNamedWindow ( "Video", CV_WINDOW_AUTOSIZE );
	CvCapture* capture = cvCreateFileCapture( vidName );
	IplImage* frame;
	while ( true )
	{
		frame = cvQueryFrame ( capture );
		if ( !frame ) break;
		cvShowImage ( "Video", frame );
		char c = cvWaitKey(33);
		if ( c == 27 ) break;
	}
	cvReleaseCapture ( &capture );
	cvDestroyWindow ( "Video" );
}
