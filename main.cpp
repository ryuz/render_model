
#include <stdio.h>
#include "opencv2/opencv.hpp"
#include "Render.h"


// レンダリング
class MyRender : public Render<float>
{
public:


protected:
	// バーテックスシェーダー
	void VertexShader(ShaderParam& param) {
		param.vector = m_Matrix * param.vector;
	}

	// ピクセルシェーダー
	void PixelShader(ShaderParam& param)
	{
		// 座標取り出し
		cv::Point pt;
		pt.x = (int)param.vector.at<float>(0, 0);
		pt.y = (int)param.vector.at<float>(1, 0); 
		float	z = param.vector.at<float>(2, 0); 

		// 深度テスト
		if ( z < m_DepthBuffer.at<float>(pt) ) {
			return;
		}

		m_ImageBuffer.at<cv::Vec3b>(pt)[0] = (uchar)param.color[0];
		m_ImageBuffer.at<cv::Vec3b>(pt)[1] = (uchar)param.color[1];
		m_ImageBuffer.at<cv::Vec3b>(pt)[2] = (uchar)param.color[2];

		// テクスチャ(とりあえずニアレストネイバー)
		float	u = param.color[3];
		float	v = param.color[4];
		int uu = (int)((m_Textures[0].cols-1) * u + 0.5);
		int vv = (int)((m_Textures[0].rows-1) * v + 0.5);
	//	m_ImageBuffer.at<cv::Vec3b>(pt) = m_Textures[0].at<cv::Vec3b>(vv, uu);
	}
};


// メイン関数
int main()
{
	// レンダー生成
	MyRender	r;

	MyRender::ShaderParam	vertex[3];
	cv::Mat imgTexture = cv::imread("DSC_0030.jpg");
	r.SetTexture(imgTexture);

//	vertex[0].vector = (cv::Mat_<float>(4, 1) << 100, 100, 0, 1);
	vertex[0].vector = (cv::Mat_<float>(4, 1) << -0.9, 0.9, 0, 1);
	vertex[0].color.push_back(255);
	vertex[0].color.push_back(0);
	vertex[0].color.push_back(0);

	vertex[0].color.push_back(0);
	vertex[0].color.push_back(0);

	vertex[1].vector = (cv::Mat_<float>(4, 1) << 0.1, -0.8, 0, 1);
	vertex[1].color.push_back(0);
	vertex[1].color.push_back(255);
	vertex[1].color.push_back(0);

	vertex[1].color.push_back(1);
	vertex[1].color.push_back(0);

//	vertex[2].vector = (cv::Mat_<float>(4, 1) << 0.9, 0.8, 0, 1);
	vertex[2].vector = (cv::Mat_<float>(4, 1) << 0.0, 0.0, 0, 1);
	vertex[2].color.push_back(0);
	vertex[2].color.push_back(0);
	vertex[2].color.push_back(255);

	vertex[2].color.push_back(0);
	vertex[2].color.push_back(1);
	
	r.MatrixMode(MyRender::PROJECTION);
	r.Perspective(30.0, 1.0, 0.1, 100.0);
	r.LookAt(cv::Vec3f(0, 0, 10), cv::Vec3f(0, 0, 0), cv::Vec3f(0, 1, 0));
	
	
	r.DrawTriangle(vertex);


	/*
	cv::Point_<float>	v[3];
	cv::Mat_<float>		z[3];
	v[0] = cv::Point2f(100, 100);
	v[1] = cv::Point2f(300, 420);
	v[2] = cv::Point2f(600, 150);
	z[0] = (cv::Mat_<float>(3, 1) << 255, 0, 0);
	z[1] = (cv::Mat_<float>(3, 1) << 0, 255, 0);
	z[2] = (cv::Mat_<float>(3, 1) << 0, 0, 255);
	r.Rasterise(v, z);
	*/

	cv::imshow("hoge", r.GetImage());
	cv::waitKey();

	return 0;
}



