// --------------------------------------------------------------------------
//  Jelly �� GPU�`��p�̃��C�u����
//
//                                     Copyright (C) 2017 by Ryuji Fuchikami
// --------------------------------------------------------------------------



#ifndef __RYUZ__JELLY_GL__H__
#define __RYUZ__JELLY_GL__H__


#include <array>
#include <vector>
#include <map>


template <class T=float>
class JellyGL
{
	// -----------------------------------------
	//   �^��`
	// -----------------------------------------
public:
	typedef	std::array< std::array<T, 4>, 4>	Mat4;
	typedef	std::array<T, 4>					Vec4;
	typedef	std::array<T, 3>					Vec3;
	typedef	std::array<T, 2>					Vec2;
	struct FacePoint {
		size_t	vertex;			// ���_�C���f�b�N�X		
		size_t	tex_cord;		// �e�N�X�`�����W�C���f�b�N�X
	};
	typedef	std::vector<FacePoint>				Face;

protected:
	typedef	std::array<size_t, 2>				Edge;
	struct PolygonRegion {
		size_t	edge;
		bool	inverse;
	};
	struct Rasterize {
		T	dx;
		T	dy;
		T	offset;
	};

	struct Polygon {
		std::vector<PolygonRegion>	region;		// �`��͈�
		Rasterize					u;
		Rasterize					v;
		Rasterize					t;
	};


	// -----------------------------------------
	//  ���\�b�h
	// -----------------------------------------
public:
	// �R���X�g���N�^
	JellyGL()
	{
		m_matrix = IdentityMat4();
	}

	// �f�X�g���N�^
	~JellyGL()
	{
	}

	// ���_�o�b�t�@�ݒ�
	void SetVertexBuffer(const std::vector<Vec3>& vertex)
	{
		m_vertex = vertex;
	}

	// �e�N�X�`�����W�o�b�t�@�ݒ�
	void SetTexCordBuffer(const std::vector<Vec2>& tex_cord)
	{
		m_tex_cord = tex_cord;
	}

	// �ʃo�b�t�@�ݒ�
	void SetFaceBuffer(const std::vector<Face>& face)
	{
		m_face = face;
	}
	

	// �`�惊�X�g����
	void MakeDrawList(void)
	{
		for ( auto f : m_face ) {
			Polygon	polygon;

			// �`��͈͐ݒ�
			for ( size_t i = 0; i < f.size(); ++i ) {
				size_t j = (i + 1) % m_face.size();

				PolygonRegion	region;
				Edge			edge;
				if (  f[i].vertex < f[j].vertex ) {
					edge[0] = f[i].vertex;
					edge[1] = f[j].vertex;
					region.inverse = false;
				}
				else {
					edge[0] = f[i].vertex;
					edge[1] = f[j].vertex;
					region.inverse = true;
				}

				// �G�b�W�̓o�^
				if ( m_edge_index.find(edge) == m_edge_index.end() ) {
					// �V�K�o�^
					region.edge = m_edge.size();
					m_edge.push_back(edge);
					m_edge_index[edge] = ei.edge;
				}
				else {
					// ���ɓo�^����
					region.edge = m_edge_index[edge];
				}

				// �o�^
				polygon.region.push_back(region);
			}


		}
	}
	

	// -----------------------------------------
	//  �����o�ϐ�
	// -----------------------------------------
protected:
	std::vector<Vec3>			m_vertex;		// ���_���X�g
	std::vector<Vec2>			m_tex_cord;		// �e�N�X�`�����W���X�g
	std::vector<Face>			m_face;			// �`���

	Mat4						m_matrix;		// �`��p�}�g���b�N�X
	std::vector <Vec3>			m_draw_vertex;	// ���_���X�g
	
	std::vector< Edge >			m_edge;			// ��
	std::map< Edge, size_t>		m_edge_index;	// �ӂ̃C���f�b�N�X�T���p




	// -----------------------------------------
	//  �s��v�Z�⏕�֐�
	// -----------------------------------------
public:
	// �P�ʍs�񐶐�
	static	Mat4	IdentityMat4(void)
	{
		Mat4	mat;
		for ( size_t i = 0; i < mat.size(); ++i ) {
			for ( size_t j = 0; j < mat[i].size(); ++j ) {
				mat[i][j] = (i == j) ? (T)1 : (T)0;
			}
		}
		return mat;
	}

	// �s���Z
	Mat4 MulMat(const Mat4 matSrc0, const Mat4 matSrc1)
	{
		Mat4	matDst;
		for ( size_t i = 0; i < matDst.size(); i++ ) {
			for ( size_t j = 0; j < matDst[0].size(); j++ ) {
				matDst[i][j] = 0;
				for ( size_t k = 0; k < matDst[0].size(); k++ ) {
					matDst[i][j] += matSrc0[i][k] * matSrc1[k][j];
				}
			}
		}
		return matDst;
	}


	// �s��̃x�N�^�ւ̓K�p
	Vec4 MulMat(const Mat4 matSrc, const Vec4 vecSrc)
	{
		Vec4 vecDst;
		for ( size_t i = 0; i < vecDst.size(); i++ ) {
			vecDst[i] = 0;
			for ( size_t j = 0; j < matSrc[i].size(); j++ ) {
				vecDst[i] += matSrc[i][j] * vecSrc[j];
			}
		}
		return vecDst;
	}
	
	// �s��̃x�N�^�ւ̓K�p(�ˉe)
	Vec3 MulMat(const Mat4 matSrc, const Vec3 vecSrc)
	{
		Vec4	vecIn;
		Vec4	vecOut;
		Vec3	vecDst;

		vecIn[0] = vecSrc[0];
		vecIn[1] = vecSrc[1];
		vecIn[2] = vecSrc[2];
		vecIn[3] = 1.0f;
		vecOut = MulMat(matSrc, vecIn);	
		vecDst[0] = vecDst[0] / vecDst[3];
		vecDst[1] = vecDst[1] / vecDst[3];
		vecDst[2] = vecDst[2] / vecDst[3];

		return vecDst;
	}
};


#endif	// __RYUZ__JELLY_GL__H__


// end of file
