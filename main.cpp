
#include <stdio.h>
#include "opencv2/opencv.hpp"
#include "Render.h"


// �����_�����O
template <typename T, typename RT, typename GT>
class MyRender : public Render<T, RT, GT>
{
public:

protected:
	// �o�[�e�b�N�X�V�F�[�_�[
	void VertexShader(ShaderParam& param) {
//		std::cout << m_Matrix << std::endl;
		param.vector = m_Matrix * param.vector;
	}
	

	// �s�N�Z���V�F�[�_�[
	void PixelShader(ShaderParam& param)
	{
		// ���W���o��
		cv::Point pt;
		pt.x = (int)param.vector.at<T>(0, 0);
		pt.y = (int)param.vector.at<T>(1, 0); 
		T  z = param.vector.at<T>(2, 0); 

		// �[�x�e�X�g
		if ( z < m_DepthBuffer.at<T>(pt) ) {
			return;
		}
		m_DepthBuffer.at<T>(pt) = z;

#if 0
		m_ImageBuffer.at<cv::Vec3b>(pt)[0] = m_ImageBuffer.at<cv::Vec3b>(pt)[0] + 100;
		m_ImageBuffer.at<cv::Vec3b>(pt)[1] = m_ImageBuffer.at<cv::Vec3b>(pt)[1] + 100;
		m_ImageBuffer.at<cv::Vec3b>(pt)[2] = m_ImageBuffer.at<cv::Vec3b>(pt)[2] + 100;
#else
		m_ImageBuffer.at<cv::Vec3b>(pt)[0] = (uchar)param.color[0];
		m_ImageBuffer.at<cv::Vec3b>(pt)[1] = (uchar)param.color[1];
		m_ImageBuffer.at<cv::Vec3b>(pt)[2] = (uchar)param.color[2];
#endif

		// �e�N�X�`��(�Ƃ肠�����j�A���X�g�l�C�o�[)
#if 0
		// �␳�Ȃ�
		float	u = param.color[3];
		float	v = param.color[4];
#else
		// �␳����
		float	u = param.coordinate[0];
		float	v = param.coordinate[1];
#endif
		if ( u < 0 || u > 1.0f || v < 0 || v > 1.0f ) {
			return;
		}

//		int uu = (int)((m_Textures[0].cols-1) * u + 0.5);
//		int vv = (int)((m_Textures[0].rows-1) * v + 0.5);
		int uu = (int)((m_Textures[0].cols-1) * u);
		int vv = (int)((m_Textures[0].rows-1) * v);
//		m_ImageBuffer.at<cv::Vec3b>(pt) = m_Textures[0].at<cv::Vec3b>(vv, uu);
	}
};


