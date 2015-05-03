

#ifndef __RYUZ__RENDER_H__
#define __RYUZ__RENDER_H__

#include <assert.h>
#include <vector>
#include "opencv2/opencv.hpp"


template <typename T>
class Render
{
public:
	// �|���S���̕`�惂�[�h
	enum PolygonMode {
		TRIANGLES,
		TRIANGLE_STRIP,
		TRIANGLE_FAN,
		QUADS,
		QUAD_STRIP,
	};

	// �V�F�[�_�[�p�����[�^
	struct	ShaderParam {
		cv::Mat_<T>		vector;
		std::vector<T>	color;
		std::vector<T>	coordinate;
	};
	
	// �R���X�g���N�^
	Render()
	{
		m_ImageType     = CV_8UC3;
		m_DepthType     = cv::DataType<T>::type;
		m_PolygonMode   = TRIANGLES;
		m_CullingEnable = false;
		LoadIdentity();
		SetSize(640, 480);
		SetTextureNum(1);
	}
	
	// �f�X�g���N�^
	virtual	~Render()	{}
	
	// �摜�T�C�Y�ݒ�
	void SetSize(int width, int height)
	{
		m_Width  = width;
		m_Height = height;
		ClearBuffer();
	}

	// �o�b�t�@�N���A
	void ClearBuffer()
	{
		m_ImageBuffer = cv::Mat::zeros(m_Height, m_Width, m_ImageType);
		m_DepthBuffer = cv::Mat::zeros(m_Height, m_Width, m_DepthType);
	}

	void SetTextureNum(int n)				{ m_Textures.resize(n); }
	void SetTexture(cv::Mat img, int n=0)	{ m_Textures[n] = img; }

	// �o�b�t�@�擾
	cv::Mat&	GetImage(void)			{ return m_ImageBuffer; }
	cv::Mat&	GetDepth(void)			{ return m_DepthBuffer; }
	cv::Mat&	GetTexture(int n=0)		{ return m_Textures[n]; }
	
	// �}�g���b�N�X�ݒ�
	void		LoadIdentity(void)			{ m_Matrix = cv::Mat_<T>::eye(4, 4); }
	void		LoadMatrix(cv::Mat_<T> mat)	{ m_Matrix = mat; }
	void		MultMatrix(cv::Mat_<T> mat)	{ m_Matrix = m_Matrix * m_mat; }
	
	// �ˉe�ϊ��ݒ�(gluPerspective���ǂ�)
	void Perspective(T fovy, T aspect, T znear, T zfar)
	{
		T	r = fovy * (T)((3.1415926535897932384626/180.0) / 2.0);
		T	t = (T)1.0 / (T)tan(r);
		
		cv::Mat_(T)	mat = (cv::Mat_<T(3,3) <<
				t/aspect, 0,                                   0,  0,
				0,        t,                                   0,  0,
				0,        0,     (zfar + znear) / (znear - zfar), -1,
				0,        0, (2 * zfar * znear) / (znear - zfar),  0);

		MultMatrix(mat);
	}

	// ���s�ړ�(glTranslate ���ǂ�)
	void Translated(cv::Vec<T, 3> v)
	{
		cv::Mat_(T)	mat = (cv::Mat_<T(3,3) <<
				1, 0, 0, v[0],
				0, 1, 0, v[1],
				0, 0, 1, v[2],
				0, 0, 0,    1);

		MultMatrix(mat);
	}

	// ���_�ݒ�(gluLookAt ���ǂ�)
	void LookAt(cv::Vec<T, 3> eye, cv::Vec<T, 3> center, cv::Vec<T, 3> up)
	{
		cv::Vec<T, 3>	f = center - eye;
		cv::normalize(f);

		cv::normalize(up);

		cv::Vec<T, 3>	s  = f.cross(up);
		
		cv::Vec<T, 3>	ss = s;
		normalize(ss);
		cv::Vec<T, 3>	u  = ss.cross(f);
		
		cv::Mat_(T)	mat = (cv::Mat_<T(3,3) <<
				  s[0],  s[1],  s[2], 0,
				  u[0],  u[1],  u[2], 0,
				 -f[0], -f[1], -f[2], 0,
				     0,     0,     0, 1);

		MultMatrix(mat);

		Translated(-eye);
	};
	
