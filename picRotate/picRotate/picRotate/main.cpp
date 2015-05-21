
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <cmath>


#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#define SHOW_IMAGE_DEBUG 1
#define SAVE_CUT_FILE 0

#define PI 3.1415926
using namespace std;
using namespace cv;



int main(int argc, char *argv[])
{

	int acc_data[3] = { -765 ,-625, -156};
	
	Mat imgOri;
	
	String imagePath = "D:\\video\\pic\\";
	
	String filename  = "event_150413_024218_45010053454d3038479001d0b521510f.jpg";
	
	imgOri = imread(imagePath + filename, 1);//Ū��

	if ( imgOri.empty()) // �ɮפ��s�b  ���X
		return -1;
	
	// ����X�̤j�i�઺��
	int squareSize = sqrt(pow(imgOri.size().width, 2) + pow(imgOri.size().height, 2)); 
	
	// ���w�̤j�䪺��}��ȳ����s
	Mat largeOri = Mat::zeros(squareSize, squareSize, CV_8UC3); 	
	
	//�����ȭ�x�}��̤j�x�}  �i�קK�]������ҥᥢ�����
	for (size_t j = 0; j < imgOri.size().height; j++){
		for (size_t i = 0; i < imgOri.size().width; i++){
			largeOri.at<Vec3b>((squareSize - imgOri.size().height) / 2 + j, (squareSize - imgOri.size().width) / 2 + i) = imgOri.at<Vec3b>(j, i);
			
		}
	}
	
	//�p��n���઺����
	double RotateDegree = acos( acc_data[0] / sqrt(pow(acc_data[0], 2) + pow(acc_data[1], 2)))*180.0/PI;
	
	
	//�Ҽ{�V�q�ᰵ�ץ�
	if (acc_data[0] < 0 && acc_data[1] > 0){
		RotateDegree = RotateDegree-180;
	}
	else if (acc_data[0] < 0 && acc_data[1] < 0){
		RotateDegree = 180 - RotateDegree;
	}
	else if (acc_data[0] > 0 && acc_data[1] > 0){
		RotateDegree = 180 + RotateDegree;
	}
	
	// �p�����x�}
	Mat matRotation = getRotationMatrix2D(Point(largeOri.cols / 2, largeOri.rows / 2), RotateDegree, 1.0);

	Mat imgRotated;
	
	// �Q�α���x�}  �����Ӥj��
	warpAffine(largeOri, imgRotated, matRotation, Size(squareSize, squareSize));

	// �ץ����� �� 0~90
	while (RotateDegree >= 360)
		RotateDegree -= 360;
	while (RotateDegree < 0)
		RotateDegree += 360;
	if (RotateDegree > 180 )
		RotateDegree = RotateDegree -180;
	if (RotateDegree > 90)
		RotateDegree = 180 - RotateDegree;

	// �p��n������x��  ���F�[�ݤ�K  �ڿ�ܺ�������  �ç��Ϫ�����  �����s�x�}������
	
	Rect cutRegion = Rect( largeOri.cols / 2 - imgOri.cols*sin(RotateDegree*PI / 180) / 2, 
		                   largeOri.rows / 2 - imgOri.cols*sin(RotateDegree*PI / 180) * imgOri.rows / imgOri.cols / 2, 
						   imgOri.cols*sin(RotateDegree*PI / 180), 
						   imgOri.cols*sin(RotateDegree*PI / 180) * imgOri.rows / imgOri.cols);
	
	
	// �q�x�}��  ����x�Τ�����
	Mat cutImg = imgRotated(cutRegion);
	
#if SAVE_CUT_FILE
	//��.jpg��.����m
	size_t pos = filename.find_first_of('.'); 

	// �䤣��  ���X
	if (pos == std::string::npos) 
		return -1;
	
	// �]�w�n�Q���Ϊ��ɦW �Ϥ����|�\�L��צW
	filename.insert(pos, "_CUT"); 
	
	// �g�J�ɮ�
	imwrite(imagePath + filename, cutImg);
#endif

	//�q�X���G  debug��
#if SHOW_IMAGE_DEBUG
	cvNamedWindow("Ori", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("Rotate", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("Cut", CV_WINDOW_AUTOSIZE);
	
	while (1)
	{
		imshow("Ori", imgOri);
		imshow("Rotate", imgRotated);
		imshow("Cut", cutImg);
		
		if (cvWaitKey(1) == 27)
			break;
	}
#endif
	imgOri.release();
	largeOri.release();
	matRotation.release();
	imgRotated.release();
	cutImg.release();
	return 0;
}

