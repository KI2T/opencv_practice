#include <iostream>
#include <sstream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>

using namespace cv;
using namespace std;

int main()
{
	
	//создали проект
	Ptr<BackgroundSubtractor> pBackSub;
	
		pBackSub = createBackgroundSubtractorKNN(); //задали алгоритм вырезания фона
	VideoCapture capture(0); //подняли устройство
	
	Mat frame, fgMask; //создали два кадра
	while (true) 
	{
		capture >> frame;
		
		pBackSub->apply(frame, fgMask); //скормили алгоритму кадр на вход и на выход

		
		imshow("Frame", frame); // отобразили оба кадра, исходный и с маской
		imshow("FG Mask", fgMask);
		//ловим выход и заодно даем задержку для отрисовки
		int keyboard = waitKey(30);
		if (keyboard == 'q' || keyboard == 27)
			break;
	}
	return 0;
}