// --------------------------------------------------------------------------
//  Jelly の GPU描画用のライブラリ
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
	//   型定義
	// -----------------------------------------
public:
	typedef	std::array< std::array<T, 4>, 4>	Mat4;
	typedef	std::array<T, 4>					Vec4;
	typedef	std::array<T, 3>					Vec3;
	typedef	std::array<T, 2>					Vec2;
	struct FacePoint {
		size_t	vertex;			// 頂点インデックス		
		size_t	tex_cord;		// テクスチャ座標インデックス
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
		std::vector<PolygonRegion>	region;		// 描画範囲
		Rasterize					u;
		Rasterize					v;
		Rasterize					t;
	};


	// -----------------------------------------
	//  メソッド
	// -----------------------------------------
public:
	// コンストラクタ
	JellyGL()
	{
		m_matrix = IdentityMat4();
	}

	// デストラクタ
	~JellyGL()
	{
	}

	// 頂点バッファ設定
	void SetVertexBuffer(const std::vector<Vec3>& vertex)
	{
		m_vertex = vertex;
	}

	// テクスチャ座標バッファ設定
	void SetTexCordBuffer(const std::vector<Vec2>& tex_cord)
	{
		m_tex_cord = tex_cord;
	}

	// 面バッファ設定
	void SetFaceBuffer(const std::vector<Face>& face)
	{
		m_face = face;
	}
	

	// 描画リスト生成
	void MakeDrawList(void)
	{
		for ( auto f : m_face ) {
			Polygon	polygon;

			// 描画範囲設定
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

				// エッジの登録
				if ( m_edge_index.find(edge) == m_edge_index.end() ) {
					// 新規登録
					region.edge = m_edge.size();
					m_edge.push_back(edge);
					m_edge_index[edge] = ei.edge;
				}
				else {
					// 既に登録あり
					region.edge = m_edge_index[edge];
				}

				// 登録
				polygon.region.push_back(region);
			}


		}
	}
	

	// -----------------------------------------
	//  メンバ変数
	// -----------------------------------------
protected:
	std::vector<Vec3>			m_vertex;		// 頂点リスト
	std::vector<Vec2>			m_tex_cord;		// テクスチャ座標リスト
	std::vector<Face>			m_face;			// 描画面

	Mat4						m_matrix;		// 描画用マトリックス
	std::vector <Vec3>			m_draw_vertex;	// 頂点リスト
	
	std::vector< Edge >			m_edge;			// 辺
	std::map< Edge, size_t>		m_edge_index;	// 辺のインデックス探索用




	// -----------------------------------------
	//  行列計算補助関数
	// -----------------------------------------
public:
	// 単位行列生成
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

	// 行列乗算
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


	// 行列のベクタへの適用
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
	
	// 行列のベクタへの適用(射影)
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