// �`��e�X�g
template <typename T, typename RT, typename GT>
void RenderTest(void)
{
	// �����_�[����
	MyRender<T, RT, GT>	r;

	// �e�N�X�`���ݒ�
	cv::Mat imgTexture = cv::imread("checker.png");
//	cv::Mat imgTexture = cv::imread("white.png");
	r.SetTexture(imgTexture);

	// ���W�ݒ�
	r.Viewport(0, 0, 640, 480);
//	r.Viewport(0, 0, 1280, 768);
	r.MatrixMode(MyRender<T, RT, GT>::PROJECTION);
	r.Perspective(20.0f, 1.0f, 0.1f, 100.0f);
	r.LookAt(cv::Vec3f(0, 5, 3), cv::Vec3f(0, 0, 0), cv::Vec3f(0, 1, 0));
	

	// ������Ƃ��׋��p
	if (0){
		r.Perspective(20.0f, 1.0f, 1.5f, 100.0f);
		r.LookAt(cv::Vec3f(0, 0, 0), cv::Vec3f(0, 0, -1), cv::Vec3f(0, 1, 0));

		FILE *fp;
		fopen_s(&fp, "test.txt", "w");
		cv::Mat	mat = r.GetDrawMatrix();
		cv::Mat	vec = cv::Mat_<T>(4, 1);
		for ( T z = -200; z < 200; z += 0.01 ) {
			vec.at<T>(0, 0) = 0;
			vec.at<T>(1, 0) = 0;
			vec.at<T>(2, 0) = z;
			vec.at<T>(3, 0) = 1;
			cv::Mat cnv = mat * vec;
//			fprintf(fp, "%f %f\n", (double)z, (double)cnv.at<T>(2, 0));
			fprintf(fp, "%f %f\n", (double)z, (double)cnv.at<T>(2, 0)/cnv.at<T>(3, 0));
		}
		fclose(fp);
	}


	// �`��
	std::vector<MyRender<T, RT, GT>::ShaderParam>	vertex(4);

	vertex[0].vector = (cv::Mat_<T>(4, 1) << -1.0, -1.0, 0, 1);	// (x, y, z, 1)
//	vertex[0].vector = (cv::Mat_<T>(4, 1) <<  1.0,   0, 0, 1);	// (x, y, z, 1)
	vertex[0].color.push_back(255);		// B
	vertex[0].color.push_back(0);		// G
	vertex[0].color.push_back(0);		// R

	vertex[0].color.push_back(0);		// �e�N�X�`�����W u (�p�[�X�y�N�e�B�u�␳�����̏ꍇ�̎����p)
	vertex[0].color.push_back(0);		// �e�N�X�`�����W v (�p�[�X�y�N�e�B�u�␳�����̏ꍇ�̎����p)
	vertex[0].coordinate.push_back(0);	// �e�N�X�`�����W u
	vertex[0].coordinate.push_back(0);	// �e�N�X�`�����W v


	vertex[1].vector = (cv::Mat_<T>(4, 1) << +1.0, -1.0, 0, 1);
	vertex[1].color.push_back(0);
	vertex[1].color.push_back(255);
	vertex[1].color.push_back(0);

	vertex[1].color.push_back(1);
	vertex[1].color.push_back(0);
	vertex[1].coordinate.push_back(1);
	vertex[1].coordinate.push_back(0);


	vertex[2].vector = (cv::Mat_<T>(4, 1) << +1.0, +1.0, 0, 1);
	vertex[2].color.push_back(0);
	vertex[2].color.push_back(0);
	vertex[2].color.push_back(255);

	vertex[2].color.push_back(1);
	vertex[2].color.push_back(1);
	vertex[2].coordinate.push_back(1);
	vertex[2].coordinate.push_back(1);


	vertex[3].vector = (cv::Mat_<T>(4, 1) << -1.0, +1.0, 0, 1);
	vertex[3].color.push_back(255);
	vertex[3].color.push_back(0);
	vertex[3].color.push_back(255);

	vertex[3].color.push_back(0);
	vertex[3].color.push_back(1);
	vertex[3].coordinate.push_back(0);
	vertex[3].coordinate.push_back(1);
	
	r.DrawPolygon(vertex, MyRender<T, RT, GT>::QUADS);
	
	// �\��
	cv::imshow("view", r.GetImage());
	cv::waitKey();
}


#include "FixedPointNumber.h"


void test(void);
void rasterizer_test(void);


// ���C���֐�
int main()
{
	rasterizer_test();
//	test();
	return 0;

//	RenderTest< double, double, double >();
//	RenderTest< float, float, float >();
//	RenderTest< float, FixedPointNumber<8, 24>, float >();
	RenderTest< float, FixedPointNumber<8, 24>, FixedPointNumber<8, 24> >();
//	RenderTest< float, FixedPointNumber<14, 32>, FixedPointNumber<18, 32, long long> >();
	
	return 0;
}




