// MySceondFR.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "vector"
#include "math.h"
#include<opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/imgproc/imgproc.hpp>
#include<iostream>

using namespace std;
using namespace cv;

#define TV_IV 1	// threshold value  兴趣点的阈值
#define w	5		//兴趣点搜索窗口
#define w2  17		//极值窗口


#define m_		5		//目标窗口和搜索窗口
#define n_		11
#define k_		15		//搜索区
#define l_		17		


//像素特征值
struct FeaturePoint {
	int value;
	int rows;
	int cols;
};

//存储同名点在两图特征值的位置
struct counter {
	int p1;
	int p2;
};

int num1 = 0, num2 = 0;		//特征点的个数

int GetMin(int *V, int num) {
	int min = 1000000;
	for (int i = 0; i < num; i++) {
		if (V[i] < min)
			min = V[i];
	}
	return min;
}

double GetMax(double *V, int num) {
	double max = -10;
	for (int i = 0; i < num; i++) {
		if (abs(V[i]) > max)
			max = abs(V[i]);
	}
	return max;
}

//F为兴趣点（特征点）
void ShowMark(Mat M, FeaturePoint *F,int numF) {
	
	for (int i = 0; i < M.rows; i++) {
		for (int j = 0; j < M.cols; j++) {
			for (int k = 0; k < numF; k++) {
				if (F[k].rows == i&&F[k].cols == j) {
					for (int p = -2; p < 5; p++) {
						M.at<uchar>(i + p, j + p) = 0;
						M.at<uchar>(i + p, j - p) = 0;
					}
				}
			}
			
		}
	}
	namedWindow("MarkPic",0);
	imshow("MarkPic",M);
	waitKey(0);

}

//获取w*w窗口范围内的兴趣点	Moravec算子,在此兴趣点便等于特征点
int GetIV(Mat M, int c, int r) {
	//前期工作
	int cols = M.cols;
	int rows = M.rows;
	int k = int(w / 2);
	int F = 0;

	//遍历图像，寻找各个窗口内的兴趣点
	int V[4] = { 0,0,0,0 };

	for (int i = -k; i <= k - 1; i++) {
		V[0] += pow((M.at<uchar>(c + i, r) - M.at<uchar>(c + i + 1, r)), 2);

		V[1] += pow((M.at<uchar>(c + i, r + i) - M.at<uchar>(c + i + 1, r + i + 1)), 2);

		V[2] += pow((M.at<uchar>(c, r + i) - M.at<uchar>(c, r + i + 1)), 2);

		V[3] += pow((M.at<uchar>(c + i, r - i) - M.at<uchar>(c + i + 1, r - i - 1)), 2);

	}

	F = GetMin(V, 4);

	return F;
}

