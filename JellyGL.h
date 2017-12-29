

#ifndef __RYUZ__JELLY_GL__H__
#define __RYUZ__JELLY_GL__H__


#include <array>
#include <vector>


template <class T=float>
class JellyGL
{
public:
	// �^��`
	typedef	std::array< std::array<T, 4>, 4>	Mat4;
	typedef	std::array<T, 4>					Vec4;
	typedef	std::array<T, 3>					Vec3;
	typedef	std::array<T, 2>					Vec2;

	struct FacePoint {
		int	vertex;			// ���_�C���f�b�N�X		
		int	tex_cord;		// �e�N�X�`�����W�C���f�b�N�X
	};

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
	std::vector<Vec3>						m_vertex;		// ���_���X�g
	std::vector<Vec2>						m_tex_cord;		// �e�N�X�`�����W���X�g
	std::vector< std::vector<FacePoint> >	m_face;			// �`���

	float									m_matrix[4][4];
	std::vector <Vec3>						m_draw_vertex;	// ���_���X�g
	
	std::vector< std::array<int, 2> >		m_edge;			// �`���
};


#endif	// __RYUZ__JELLY_GL__H__


// end of file