void test(void)
{
	Render< float, float, float >	r;
	cv::Mat	imgSrc = cv::imread("DSC_0030.jpg");

	cv::resize(imgSrc, imgSrc, cv::Size(720, 480));

	cv::Mat	imgDst = cv::Mat::zeros(480, 640, CV_8UC3);
	cv::Mat	imgDst2 = cv::Mat::zeros(480, 640, CV_8UC3);

	cv::Mat mat = cv::Mat::eye(4, 4, CV_32F);

//	float theta = 3.141592 / 6;
	float	theta = 3.141592653589 / 64;


//	for (;;) {
//		theta += 3.141592 / 64;
	{
		theta += 3.141592653589 / 32;

		mat     = cv::Mat::eye(4, 4, CV_32F);
		imgDst  = cv::Mat::zeros(480, 640, CV_8UC3);
		imgDst2 = cv::Mat::zeros(480, 640, CV_8UC3);

		// �����Ɉړ�
		cv::Mat m;
		m = (cv::Mat_<float>(4, 4) <<
			1, 0, 0, -imgSrc.cols / 2,
			0, 1, 0, -imgSrc.rows / 2,
			0, 0, 1, 0,
			0, 0, 0, 1
			);
		mat = m * mat;

		// ��]
		m = (cv::Mat_<float>(4, 4) <<
			cos(theta), 0, sin(theta), 0,
			0, 1, 0, 0,
			-sin(theta), 0, cos(theta), 0,
			0, 0, 0, 1);
		mat = m * mat;

		// Z�����ɂ��炷
		m = (cv::Mat_<float>(4, 4) <<
			1, 0, 0, 0,//+imgDst.cols,
			0, 1, 0, 0,//+imgDst.rows,
			0, 0, 2.0 / imgSrc.cols, 1,
			0, 0, 0, 1
			);
		mat = m * mat;

		std::cout << mat << std::endl;

		cv::Mat mat1(3, 3, CV_32F);
		mat1.at<float>(0, 0) = mat.at<float>(0, 0);
		mat1.at<float>(0, 1) = mat.at<float>(0, 1);
		mat1.at<float>(0, 2) = mat.at<float>(0, 3);
		mat1.at<float>(1, 0) = mat.at<float>(1, 0);
		mat1.at<float>(1, 1) = mat.at<float>(1, 1);
		mat1.at<float>(1, 2) = mat.at<float>(1, 3);
		mat1.at<float>(2, 0) = mat.at<float>(2, 0);
		mat1.at<float>(2, 1) = mat.at<float>(2, 1);
		mat1.at<float>(2, 2) = mat.at<float>(2, 3);

		mat1.at<float>(0, 0) += mat.at<float>(2, 0) * (imgDst.cols / 2.0);
		mat1.at<float>(0, 1) += mat.at<float>(2, 1) * (imgDst.cols / 2.0);
		mat1.at<float>(0, 2) += mat.at<float>(2, 3) * (imgDst.cols / 2.0);
		mat1.at<float>(1, 0) += mat.at<float>(2, 0) * (imgDst.rows / 2.0);
		mat1.at<float>(1, 1) += mat.at<float>(2, 1) * (imgDst.rows / 2.0);
		mat1.at<float>(1, 2) += mat.at<float>(2, 3) * (imgDst.rows / 2.0);

		//	cv::Mat v(4, 1, CV_32F);
		cv::Mat v(3, 1, CV_32F);
		for (int y = 0; y < imgSrc.rows; y++) {
			for (int x = 0; x < imgSrc.cols; x++) {
				v.at<float>(0, 0) = (float)x;
				v.at<float>(1, 0) = (float)y;
				//			v.at<float>(2, 0) = 0.0f;
				//			v.at<float>(3, 0) = 1.0f;
				v.at<float>(2, 0) = 1.0f;
				cv::Mat p = mat1 * v;
				//		std::cout << "(" << x << ", " << y << ")\n" << p << std::endl;
				int xx = (int)p.at<float>(0, 0) / p.at<float>(2, 0);// +imgDst.cols / 2;
				int yy = (int)p.at<float>(1, 0) / p.at<float>(2, 0);// + imgDst.rows/2;
				if (xx >= 0 && xx < imgDst2.cols && yy >= 0 && yy < imgDst2.rows) {
					imgDst2.at<cv::Vec3b>(yy, xx) = imgSrc.at<cv::Vec3b>(y, x);
				}
			}
		}
		cv::imshow("dst2", imgDst2);

#if 1
		//	mat = mat.inv();
		//	mat = mat.t();

		cv::Mat mat2(3, 3, CV_32F);
		mat2.at<float>(0, 0) = mat1.at<float>(1, 1)*mat1.at<float>(2, 2) - mat1.at<float>(1, 2)*mat1.at<float>(2, 1);
		mat2.at<float>(0, 1) = mat1.at<float>(0, 2)*mat1.at<float>(2, 1) - mat1.at<float>(0, 1)*mat1.at<float>(2, 2);
		mat2.at<float>(0, 2) = mat1.at<float>(0, 1)*mat1.at<float>(1, 2) - mat1.at<float>(0, 2)*mat1.at<float>(1, 1);

		mat2.at<float>(1, 0) = mat1.at<float>(1, 2)*mat1.at<float>(2, 0) - mat1.at<float>(1, 0)*mat1.at<float>(2, 2);
		mat2.at<float>(1, 1) = mat1.at<float>(0, 0)*mat1.at<float>(2, 2) - mat1.at<float>(0, 2)*mat1.at<float>(2, 0);
		mat2.at<float>(1, 2) = mat1.at<float>(0, 2)*mat1.at<float>(1, 0) - mat1.at<float>(0, 0)*mat1.at<float>(1, 2);

		mat2.at<float>(2, 0) = mat1.at<float>(1, 0)*mat1.at<float>(2, 1) - mat1.at<float>(1, 1)*mat1.at<float>(2, 0);
		mat2.at<float>(2, 1) = mat1.at<float>(0, 1)*mat1.at<float>(2, 0) - mat1.at<float>(0, 0)*mat1.at<float>(2, 1);
		mat2.at<float>(2, 2) = mat1.at<float>(0, 0)*mat1.at<float>(1, 1) - mat1.at<float>(0, 1)*mat1.at<float>(1, 0);

		FILE* fp = fopen("mat.txt", "w");
		for (int y = 0; y < 3; y++) {
			for (int x = 0; x < 3; x++) {
				float f = mat2.at<float>(y, x);
				unsigned long u = *(unsigned long *)&f;
				fprintf(fp, "%08x\n", u);
			}
		}
		fclose(fp);


		fp = fopen("xy.txt", "w");
		for (int y = 0; y < imgDst.rows; y++) {
			for (int x = 0; x < imgDst.cols; x++) {
				v.at<float>(0, 0) = (float)x;
				v.at<float>(1, 0) = (float)y;
				v.at<float>(2, 0) = 1.0f;
				cv::Mat p = mat2 * v;
				int xx = (int)p.at<float>(0, 0) / p.at<float>(2, 0);
				int yy = (int)p.at<float>(1, 0) / p.at<float>(2, 0);
				if (xx >= 0 && xx < imgSrc.cols && yy >= 0 && yy < imgSrc.rows) {
					imgDst.at<cv::Vec3b>(y, x) = imgSrc.at<cv::Vec3b>(yy, xx);
					fprintf(fp, "0, ");
				}
				else {
					fprintf(fp, "1, ");
				}
				fprintf(fp, "%d %d (%g %g %g : %g)", xx, yy, p.at<float>(0, 0), p.at<float>(1, 0), p.at<float>(2, 0), 1.0/p.at<float>(2, 0));
				fprintf(fp, "%g %g %g %g %g %g\n",
					v.at<float>(0, 0) * mat2.at<float>(0, 0),
					v.at<float>(1, 0) * mat2.at<float>(0, 1),
					v.at<float>(0, 0) * mat2.at<float>(1, 0),
					v.at<float>(1, 0) * mat2.at<float>(1, 1),
					v.at<float>(0, 0) * mat2.at<float>(2, 0),
					v.at<float>(1, 0) * mat2.at<float>(2, 1));

			}
		}
		fclose(fp);

		cv::imshow("dst", imgDst);
#endif

		cv::imshow("src", imgSrc);

//		cv::waitKey(100);
		
	}
	cv::waitKey();
}



// end of file