//特征点提取	F用于存储特征点
int GetFeature(Mat M, FeaturePoint *Candidate) {
	//前期工作
	int cols = M.cols;
	int rows = M.rows;
	int c = 0, r = 0;
	int numIV = 0;
	FeaturePoint *temp=(FeaturePoint*)malloc(sizeof(FeaturePoint)*2000);
	//FeaturePoint *Candidate;		//存储兴趣点

	
	int k = 0;
	for (c = int(w / 2); c < rows - int(w / 2); c++) {
		for (r = int(w / 2); r < cols - int(w / 2); r++) {
			if (GetIV(M, c, r) > 2500) {
				temp[k].value = GetIV(M, c, r);
				temp[k].rows = c;	temp[k].cols = r;
				k++;
			}
			else {
				temp[k].value = 0;
				temp[k].rows = c;	temp[k].cols = r;
				k++;
			}
		}
	}
	
	numIV = k;
	Candidate = (FeaturePoint*)malloc(sizeof(FeaturePoint)*numIV);	//存储兴趣点

	for (k=0; k < numIV; k++) {
		Candidate[k].value = temp[k].value;
		Candidate[k].rows = temp[k].rows;
		Candidate[k].cols = temp[k].cols;
	}

	free(temp);
	k = 0;
	FeaturePoint max = {0,0,0};
	int numF = 0;

	cout << "numIV="<<numIV << endl;

	//打印候选点
	for (int k = 0; k < numIV; k++) {
		cout << "Candidate=" << Candidate[k].value << endl;
	}

	FeaturePoint MaxIV = { 0,0,0 };
	for (int i = 0; i < rows - w2; i++) {
		for (int j = 0; j < cols - w2; j++) {
			for (int m = 0; m < w2; m++) {
				for (int n = 0; n < w2; n++) {
					if (m == 0 && n == 0) {
						
					}
				}
			}
		}
	}







	//ShowMark(M, Candidate, numIV);










	////滑动搜索最大值的窗口
	//for (int i = int(w2 / 2); i < rows - int(w2 / 2); i+=w2) {
	//	for (int j = int(w2 / 2); j < cols - int(w2 / 2); j+=w2) {
	//		cout << "i=" << i << "j=" << j;
	//		for (k = 0; k < numIV; k++) {
	//			//判断该候选点是否在搜索区
	//			if ((Candidate[k].rows >= i - int(w2 / 2)) && (Candidate[k].rows < i + int(w2 / 2)) && (Candidate[k].cols >= j - int(w2 / 2)) && (Candidate[k].cols < j + int(w2 / 2))) {
	//				if (Candidate[k].value > max.value) {	//求区域极大值
	//					max.value = Candidate[k].value;
	//					max.rows = Candidate[k].rows;
	//					max.cols = Candidate[k].cols;
	//				}
	//				else {
	//					Candidate[k].value = 0;				//若非极大值则赋值为0
	//				}
	//			}
	//		}
	//		//将该窗口的极大值作为特征值存入
	//		F[numF].value = max.value;
	//		F[numF].rows = max.rows;
	//		F[numF].cols = max.cols;
	//		cout << "numF=" << numF << endl;
	//		numF++;
	//		max = { 0,0,0 };
	//	}
	//}


	cout << "numF= " << numF << endl;
	//cout << "k=" << k << endl;
	return numIV;

	

}

//获取搜索窗口内的相关系数
double GetRelated(Mat M1, Mat M2, counter *c, FeaturePoint *F1, FeaturePoint *F2, int k, int r_, int c_) {

	double fenzi = 0, fenmu = 0;
	double sum1 = 0, sum2 = 0, sum3 = 0, sum4 = 0, sum5 = 0, sum6 = 0;
	//cout << "见到k的第" << k << "次" << endl;
	for (int i = 0; i < m_; i++) {
		for (int j = 0; j < n_; j++) {
			sum1 += M1.at<uchar>(i + F1[c[k].p1].rows - k_ / 2+r_, j + F1[c[k].p1].cols - l_ / 2+c_) * M1.at<uchar>(i + r_ + F1[c[k].p1].rows - k_ / 2 + r_, j + c_ + F1[c[k].p1].cols - l_ / 2 + c_);

			sum2 += M1.at<uchar>(i + F1[c[k].p1].rows - k_ / 2 + r_, j + F1[c[k].p1].cols - l_ / 2 + c_)*M2.at<uchar>(i + r_ + F2[c[k].p2].rows - k_ / 2 + r_, j + c_ + F2[c[k].p2].cols - l_ / 2 + c_);

			sum3 += pow(M1.at<uchar>(i + F1[c[k].p1].rows - k_ / 2 + r_, j + F1[c[k].p1].cols - l_ / 2 + c_), 2);

			sum4 += M1.at<uchar>(i + F1[c[k].p1].rows - k_ / 2 + r_, j + F1[c[k].p1].cols - l_ / 2 + c_);

			sum5 += pow(M2.at<uchar>(i + r_ + F2[c[k].p2].rows - k_ / 2 + r_, j + c_ + F2[c[k].p2].cols - l_ / 2 + c_), 2);

			sum6 += M2.at<uchar>(i + r_ + F2[c[k].p2].rows - k_ / 2 + r_, j + c_ + F2[c[k].p2].cols - l_ / 2 + c_);

		}
	}

	
	fenzi = sum1 - 1 * (m_*n_)*sum2;
	fenmu = sqrt((sum3 - 1 * (m_*n_)*pow(sum4,2))*(sum5 - 1 * (m_*n_)*pow(sum6,2)));

	

	return fenzi / fenmu;
}

