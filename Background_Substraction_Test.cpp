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
	
	//������� ������
	Ptr<BackgroundSubtractor> pBackSub;
	
		pBackSub = createBackgroundSubtractorKNN(); //������ �������� ��������� ����
	VideoCapture capture(0); //������� ����������
	
	Mat frame, fgMask; //������� ��� �����
	while (true) 
	{
		capture >> frame;
		
		pBackSub->apply(frame, fgMask); //�������� ��������� ���� �� ���� � �� �����

		
		imshow("Frame", frame); // ���������� ��� �����, �������� � � ������
		imshow("FG Mask", fgMask);
		//����� ����� � ������ ���� �������� ��� ���������
		int keyboard = waitKey(30);
		if (keyboard == 'q' || keyboard == 27)
			break;
	}
	return 0;
}