

#ifndef __RYUZ__RENDER_H__
#define __RYUZ__RENDER_H__


#include <vector>
#include "opencv2/opencv.hpp"


template <typename T>
class Render
{
public:
	void		SetImage(cv::Mat img)	{ m_img = img; }
	cv::Mat&	GetImage(void)			{ return m_img; }
	
	T	Min(T a, T b)	{ return a < b ? a : b; }
	T	Min(T a, T b, T c) { return Min(Min(a, b), c); }
	T	Min(T a, T b, T c, T d) { return Min(Min(a, b), Min(c, d)); }
	T	Max(T a, T b)	{ return a > b ? a : b; }
	T	Max(T a, T b, T c) { return Max(Max(a, b), c); }
	T	Max(T a, T b, T c, T d) { return Max(Max(a, b), Max(c, d)); }
	
	virtual void PixcelProc(cv::Point pt, cv::Mat_<T> z) = 0;
//	{
//		printf("hoge\n");
//	}

	void Rasterise(cv::Point_<T> v[3], cv::Mat_<T> z[3])
	{
		// 範囲算出
		int	x0 = (int)(Min(v[0].x, v[1].x, v[2].x, (T)(m_img.cols-1)) + (T)0.5);
		int	y0 = (int)(Min(v[0].y, v[1].y, v[2].y, (T)(m_img.rows-1)) + (T)0.5);
		int	x1 = (int)(Max(v[0].x, v[1].x, v[2].x, (T)0) + (T)0.5);
		int	y1 = (int)(Max(v[0].y, v[1].y, v[2].y, (T)0) + (T)0.5);
		
		// 領域内判定式作成
		cv::Vec<T, 3>	e0;
		cv::Vec<T, 3>	edx;
		cv::Vec<T, 3>	edy;
		for ( int i = 0; i < 3; i++ ) {
			int j = (i+1) % 3;
			edx[i] = v[j].x - v[i].x;
			edy[i] = v[j].y - v[i].y;
			e0[i]  = ((T)x0 - v[i].x) * edy[i] - ((T)y0 - v[i].y) * edx[i];
		}
		
		// 記述を楽にするためにXYもMat化
		cv::Mat_<T> x[3];
		cv::Mat_<T> y[3];
		for ( int i = 0; i < 3; i++ ) {
			x[i] = cv::Mat_<T>::zeros(z[i].size()) + v[i].x;
			y[i] = cv::Mat_<T>::zeros(z[i].size()) + v[i].y;
		}
		
		// 平面の式(a*x + b*y + c*z + d = 0)の係数計算(外積)
		cv::Mat_<T>	v0x = x[1] - x[0];
		cv::Mat_<T>	v0y = y[1] - y[0]; 
		cv::Mat_<T>	v0z = z[1] - z[0];
		cv::Mat_<T>	v1x = x[2] - x[0]; 
		cv::Mat_<T>	v1y = y[2] - y[0];
		cv::Mat_<T>	v1z = z[2] - z[0];
		cv::Mat_<T>	a = v0y.mul(v1z) - v0z.mul(v1y);
		cv::Mat_<T>	b = v0z.mul(v1x) - v0x.mul(v1z);
		cv::Mat_<T>	c = v0x.mul(v1y) - v0y.mul(v1x);

		// グーロー計算用係数生成
		cv::Mat_<T>	zdx = -a.mul(1/c);
		cv::Mat_<T>	zdy = -b.mul(1/c);
		cv::Mat_<T>	dd  = z[0] - zdx.mul(x[0]) - zdy.mul(y[0]);
		cv::Mat_<T>	z0  = (zdx * x0) + (zdy * y0) + dd;

		// ラスタライズ
		cv::Vec<T, 3>	ey = e0;
		cv::Mat_<T>		zy = z0;
		cv::Point		pt;
		for ( pt.y = y0; pt.y <= y1; pt.y++ ) {
			cv::Vec<T, 3>	ex = ey;
			cv::Mat_<T>		zx = zy.clone();
			for ( pt.x = x0; pt.x <= x1; pt.x++ ) {
				if ( ex[0] >= 0 && ex[1] >= 0 && ex[2] >= 0 ) {
					PixcelProc(pt, zx);
				}
				ex += edy;
				zx += zdx;
			}
			ey -= edx;
			zy += zdy;
		}
	}

protected:
	cv::Mat		m_img;
};


#endif	// __RYUZ__RENDER_H__

