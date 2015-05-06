
#include <stdio.h>
#include "opencv2/opencv.hpp"
#include "Render.h"


// �����_�����O
template <typename T>
class MyRender : public Render<T>
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
		float	z = param.vector.at<T>(2, 0); 

		// �[�x�e�X�g
		if ( z < m_DepthBuffer.at<T>(pt) ) {
			return;
		}

		m_ImageBuffer.at<cv::Vec3b>(pt)[0] = (uchar)param.color[0];
		m_ImageBuffer.at<cv::Vec3b>(pt)[1] = (uchar)param.color[1];
		m_ImageBuffer.at<cv::Vec3b>(pt)[2] = (uchar)param.color[2];

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

//		int uu = (int)((m_Textures[0].cols-1) * u + 0.5);
//		int vv = (int)((m_Textures[0].rows-1) * v + 0.5);
		int uu = (int)((m_Textures[0].cols-1) * u);
		int vv = (int)((m_Textures[0].rows-1) * v);
		m_ImageBuffer.at<cv::Vec3b>(pt) = m_Textures[0].at<cv::Vec3b>(vv, uu);
	}
};


// �`��e�X�g
template <typename T>
void RenderTest(void)
{
	// �����_�[����
	MyRender<T>	r;

	// �e�N�X�`���ݒ�
//	cv::Mat imgTexture = cv::imread("checker.png");
	cv::Mat imgTexture = cv::imread("white.png");
	r.SetTexture(imgTexture);

	// ���W�ݒ�
	r.Viewport(0, 0, 640, 480);
	r.MatrixMode(MyRender<T>::PROJECTION);
	r.Perspective(20.0f, 1.0f, 0.1f, 100.0f);
	r.LookAt(cv::Vec3f(0, 5, 3), cv::Vec3f(0, 0, 0), cv::Vec3f(0, 1, 0));


	// �`��
	std::vector<MyRender<T>::ShaderParam>	vertex(4);

	vertex[0].vector = (cv::Mat_<T>(4, 1) << -1.0, -1.0, 0, 1);	// (x, y, z, 1)
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
	
	r.DrawPolygon(vertex, MyRender<T>::QUADS);
	
	// �\��
	cv::imshow("view", r.GetImage());
	cv::waitKey();
}


// ���C���֐�
int main()
{
	RenderTest<float>();
	
	return 0;
}


// end of file
