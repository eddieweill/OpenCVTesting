#include "cvHeader.hpp"

int main(int argc, char **argv)
{
	if (argc < 2)
	{
		printf("Usage: ./main <function> <data>\n");
		return -1;
	}

	string selection;
	strcpy(selection, argv[1]);

	if (strcmp("--options", selection) == 0)
		print_options();
	else if (strcmp("blur", selection) == 0)
		blur_image(selection);
	else if (strcmp("videoCapture", selection) == 0)
		video_capture();
	else if (strcmp("cannyVideo",selection) == 0)
		canny_video();
	else if (strcmp("displayImage", selection) == 0)
		display_image(selection);
	else if (strcmp("videoFromFile", selection) == 0)
		video_from_file(selection);
	else if (strcmp("objectDetection", selection) == 0)
		object_tracking(selection);
	else
		printf("Unknown option.\n");

	return 0;
}