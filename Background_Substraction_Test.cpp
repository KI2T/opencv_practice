#include <iostream>
#include <sstream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>

using namespace cv;
using namespace std;

int main(int argc, char* argv[])
{
	
	//create Background Subtractor objects
	Ptr<BackgroundSubtractor> pBackSub;
	
		//pBackSub = createBackgroundSubtractorMOG2();
	
		pBackSub = createBackgroundSubtractorKNN();
	VideoCapture capture(0);
	if (!capture.isOpened()) {
		//error in opening the video input
		cerr << "Unable to open camera" << endl;
		return 0;
	}
	Mat frame, fgMask;
	while (true) {
		capture >> frame;
		if (frame.empty())
			break;
		//update the background model
		pBackSub->apply(frame, fgMask);

		//show the current frame and the fg masks
		imshow("Frame", frame);
		imshow("FG Mask", fgMask);
		//get the input from the keyboard
		int keyboard = waitKey(30);
		if (keyboard == 'q' || keyboard == 27)
			break;
	}
	return 0;
}