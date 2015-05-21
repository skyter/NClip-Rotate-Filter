
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
	
	imgOri = imread(imagePath + filename, 1);//讀檔

	if ( imgOri.empty()) // 檔案不存在  跳出
		return -1;
	
	// 先找出最大可能的邊
	int squareSize = sqrt(pow(imgOri.size().width, 2) + pow(imgOri.size().height, 2)); 
	
	// 給定最大邊的方陣其值都為零
	Mat largeOri = Mat::zeros(squareSize, squareSize, CV_8UC3); 	
	
	//拷貝值原矩陣到最大矩陣  可避免因為旋轉所丟失的資料
	for (size_t j = 0; j < imgOri.size().height; j++){
		for (size_t i = 0; i < imgOri.size().width; i++){
			largeOri.at<Vec3b>((squareSize - imgOri.size().height) / 2 + j, (squareSize - imgOri.size().width) / 2 + i) = imgOri.at<Vec3b>(j, i);
			
		}
	}
	
	//計算要選轉的角度
	double RotateDegree = acos( acc_data[0] / sqrt(pow(acc_data[0], 2) + pow(acc_data[1], 2)))*180.0/PI;
	
	
	//考慮向量後做修正
	if (acc_data[0] < 0 && acc_data[1] > 0){
		RotateDegree = RotateDegree-180;
	}
	else if (acc_data[0] < 0 && acc_data[1] < 0){
		RotateDegree = 180 - RotateDegree;
	}
	else if (acc_data[0] > 0 && acc_data[1] > 0){
		RotateDegree = 180 + RotateDegree;
	}
	
	// 計算旋轉矩陣
	Mat matRotation = getRotationMatrix2D(Point(largeOri.cols / 2, largeOri.rows / 2), RotateDegree, 1.0);

	Mat imgRotated;
	
	// 利用旋轉矩陣  旋轉整個大圖
	warpAffine(largeOri, imgRotated, matRotation, Size(squareSize, squareSize));

	// 修正角度 至 0~90
	while (RotateDegree >= 360)
		RotateDegree -= 360;
	while (RotateDegree < 0)
		RotateDegree += 360;
	if (RotateDegree > 180 )
		RotateDegree = RotateDegree -180;
	if (RotateDegree > 90)
		RotateDegree = 180 - RotateDegree;

	// 計算要抓取的矩形  為了觀看方便  我選擇維持原比例  並把原圖的中心  視為新矩陣的中心
	
	Rect cutRegion = Rect( largeOri.cols / 2 - imgOri.cols*sin(RotateDegree*PI / 180) / 2, 
		                   largeOri.rows / 2 - imgOri.cols*sin(RotateDegree*PI / 180) * imgOri.rows / imgOri.cols / 2, 
						   imgOri.cols*sin(RotateDegree*PI / 180), 
						   imgOri.cols*sin(RotateDegree*PI / 180) * imgOri.rows / imgOri.cols);
	
	
	// 從矩陣當中  抓取矩形內的質
	Mat cutImg = imgRotated(cutRegion);
	
#if SAVE_CUT_FILE
	//找.jpg當中.的位置
	size_t pos = filename.find_first_of('.'); 

	// 找不到  跳出
	if (pos == std::string::npos) 
		return -1;
	
	// 設定要被切割的檔名 使之不會蓋過原擋名
	filename.insert(pos, "_CUT"); 
	
	// 寫入檔案
	imwrite(imagePath + filename, cutImg);
#endif

	//秀出結果  debug用
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

