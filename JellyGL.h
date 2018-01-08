// --------------------------------------------------------------------------
//  Jelly �� GPU�`��p�̃��C�u����
//
//                                     Copyright (C) 2017 by Ryuji Fuchikami
// --------------------------------------------------------------------------



#ifndef __RYUZ__JELLY_GL__H__
#define __RYUZ__JELLY_GL__H__


#include <stdint.h>
#include <array>
#include <vector>
#include <map>


template <class T=float, class TI=int64_t, int Q=10>
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
		FacePoint() {}
		FacePoint(size_t v, size_t t) { vertex = v; tex_cord = t; }
	};
	typedef	std::vector<FacePoint>				Face;

	struct PixelParam {
		T		u;
		T		v;
		T		t;
	};

protected:
	typedef	std::array<size_t, 2>				Edge;
	struct PolygonRegion {
		size_t	edge;
		bool	inverse;
	};

	struct Polygon {
		std::vector<PolygonRegion>	region;			// �`��͈�
		size_t						vertex[3];		// ���_�C���f�b�N�X		
		size_t						tex_cord[3];	// �e�N�X�`�����W�C���f�b�N�X
	};
	
	struct RasterizeParam {
		TI	dx;
		TI	dy;
		TI	offset;

		TI calc(int x, int y) const { return dx*(TI)x + dy*(TI)y + offset; }
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
	
	void SetMatrix(const Mat4 mat)
	{
		m_matrix = mat;
	}

	// �`�惊�X�g����
	void MakeDrawList(void)
	{
		for ( auto f : m_face ) {
			Polygon	polygon;

			// �`��͈͐ݒ�
			for ( size_t i = 0; i < f.size(); ++i ) {
				size_t j = (i + 1) % f.size();

				PolygonRegion	region;
				Edge			edge;
				if (  f[i].vertex < f[j].vertex ) {
					edge[0] = f[i].vertex;
					edge[1] = f[j].vertex;
					region.inverse = false;
				}
				else {
					edge[0] = f[j].vertex;
					edge[1] = f[i].vertex;
					region.inverse = true;
				}

				// �G�b�W�̓o�^
				if ( m_edge_index.find(edge) == m_edge_index.end() ) {
					// �V�K�o�^
					region.edge = m_edge.size();
					m_edge.push_back(edge);
					m_edge_index[edge] = region.edge;
				}
				else {
					// ���ɓo�^����
					region.edge = m_edge_index[edge];
				}

				// �o�^
				polygon.region.push_back(region);
			}

			FacePoint	fp[3] = {f[0], f[1], f[f.size() - 1]};
			for ( int i = 0; i < 3; ++i ) {
				polygon.vertex[i]   = fp[i].vertex;
				polygon.tex_cord[i] = fp[i].tex_cord;
			}

#if 0
				param.x[i] = m_vertex[fp[i].vertex][0]; 
				param.y[i] = m_vertex[fp[i].vertex][1];
				u[i] = 1 / m_tex_cord[fp[i].tex_cord][0];
				v[i] = 1 / m_tex_cord[fp[i].tex_cord][1];
				w[i] = 1 / m_vertex[fp0.vertex][2],     1/m_vertex[fp1.vertex][0],     1/m_vertex[fp2.vertex][0]};
			}

			Vec3 w = { 1/m_vertex[fp0.vertex][2],     1/m_vertex[fp1.vertex][0],     1/m_vertex[fp2.vertex][0]};
			Vec3 u = { 1/m_tex_cord[fp0.tex_cord][0], 1/m_tex_cord[fp1.tex_cord][0], 1/m_tex_cord[fp2.tex_cord][0]};
			Vec3 v = { 1/m_tex_cord[fp0.tex_cord][1], 1/m_tex_cord[fp1.tex_cord][1], 1/m_tex_cord[fp2.tex_cord][1]};
	//		std::vector<Vec3> param;
			polygon.param.push_back(w);
			polygon.param.push_back(u);
			polygon.param.push_back(v);

			/*
			Vec3 vertex0 = m_vertex[fp0.vertex];
			Vec3 vertex1 = m_vertex[fp1.vertex];
			Vec3 vertex2 = m_vertex[fp2.vertex];
			for ( auto param : param ) {
				// �O�όv�Z
				Vec3	vector0 = {vertex1[0] - vertex0[0], vertex1[1] - vertex0[1], param[1] - param[1]};
				Vec3	vector1 = {vertex2[0] - vertex0[0], vertex2[1] - vertex0[1], param[2] - param[1]};
				Vec3	cross   = CrossVec3(vector0, vector1);
				T	dx     = -cross[0] / cross[2];
				T	dy     = -cross[1] / cross[2];
				T	offset = (cross[0]*vertex0[0] + cross[1]*vertex0[1] + cross[2]*param[0]) / cross[2];

				RasterizeParam	rp;
				rp.dx     = (TI)(dx / (1 << Q));
				rp.dy     = (TI)(dy / (1 << Q));
				rp.offset = (TI)(offset / (1 << Q));
				polygon.param.push_back(rp);
			}
			*/
#endif

			m_polygon.push_back(polygon);
		}
	}
	

	void Draw(int width, int height, void (*proc)(int x, int y, bool polygon, PixelParam pp, void* user), void* user=0)
	{
		// ���_���W�v�Z
		m_draw_vertex.clear();
		for ( const auto& v : m_vertex ) {
			m_draw_vertex.push_back(MulMat(m_matrix, v));
		}

		// ���X�^���C�U�p�����[�^����
		m_rasterizeEdge.clear();
		for ( auto edge : m_edge ) {
			m_rasterizeEdge.push_back(EdgeToRasterizeParam( m_draw_vertex[edge[0]], m_draw_vertex[edge[1]]));
		}
		
		m_rasterizeParam.clear();
		for ( auto p : m_polygon ) {
			Vec3 param[3];
			for ( int i = 0; i < 3; ++i ) {
				param[0][i] = 1 / m_draw_vertex[p.vertex[i]][2];	// w
				param[1][i] = 1 / m_tex_cord[p.tex_cord[i]][0];		// u
				param[2][i] = 1 / m_tex_cord[p.tex_cord[i]][1];		// v
			}

			std::vector<RasterizeParam>	rp;
			for ( int i = 0; i < 3; ++i ) {
				Vec3 vertex[3];
				for ( int j = 0; j < 3; ++j ) {
					vertex[j][0] =  m_draw_vertex[p.vertex[j]][0];	// x
					vertex[j][1] =  m_draw_vertex[p.vertex[j]][0];	// y
					vertex[j][2] =  param[i][j];					// param
				}
				rp.push_back(ParamToRasterizeParam(vertex));
			}
			m_rasterizeParam.push_back(rp);
		}
		
		std::vector<bool>	edge;
		for ( int y = 0; y < height; ++y ) {
			for ( int x = 0; x < width; ++x ) {

#if 0
				PixelParam pp = {};
				bool v = (m_rasterizeEdge[0].calc(x, y) >= 0
						&& m_rasterizeEdge[1].calc(x, y) >= 0
						&& !(m_rasterizeEdge[2].calc(x, y) >= 0)
						&& !(m_rasterizeEdge[3].calc(x, y) >= 0));
				proc(x, y, v, pp, user);
#else
				edge.clear();
				for ( const auto& e : m_rasterizeEdge ) {
					edge.push_back(e.calc(x, y) >= 0);
				}

				for ( auto& p : m_polygon ) {
					bool v = true;
					for ( auto& r : p.region ) {
						if ( r.inverse ? edge[r.edge] : !edge[r.edge] ) {
							v = false; 
						}
					}

					PixelParam pp = {};
					proc(x, y, v, pp, user);
				}
#endif
			}
		}
	}