	// �|���S���`��
	void DrawPolygon(std::vector<ShaderParam> vertexes)
	{
		ShaderParam		v[3];
		unsigned int	p = 0;
		unsigned int	n = vertexes.size();
		
		if ( n < 3 ) {
			return;
		}

		switch ( m_PolygonMode ) {
		case TRIANGLES:
			while ( n - p > 3 ) {
				v[0] = vertexes[p+0];
				v[1] = vertexes[p+1];
				v[2] = vertexes[p+2];
				DrawTriangle(v);
				p += 3;
			}
			break;
		
		case TRIANGLE_STRIP:
			while ( n - p > 3 ) {
				v[0] = vertexes[p+0];
				v[1] = vertexes[p+1];
				v[2] = vertexes[p+2];
				DrawTriangle(v);
				p++;
			}
			break;

		case TRIANGLE_FAN:
			v[0] = vertexes[p++];
			while ( n - p > 2 ) {
				v[1] = vertexes[p+0];
				v[2] = vertexes[p+1];
				DrawTriangle(v);
				p++;
			}
			break;

		case QUADS:
			while ( n - p > 4 ) {
				v[0] = vertexes[p+0];
				v[1] = vertexes[p+1];
				v[2] = vertexes[p+2];
				DrawTriangle(v);
				v[0] = vertexes[p+0];
				v[1] = vertexes[p+2];
				v[2] = vertexes[p+3];
				DrawTriangle(v);
				p += 4;
			}
			break;

		case QUAD_STRIP:
			while ( n - p > 4 ) {
				v[0] = vertexes[p+0];
				v[1] = vertexes[p+1];
				v[2] = vertexes[p+2];
				DrawTriangle(v);
				v[0] = vertexes[p+1];
				v[1] = vertexes[p+3];
				v[2] = vertexes[p+2];
				DrawTriangle(v);
				p += 2;
			}
			break;
		}
	}
	
	// �O�p�`�`��
	void DrawTriangle(ShaderParam vertex[3]) {
		// �p�����[�^���擾
		m_ColorSize      = vertex[0].color.size();
		m_CoordinateSize = vertex[0].coordinate.size();;
		
		assert(vertex[1].color.size() == m_ColorSize);
		assert(vertex[2].color.size() == m_ColorSize);
		assert(vertex[1].coordinate.size() == m_CoordinateSize);
		assert(vertex[2].coordinate.size() == m_CoordinateSize);

		cv::Point_<T>	v[3];
		cv::Mat_<T>		z[3];
		for ( int i = 0; i < 3; i ++ ) {
			// �f�[�^�R�s�[
			ShaderParam param = vertex[i];
			
			// ���_�V�F�[�_�[
			VertexShader(param);

			// ���W�ݒ�
			T	w  = param.vector.at<T>(3, 0);
			v[i].x = param.vector.at<T>(0, 0) / w;
			v[i].y = param.vector.at<T>(1, 0) / w;
			
			// Z�p�����[�^�ݒ�
			z[i] = cv::Mat_<T>(2+m_ColorSize+m_CoordinateSize, 1);
			int p = 0;
			z[i].at<T>(p++, 0) = param.vector.at<T>(2, 0) / w;	// z
			z[i].at<T>(p++, 0) = (T)1.0 / w;					// 1/w

			// �F�p�����[�^�ݒ�
			for ( unsigned j = 0; j < m_ColorSize; j++ ) {
				z[i].at<T>(p++, 0) = param.color[j];
			}

			// ���W�p�����[�^�ݒ�(�p�[�X�y�N�e�B�u�␳)
			for ( unsigned j = 0; j < m_CoordinateSize; j++ ) {
				z[i].at<T>(p++, 0) = param.coordinate[j] / param.vector.at<T>(3, 0);
			}
		}
		Rasteriser(v, z);
	}


protected:
	// �ő�/�ŏ�
	T	Min(T a, T b)	{ return a < b ? a : b; }
	T	Min(T a, T b, T c) { return Min(Min(a, b), c); }
	T	Min(T a, T b, T c, T d) { return Min(Min(a, b), Min(c, d)); }
	T	Max(T a, T b)	{ return a > b ? a : b; }
	T	Max(T a, T b, T c) { return Max(Max(a, b), c); }
	T	Max(T a, T b, T c, T d) { return Max(Max(a, b), Max(c, d)); }
	
