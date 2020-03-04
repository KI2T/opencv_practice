#include <dos.h>
#include "windows.h"
#include <iostream>
#include <sstream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>

using namespace cv;
using namespace std;

int mode = 0;
int e1,e2,d1,d2,erosion_type;
VideoCapture capture(0);
Mat frame, fgMask, grey, linemat;
int waitq()
{
	int key = waitKey(30);
	if (key == 'q' || key == 27)
		return 0;

	else
		return 1;
}

void FilterTest()
{
	Mat image1, image2;
	Mat grayscale1, grayscale2;
	Mat absOut, thresholdImg;

	while (waitq())
	{
		capture >> frame;
		capture >> image1;
		cvtColor(image1, grayscale1, COLOR_BGR2GRAY); 
		Sleep(100);
		capture >> image2;
		cvtColor(image2, grayscale2, COLOR_BGR2GRAY); 

		absdiff(grayscale1, grayscale2, absOut); 
		imshow("Test", absOut);
	}

}

void BackSubstr()
{
	int t1, t2;
	cout << "Starting Background substractor\n";
	cout << "Thr params:\n";
	cin >> t1;
	cin >> t2;
	cout << "Er params:\n";
	cin >> e1; //0 1 2
	cin >> e2; //0-21. 3 норм
	cout << "Dil params:\n";
	cin >> d1; //0 1 2
	cin >> d2; // 0-21

	Ptr<BackgroundSubtractor> pBackSub;

	pBackSub = createBackgroundSubtractorKNN();
	//erose -> dilate 
	while (waitq() == 1)
	{
		capture >> frame;

		pBackSub->apply(frame, fgMask); 
		
		threshold(fgMask, fgMask,t1,t2,THRESH_BINARY);
		if (e1 == 0) { erosion_type = MORPH_RECT; }
		else if (e1 == 1) { erosion_type = MORPH_CROSS; }
		else if (e1 == 2) { erosion_type = MORPH_ELLIPSE; }

		Mat element = getStructuringElement(erosion_type,
			Size(2 * e2 + 1, 2 * e2 + 1),
			Point(e2, e2));

		erode(fgMask, fgMask, element);
		blur(fgMask, fgMask, Size(3, 3));

		vector<vector<Point> > contours;
		vector<Vec4i> hierarchy;
		findContours(fgMask, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

		for (size_t r = 0; r < contours.size(); r++)
		{		
			if (contourArea(contours[r]) > 200)
				rectangle(frame, boundingRect(contours[r]), Scalar(0, 255, 0),2);
		}
		

		imshow("Frame", frame); 
		imshow("FG Mask", fgMask);
		
	}
	cout << "Stopping Background Substractor\n";
}

void Findcircles()
{
	cout << "Starting Circle recogniser\n";
	while (waitq())
	{
		capture >> frame;
		cvtColor(frame, grey, COLOR_BGR2GRAY);
		medianBlur(grey, grey, 5);
		vector<Vec3f> circles;
		HoughCircles(grey, circles, HOUGH_GRADIENT, 1,
			grey.rows / 16,  // distance to each other
			100, 30, 
			1, 30 //(min_radius & max_radius)
		);

		for (size_t i = 0; i < circles.size(); i++)
		{
			Vec3i c = circles[i];
			Point center = Point(c[0], c[1]);
			// circle center
			circle(frame, center, 1, Scalar(0, 100, 100), 3, LINE_AA);
			// circle outline
			int radius = c[2];
			circle(frame, center, radius, Scalar(255, 0, 255), 3, LINE_AA);
		}

		imshow("Frame", frame);
		
	}
	cout << "Stopping Circle recogniser\n";
}

void Findlines()
{
	cout << "Starting line recogniser\n";
	
	while (waitq())
	{
		capture >> frame;
		cvtColor(frame, grey, COLOR_BGR2GRAY);
		Canny(grey, linemat, 50, 200, 3);

		vector<Vec2f> lines; 
		HoughLines(linemat, lines, 1, CV_PI / 180, 150, 0, 0); 

		for (size_t i = 0; i < lines.size(); i++)
		{
			float rho = lines[i][0], theta = lines[i][1];
			Point pt1, pt2;
			double a = cos(theta), b = sin(theta);
			double x0 = a * rho, y0 = b * rho;
			pt1.x = cvRound(x0 + 1000 * (-b));
			pt1.y = cvRound(y0 + 1000 * (a));
			pt2.x = cvRound(x0 - 1000 * (-b));
			pt2.y = cvRound(y0 - 1000 * (a));
			line(frame, pt1, pt2, Scalar(0, 0, 255), 3, LINE_AA);
		}
		imshow("Frame", frame);
		imshow("grey", grey);
	}
	cout << "Stopping line recogniser\n";
}

void Modes()
{
	cout << "Modes:\n 1 - Movement detector\n 2 - Circle recogniser\n 3 - Line recogniser\n 4 - exit\n ";
	cin >> mode;
	switch (mode)
	{
	case 0:
		
		break;
	case 1:
		BackSubstr();
		break;
	case 2:
		Findcircles();
		break;
	case 3: 
		Findlines();
		break;
	case 4: //exit case
		break;
	case 5: //filter test
		FilterTest();
	}

}



int main()
{

	while(mode != 4)
		Modes();
	return 0;
	cout << "Exiting...";
}