protected:
	// �G�b�W����p�����[�^�Z�o
	RasterizeParam	EdgeToRasterizeParam(Vec3 v0, Vec3 v1)
	{
		RasterizeParam	rp;
		
		rp.dx     = (TI)(v1[1] - v0[1]);
		rp.dy     = (TI)(v0[0] - v1[0]);
		rp.offset = (TI)-((v0[1] * rp.dy) + (v0[0] * rp.dx));

		return rp;
	}

	RasterizeParam	ParamToRasterizeParam(Vec3 vertex[3])
	{
//		Vec3	vector0 = {v1[0] - v0[0], v1[1] - v0[1], v1[2] - param[1]};
//		Vec3	vector1 = {v2[0] - v0[0], v2[1] - v0[1], v1[2] - param[1]};
		Vec3	vector0 = SubVec3(vertex[1], vertex[0]);
		Vec3	vector1 = SubVec3(vertex[2], vertex[0]);
		Vec3	cross   = CrossVec3(vector0, vector1);
		T		dx     = -cross[0] / cross[2];
		T		dy     = -cross[1] / cross[2];
		T		offset = (cross[0]*vertex[0][0] + cross[1]*vertex[0][1] + cross[2]*vertex[0][2]) / cross[2];

		RasterizeParam	rp;
		rp.dx     = (TI)(dx / (1 << Q));
		rp.dy     = (TI)(dy / (1 << Q));
		rp.offset = (TI)(offset / (1 << Q));
		return rp;
	}
	
	// -----------------------------------------
	//  �����o�ϐ�
	// -----------------------------------------
protected:
	std::vector<Vec3>			m_vertex;			// ���_���X�g
	std::vector<Vec2>			m_tex_cord;			// �e�N�X�`�����W���X�g
	std::vector<Face>			m_face;				// �`���
	std::vector<Polygon>		m_polygon;			// �|���S���f�[�^

	Mat4						m_matrix;			// �`��p�}�g���b�N�X
	std::vector <Vec3>			m_draw_vertex;		// ���_���X�g
	
	std::vector<Edge>			m_edge;				// ��
	std::map< Edge, size_t>		m_edge_index;		// �ӂ̃C���f�b�N�X�T���p

	std::vector<RasterizeParam>					m_rasterizeEdge;	// ��
	std::vector< std::vector<RasterizeParam> >	m_rasterizeParam;		// �p�����[�^


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
		vecDst[0] = vecOut[0] / vecOut[3];
		vecDst[1] = vecOut[1] / vecOut[3];
		vecDst[2] = vecOut[2] / vecOut[3];

		return vecDst;
	}
	
	// �x�N�g���̌��Z
	Vec3 SubVec3(const Vec3 vec0, const Vec3 vec1) {
		Vec3 v;
		for ( size_t i = 0; i < v.size(); i++ ) {
			v[i] = vec0[i] - vec1[i];
		}
		return v;
	}

	// �x�N�g���̊O��
	Vec3 CrossVec3(const Vec3 vec0, const Vec3 vec1)
	{
		Vec3	vecCross;
		vecCross[0] = vec0[1]*vec1[2] - vec0[2]*vec1[1];
		vecCross[1] = vec0[2]*vec1[0] - vec0[0]*vec1[2];
		vecCross[2] = vec0[0]*vec1[1] - vec0[1]*vec1[0];
		return vecCross;
	}
};


#endif	// __RYUZ__JELLY_GL__H__


// end of file