//模板匹配
void Matching(Mat M1,Mat M2,FeaturePoint* F,int num) {
	
	//前期工作

	int cols_1 = M1.cols;
	int rows_1 = M1.rows;
	int cols_2 = M2.cols;
	int rows_2 = M2.rows;
	




}

//对二图进行特征点提取和模板匹配
void Function(Mat M1, Mat M2) {

	//前期工作（获得影像长宽等）
	int cols_1 = M1.cols;
	int rows_1 = M1.rows;
	int cols_2 = M2.cols;
	int rows_2 = M2.rows;

	int V[4] = { 0,0,0,0 };
	//FeaturePoint F1[1000], F2[2000];	//存储特征点的像素值和坐标
	FeaturePoint *F1 = (FeaturePoint*)malloc(sizeof(FeaturePoint));
	int cnum = 0;				//同名点个数
	counter *c;

	int Rnum = 2000;			
	double *RelateWin = (double*)malloc(sizeof(double)*Rnum);	//一个窗口上的相关系数
	double Relate[1000];
 
	//提取左右两图的特征点

	num1 = GetFeature(M1, F1);

	//根据特征点确定两幅图像的同名点



	












	//c = (struct counter*)malloc(sizeof(counter)*cnum);
	//int k = 0;
	//for (int i = 0; i < num1; i++) {
	//	for (int j = 0; j < num2; j++) {
	//		if ((F1[i].value == F2[j].value)) {
	//			c[k].p1 = i; c[k].p2 = j;			//得到同名点在两幅图上的具体位置和其特征值
	//			//cout <<"k="<<k<< "   p1=" << c[k].p1 << "	p2=" << c[k].p2 << endl;
	//			k++;
	//		}
	//	}
	//}
	//根据同名点确定搜索区
	//搜索同名点
	//k = 0;
	//int p = 0, q = 0;
	//int c_ = 0, r_ = 0;
	//for (int k = 0; k < cnum; k++) {	//逐步遍历同名点
	//	//判断同名点的搜索区是否在图像内
	//	if ((F1[c[k].p1].x - int(k_ / 2) >= 0) && (F1[c[k].p1].y - int(l_ / 2) >= 0) && (F2[c[k].p2].x - int(k_ / 2) >= 0) && (F2[c[k].p2].y - int(l_ / 2) >= 0) && (F1[c[k].p1].x + int(k_ / 2) < rows_1) && (F1[c[k].p1].y + int(l_ / 2) < cols_1) && (F2[c[k].p2].x + int(k_ / 2) < rows_2) && (F2[c[k].p2].y + int(l_ / 2) < cols_2)) {
	//		cout << "k=" << k << endl;
			//在搜索区内逐窗口求相关系数
			//for (r_ = int(m_ / 2) + 1; r_ <= k_ - int(m_ / 2); r_++) {
			//	for (c_ = int(n_ / 2) + 1; c_ <= l_ - int(n_ / 2); c_++) {
			//		RelateWin[p] = GetRelated(M1, M2, c, F1, F2, 50, c_, r_);
			//		//cout << "p=" << p++ << endl;
			//		cout << "RelatedWin=" << RelateWin[p++] << endl;
			//	}
			//}
	//		Relate[q++] = GetMax(RelateWin, p);
	//		p = 0;
	//		cout << "q=" << q << endl;


	//	}
	//}

	//cout << "我到这里了" << endl;
	//double a=-1;
	//a = GetRelated(M1, M2, c, F1, F2, 100, 7, 7);
	//cout << "a=" << a << endl;




	// 在搜索区中逐窗口计算相关系数，取最大值为匹配点

}

int main()
{

	char img1[] = "u0369_panLeft.bmp";
	char img2[] = "u0367_panRight.bmp";

	Mat M1 = imread(img1, IMREAD_GRAYSCALE);
	Mat M2 = imread(img2, IMREAD_GRAYSCALE);

	if (M1.empty()||M2.empty() )    
	{
		printf("Can not load image %s\n", img1);
		return -1;
	}

	//namedWindow("Pic1", 0);
	//imshow("Pic1", M1);
	//namedWindow("Pic2", 0);
	//imshow("Pic2", M2);


	Function(M1, M2);


	//waitKey();

	cout << "Finish" << endl;
    return 0;
}

