#include "cvHeader.hpp"

int main(int argc, char **argv)
{
	if (argc < 2) {
		printf("Usage: ./main <function> <data> <sensitivity>\n");
		return -1;
	}

	string function, data;
	int sensitivity;
	if (argc == 2)
		function = argv[1];
	else if (argc == 3) {
		function = argv[1];
		data = argv[2];
	}
	else if (argc == 4) {
		function = argv[1];
		data = argv[2];
		sensitivity = atoi(argv[3]);
	}

	if (function.compare("--options") == 0)
		print_options();
	else if (function.compare("blur") == 0)
		blur_image(data);
	else if (function.compare("videoCapture") == 0)
		video_capture();
	else if (function.compare("cannyVideo") == 0)
		canny_video();
	else if (function.compare("displayImage") == 0)
		display_image(data);
	else if (function.compare("videoFromFile") == 0)
		video_from_file(data);
	else if (function.compare("objectDetection") == 0)
		object_tracking(data, sensitivity);
	else
		printf("Unknown option\n");

	return 0;
}
