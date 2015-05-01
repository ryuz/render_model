
#include <stdio.h>
#include "opencv2/opencv.hpp"
#include "Render.h"


class MyRender : public Render<float>
{
	void PixcelProc(cv::Point pt, cv::Mat_<float> z) {
		m_img.at<cv::Vec3b>(pt)[0] = z(0, 0);
		m_img.at<cv::Vec3b>(pt)[1] = z(1, 0);
		m_img.at<cv::Vec3b>(pt)[2] = z(2, 0);
	}
};


int main()
{
	MyRender	r;
	
	cv::Mat img = cv::Mat::zeros(480, 640, CV_8UC3);
	r.SetImage(img);

	cv::Point_<float>	v[3];
	cv::Mat_<float>		z[3];
	v[0] = cv::Point2f(100, 100);
	v[1] = cv::Point2f(300, 420);
	v[2] = cv::Point2f(600, 150);
	z[0] = (cv::Mat_<float>(3, 1) << 255, 0, 0);
	z[1] = (cv::Mat_<float>(3, 1) << 0, 255, 0);
	z[2] = (cv::Mat_<float>(3, 1) << 0, 0, 255);
	
	r.Rasterise(v, z);

	cv::imshow("hoge", img);
	cv::waitKey();

	return 0;
}



