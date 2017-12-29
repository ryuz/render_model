

#ifndef __RYUZ__JELLY_GL__H__
#define __RYUZ__JELLY_GL__H__


#include <array>
#include <vector>


template <class T=float>
class JellyGL
{
public:
	// 型定義
	typedef	std::array< std::array<T, 4>, 4>	Mat4;
	typedef	std::array<T, 4>					Vec4;
	typedef	std::array<T, 3>					Vec3;
	typedef	std::array<T, 2>					Vec2;

	struct FacePoint {
		int	vertex;			// 頂点インデックス		
		int	tex_cord;		// テクスチャ座標インデックス
	};

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
	void SetFaceBuffer(const std::vector< std::vector<FacePoint> >& face)
	{
		m_face = face;
	}
	

	void MakeDrawList(void);

	static	Mat4	IdentityMat4(void)
	{
		Mat4	mat;
		for ( size_t i = 0; i < mat.size(); i++ ) {
			for ( int j = 0; j < mat[i].size(); j++ ) {
				mat[i][j] = (i == j) ? (T)1 : (T)0;
			}
		}
		return mat;
	}

	static	Mat4	MulMat(const Mat4 matSrc0, const Mat4 matSrc1);
	static	Vec4	MulMat(const Mat4 matSrc, const Vec4 vec4Src);
	static	Vec3	MulMat(const Mat4 matSrc, const Vec3 vec3Src);

protected:
	std::vector<Vec3>						m_vertex;		// 頂点リスト
	std::vector<Vec2>						m_tex_cord;		// テクスチャ座標リスト
	std::vector< std::vector<FacePoint> >	m_face;			// 描画面

	float									m_matrix[4][4];
	std::vector <Vec3>						m_draw_vertex;	// 頂点リスト
	
	std::vector< std::array<int, 2> >		m_edge;			// 描画面
};


#endif	// __RYUZ__JELLY_GL__H__


// end of file