	// ���X�^���C�U
	void Rasteriser(cv::Point_<T> v[3], cv::Mat_<T> z[3])
	{
		// ��]�����`�F�b�N
		cv::Vec<T, 2>	v0 = v[1] - v[0];
		cv::Vec<T, 2>	v1 = v[2] - v[0];
		if ( v0[1]*v1[0] - v0[0]*v1[1] < 0 ) {
			if ( m_CullingEnable ) {
				return;
			}

			// �v�f����ւ�
			cv::Point_<T>	tmp_v = v[1];
			cv::Mat_<T>		tmp_z = z[1];
			v[1] = v[2];
			z[1] = z[2];
			v[2] = tmp_v;
			z[2] = tmp_z;
		}
		
		// �͈͎Z�o
		int	x0 = (int)(Min(v[0].x, v[1].x, v[2].x, (T)(m_Width-1)) + (T)0.5);
		int	y0 = (int)(Min(v[0].y, v[1].y, v[2].y, (T)(m_Height-1)) + (T)0.5);
		int	x1 = (int)(Max(v[0].x, v[1].x, v[2].x, (T)0) + (T)0.5);
		int	y1 = (int)(Max(v[0].y, v[1].y, v[2].y, (T)0) + (T)0.5);
		
		// �̈�����莮�쐬
		cv::Vec<T, 3>	e0;
		cv::Vec<T, 3>	edx;
		cv::Vec<T, 3>	edy;
		for ( int i = 0; i < 3; i++ ) {
			int j = (i+1) % 3;
			edx[i] = v[j].x - v[i].x;
			edy[i] = v[j].y - v[i].y;
			e0[i]  = ((T)x0 - v[i].x) * edy[i] - ((T)y0 - v[i].y) * edx[i];
		}
		
		// �L�q���y�ɂ��邽�߂�XY��Mat��
		cv::Mat_<T> x[3];
		cv::Mat_<T> y[3];
		for ( int i = 0; i < 3; i++ ) {
			x[i] = cv::Mat_<T>::zeros(z[i].size()) + v[i].x;
			y[i] = cv::Mat_<T>::zeros(z[i].size()) + v[i].y;
		}
		
		// ���ʂ̎�(a*x + b*y + c*z + d = 0)�̌W���v�Z(�O��)
		cv::Mat_<T>	v0x = x[1] - x[0];
		cv::Mat_<T>	v0y = y[1] - y[0]; 
		cv::Mat_<T>	v0z = z[1] - z[0];
		cv::Mat_<T>	v1x = x[2] - x[0]; 
		cv::Mat_<T>	v1y = y[2] - y[0];
		cv::Mat_<T>	v1z = z[2] - z[0];
		cv::Mat_<T>	a = v0y.mul(v1z) - v0z.mul(v1y);
		cv::Mat_<T>	b = v0z.mul(v1x) - v0x.mul(v1z);
		cv::Mat_<T>	c = v0x.mul(v1y) - v0y.mul(v1x);

		// �O�[���[�v�Z�p�W������
		cv::Mat_<T>	zdx = -a.mul(1/c);
		cv::Mat_<T>	zdy = -b.mul(1/c);
		cv::Mat_<T>	dd  = z[0] - zdx.mul(x[0]) - zdy.mul(y[0]);
		cv::Mat_<T>	z0  = (zdx * x0) + (zdy * y0) + dd;

		// ���X�^���C�Y
		cv::Vec<T, 3>	ey = e0;
		cv::Mat_<T>		zy = z0;
		cv::Point		pt;
		for ( pt.y = y0; pt.y <= y1; pt.y++ ) {
			cv::Vec<T, 3>	ex = ey;
			cv::Mat_<T>		zx = zy.clone();
			for ( pt.x = x0; pt.x <= x1; pt.x++ ) {
				if ( ex[0] >= 0 && ex[1] >= 0 && ex[2] >= 0 ) {
					PixelProc(pt, zx);
				}
				ex += edy;
				zx += zdx;
			}
			ey -= edx;
			zy += zdy;
		}
	}

	// �s�N�Z������
	void PixelProc(cv::Point pt, cv::Mat_<T> z)
	{
		ShaderParam param;
		int			p = 0;
		
		// ���W�ݒ�
		param.vector = cv::Mat_<T>(4, 1);
		param.vector.at<T>(0, 0) = (T)pt.x;
		param.vector.at<T>(1, 0) = (T)pt.y;
		param.vector.at<T>(2, 0) = z.at<T>(p++, 0);
		param.vector.at<T>(3, 0) = (T)1.0;
		T	w                    = z.at<T>(p++, 0);
		
		// �F���
		param.color.resize(m_ColorSize);
		for ( unsigned int i = 0; i < m_ColorSize; i++ ) {
			param.color[i] = z.at<T>(p++, 0);
		}

		// ���W���(�p�[�X�y�N�e�B�u�␳)
		param.coordinate.resize(m_CoordinateSize);
		for ( unsigned int i = 0; i < m_CoordinateSize; i++ ) {
			param.coordinate[i] = z.at<T>(p++, 0) / w;
		}

		// �s�N�Z���V�F�[�_�[
		PixelShader(param);
	}
	
	// �o�[�e�b�N�X�V�F�[�_�[
	virtual void VertexShader(ShaderParam& param)
	{
		param.vector = m_Matrix * param.vector;
	}

	// �s�N�Z���V�F�[�_�[
	virtual void PixelShader(ShaderParam& param)
	{
	}
	
	// �����o�[�ϐ�
	int						m_Width;
	int						m_Height;
	int						m_ImageType;		// �摜�o�b�t�@�^�C�v
	cv::Mat					m_ImageBuffer;		// �摜�o�b�t�@
	int						m_DepthType;		// �[�x�o�b�t�@�^�C�v
	cv::Mat					m_DepthBuffer;		// �[�x�o�b�t�@	
	std::vector<cv::Mat>	m_Textures;			// �e�N�X�`��

	PolygonMode				m_PolygonMode;		// �|���S���`�惂�[�h
	bool					m_CullingEnable;	// �\���J�����O
	cv::Mat					m_Matrix;			// �ϊ��s��
	unsigned int			m_ColorSize;		// �F�p�����[�^��
	unsigned int			m_CoordinateSize;	// ���W�p�����[�^��
};


#endif	// __RYUZ__RENDER_H__


// end of file
