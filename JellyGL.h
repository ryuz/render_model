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


template <class T=float, class TI=int32_t, bool perspective_correction=true>
class JellyGL
{
	// -----------------------------------------
	//   �^��`
	// -----------------------------------------
public:
	typedef	std::array< std::array<T, 4>, 4>	Mat4;					// �s��4x4
	typedef	std::array<T, 4>					Vec4;					// �x�N�^x4
	typedef	std::array<T, 3>					Vec3;					// �x�N�^x3
	typedef	std::array<T, 2>					Vec2;					// �x�N�^x2
	typedef	std::array<TI, 3>					RasterizerParameter;	// ���X�^���C�U�p�����[�^

	// �ʒ�`���̒��_���
	struct FacePoint {
		size_t	vertex;			// ���_�C���f�b�N�X		
		size_t	tex_cord;		// �e�N�X�`�����W�C���f�b�N�X
		Vec3	color;			// �F
		FacePoint() {}
		FacePoint(size_t v, size_t t)         { vertex = v; tex_cord = t; }
		FacePoint(size_t v, size_t t, Vec3 c) { vertex = v; tex_cord = t; color = c; }
		FacePoint(size_t v, Vec3 c)           { vertex = v; color = c; }
	};
	
	// �ʏ��
	struct Face {
		int						matrial;
		std::vector<FacePoint>	points;
	};

	// �|���S���̈�
	struct PolygonRegion {
		size_t	edge;
		bool	inverse;
	};

	// �s�N�Z�����
	struct PixelParam {
		int		matrial;
		Vec3	tex_cord;
		Vec3	color;
	};

protected:
	typedef	std::array<size_t, 2>	Edge;			// �G�b�W���

	// �|���S�����
	struct Polygon {
		int							matrial;
		std::vector<PolygonRegion>	region;			// �`��͈�
		size_t						vertex[3];		// ���_�C���f�b�N�X		
		size_t						tex_cord[3];	// �e�N�X�`�����W�C���f�b�N�X
		Vec3						color[3];		// �F		
	};

	// ���X�^���C�Y�W��
	struct RasterizeCoeff {
		TI		dx;
		TI		dy;
		TI		c;

		// ���X�^���C�U�p�����[�^�ɕϊ�
		RasterizerParameter	GetRasterizerParameter(int width)
		{
			return {dx, dy - (dx * (TI)(width - 1)), c};
		}
	};
	
	// �v�Z���j�b�g(�G�b�W����ƃp�����[�^�⊮�ŋ��ʉ�)
	class RasterizerUnit
	{
	public:
		RasterizerUnit(RasterizerParameter rp)
		{
			m_dx       = rp[0];
			m_y_stride = rp[1];
			m_value    = rp[2];
		}
		
		bool GetEdgeDiscriminantValue(void)
		{
			return (m_value >= 0);
		}
		
		T GetShaderParamValue(int param_q)
		{
			return (T)m_value / (T)(1 << param_q);
		}

		// �X�e�b�v�v�Z(�n�[�h�E�F�A���z��ӏ�)
		void CalcNext(bool line_end)
		{
			m_value += line_end ? m_y_stride : m_dx;
		}
	protected:
		TI		m_value;
		TI		m_dx;
		TI		m_y_stride;
	};


	// -----------------------------------------
	//  �����o�ϐ�
	// -----------------------------------------
protected:
	int							m_width          = 640;
	int							m_height         = 480;
	bool						m_culling_cw     = true;
	bool						m_culling_ccw    = true;
	
	int							m_edge_param_q   = 4;
	int							m_shader_param_q = 24;

	std::vector<Vec3>			m_vertex;			// ���_���X�g
	std::vector<int>			m_vertex_model;		// ���_�̑����郂�f���ԍ�
	std::vector<Vec2>			m_tex_cord;			// �e�N�X�`�����W���X�g
	std::vector<Face>			m_face;				// �`���
	std::vector<Polygon>		m_polygon;			// �|���S���f�[�^

	std::vector <Vec4>			m_draw_vertex;		// �`��p���_���X�g
	
	std::vector <Mat4>			m_model_matrix;		// ���f���s��
	Mat4						m_view_matrix;		// �r���[�s��
	Mat4						m_viewport_matrix;	// viewport

	std::vector<Edge>			m_edge;				// ��
	std::map< Edge, size_t>		m_edge_index;		// �ӂ̃C���f�b�N�X�T���p


	std::vector<RasterizeCoeff>					m_coeffsEdge;	// �G�b�W����p�����[�^�W��
	std::vector< std::vector<RasterizeCoeff> >	m_coeffsShader;	//  �V�F�[�_�[�p�����[�^�W��


	// H/W�G���W���ݒ�
	uint32_t		m_hw_base_addr        = 0x40000000;
	uint32_t		m_hw_shader_type      = 0;
	uint32_t		m_hw_version          = 0;
	uint32_t		m_hw_bank_step        = 0x00004000;
	uint32_t		m_hw_params_step      = 0x00001000;
	uint32_t		m_hw_bank_num         = 2;
	uint32_t		m_hw_edge_num         = 12*2;
	uint32_t		m_hw_polygon_num      = 6*2;
	uint32_t		m_hw_shader_param_num = 4;
	uint32_t		m_hw_shader_param_q   = 4;

	bool			m_hw_shader_param_has_z        = true;
	bool			m_hw_shader_param_has_tex_cord = false; 
	bool			m_hw_shader_param_has_color    = true;

	const unsigned long	REG_ADDR_CTL_ENABLE             = 0x00*4;
	const unsigned long	REG_ADDR_CTL_BANK               = 0x01*4;
	const unsigned long	REG_ADDR_PARAM_WIDTH            = 0x02*4;
	const unsigned long	REG_ADDR_PARAM_HEIGHT           = 0x03*4;
	const unsigned long	REG_ADDR_PARAM_CULLING          = 0x04*4;
	const unsigned long	REG_ADDR_PARAMS_BANK            = 0x11*4;
	const unsigned long	REG_ADDR_CFG_SHDER_TYPE         = 0x20*4;
	const unsigned long	REG_ADDR_CFG_VERSION            = 0x21*4;
	const unsigned long	REG_ADDR_CFG_BANK_ADDR_WIDTH    = 0x22*4;
	const unsigned long	REG_ADDR_CFG_PARAMS_ADDR_WIDTH  = 0x23*4;
	const unsigned long	REG_ADDR_CFG_BANK_NUM           = 0x24*4;
	const unsigned long	REG_ADDR_CFG_EDGE_NUM           = 0x25*4;
	const unsigned long	REG_ADDR_CFG_POLYGON_NUM        = 0x26*4;
	const unsigned long	REG_ADDR_CFG_SHADER_PARAM_NUM   = 0x27*4;
	const unsigned long	REG_ADDR_CFG_EDGE_PARAM_WIDTH   = 0x28*4;
	const unsigned long	REG_ADDR_CFG_SHADER_PARAM_WIDTH = 0x29*4;
	const unsigned long	REG_ADDR_CFG_REGION_PARAM_WIDTH = 0x2a*4;
	const unsigned long	REG_ADDR_CFG_SHADER_PARAM_Q     = 0x2b*4;



	// -----------------------------------------
	//  ���\�b�h
	// -----------------------------------------
public:
	// �R���X�g���N�^
	JellyGL()
	{
		m_view_matrix     = IdentityMat4();
		m_viewport_matrix = IdentityMat4();
	}

	// �f�X�g���N�^
	~JellyGL()
	{
	}

	// ���x�ݒ�
	void SetEdgeParamFracWidth(int q)	{ m_edge_param_q   = q;	}
	void SetShaderParamFracWidth(int q)	{ m_shader_param_q = q;	}

	// �T�C�Y�ݒ�
	void SetSize(int width, int height)
	{
		m_width  = width;
		m_height = height;
	}

	// �J�����O�ݒ�
	void SetCulling(bool cw, bool ccw)
	{
		m_culling_cw  = cw;
		m_culling_ccw = ccw;
	}

	// ���_�o�b�t�@�ݒ�
	void SetVertexBuffer(const std::vector<Vec3>& vertex)
	{
		m_vertex = vertex;
		m_vertex_model.resize(m_vertex.size(), 0);
		m_model_matrix.clear();
		m_model_matrix.push_back(IdentityMat4());
	}

	// ���f���ݒ�
	void SetModel(int model, int begin, int end)
	{
		while ( model >= (int)m_model_matrix.size() ) {
			m_model_matrix.push_back(IdentityMat4());
		}
		for ( int i = begin; i < end; i++ ) {
			m_vertex_model[i] = model;
		}
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
	
	// Viewport�ݒ�
	void SetViewport(int x, int y, int width, int height)
	{
		m_viewport_matrix = ViewportMat4(x,y ,width, height); 
	}

	// View�}�g���b�N�X�ݒ�
	void SetViewMatrix(Mat4 mat)
	{
		m_view_matrix = mat;
	}

	// Model�}�g���b�N�X�ݒ�
	void SetModelMatrix(int model, Mat4 mat)
	{
		while ( model >= (int)m_model_matrix.size() ) {
			m_model_matrix.push_back(IdentityMat4());
		}
		m_model_matrix[model] = mat;
	}

	// �`�惊�X�g����
	void MakeDrawList(void)
	{
		for ( auto f : m_face ) {
			Polygon	polygon;

			// �`��͈͐ݒ�
			for ( size_t i = 0; i < f.points.size(); ++i ) {
				size_t j = (i + 1) % f.points.size();

				PolygonRegion	region;
				Edge			edge;
				if (  f.points[i].vertex < f.points[j].vertex ) {
					edge[0] = f.points[i].vertex;
					edge[1] = f.points[j].vertex;
					region.inverse = false;
				}
				else {
					edge[0] = f.points[j].vertex;
					edge[1] = f.points[i].vertex;
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

			FacePoint	fp[3] = {f.points[0], f.points[1], f.points[2]};
			for ( int i = 0; i < 3; ++i ) {
				polygon.vertex[i]   = fp[i].vertex;
				polygon.tex_cord[i] = fp[i].tex_cord;
				polygon.color[i]    = fp[i].color;
			}
			polygon.matrial = f.matrial;

			m_polygon.push_back(polygon);
		}
	}
	

	// �`����{
	void DrawSetup(void)
	{
		// �}�g���b�N�X�쐬
		std::vector<Mat4> matrix;
		for ( auto& mat : m_model_matrix ) {
			matrix.push_back(MulMat(m_viewport_matrix, MulMat(m_view_matrix, mat)));
		}

		// ���_���W�v�Z
		m_draw_vertex.clear();
		for ( size_t i = 0; i < m_vertex.size(); ++i ) {
			m_draw_vertex.push_back(MulPerspectiveVec4(matrix[m_vertex_model[i]] , m_vertex[i]));
		}

		// ���X�^���C�U�p�����[�^����
		m_coeffsEdge.clear();
		for ( auto edge : m_edge ) {
			m_coeffsEdge.push_back(EdgeToRasterizeCoeff(m_draw_vertex[edge[0]], m_draw_vertex[edge[1]], m_edge_param_q));
		}
		m_coeffsShader.clear();
		for ( auto p : m_polygon ) {
			Vec3 param[3+3];
			for ( int i = 0; i < 3; ++i ) {
				T u = m_tex_cord[p.tex_cord[i]][0];
				T v = m_tex_cord[p.tex_cord[i]][1];
				T w = m_draw_vertex[p.vertex[i]][3];
				if ( perspective_correction ) {
					param[0][i] = 1 / w;
					param[1][i] = u / w;
					param[2][i] = v / w;

					param[3][i] = p.color[i][0];
					param[4][i] = p.color[i][1];
					param[5][i] = p.color[i][2];
				}
				else {
					param[0][i] = w;
					param[1][i] = u;
					param[2][i] = v;

					param[3][i] = p.color[i][0];
					param[4][i] = p.color[i][1];
					param[5][i] = p.color[i][2];
				}
			}

			std::vector<RasterizeCoeff>	rcs;
			for ( int i = 0; i < 3+3; ++i ) {
				Vec3 vertex[3];
				for ( int j = 0; j < 3; ++j ) {
					vertex[j][0] =  m_draw_vertex[p.vertex[j]][0];	// x
					vertex[j][1] =  m_draw_vertex[p.vertex[j]][1];	// y
					vertex[j][2] =  param[i][j];					// param
				}
				rcs.push_back(ShaderParamToRasterizeCoeff(vertex, m_shader_param_q));
			}
			m_coeffsShader.push_back(rcs);
		}
	}
	

	// ���X�^���C�U�ݒ�p�p�����[�^�Z�o(edge)
	void CalcEdgeRasterizerParameter(void (*procEdge)(size_t index, RasterizerParameter rp, void* user), void* user=0)				
	{
		// edge
		for ( size_t index = 0; index < m_coeffsEdge.size(); ++index ) {
			procEdge(index, m_coeffsEdge[index].GetRasterizerParameter(m_width), user);
		}
	}

	// ���X�^���C�U�ݒ�p�p�����[�^�Z�o(shader)
	void CalcShaderRasterizerParameter(void (*procShader)(size_t index, const std::vector<RasterizerParameter>& rps, void* user), void* user=0)
	{
		// shader param
		for ( size_t index = 0; index < m_coeffsShader.size(); ++index ) {
			std::vector<RasterizerParameter>	rps;
			for ( auto& rc : m_coeffsShader[index] ) {
				rps.push_back(rc.GetRasterizerParameter(m_width));
			}
			procShader(index, rps, user);
		}
	}
	
	// ���X�^���C�U�ݒ�p�p�����[�^�Z�o(region)
	void CalcRegionRasterizerParameter(void (*procRegion)(size_t index, const std::vector<PolygonRegion>& region, void* user), void* user=0)
	{
		// region
		for ( size_t index = 0; index < m_polygon.size(); ++index ) {
			procRegion(index, m_polygon[index].region, user);
		}
	}
	
	// �`��V�~�����[�V����
	void Draw(void (*proc)(int x, int y, bool polygon, PixelParam pp, void* user), void* user=0)
	{
		// �v�Z�p���j�b�g�ݒ�
		std::vector<RasterizerUnit> rasterizerEdge;
		for ( auto& rc : m_coeffsEdge ) {
			rasterizerEdge.push_back(RasterizerUnit(rc.GetRasterizerParameter(m_width)));
		}
		std::vector< std::vector<RasterizerUnit> > rasterizerParam;
		for ( auto& rcs : m_coeffsShader ) {
			std::vector<RasterizerUnit>	vec;
			for ( auto& rc : rcs ) {
				vec.push_back(RasterizerUnit(rc.GetRasterizerParameter(m_width)));
			}
			rasterizerParam.push_back(vec);
		}

		// �`��		
		std::vector<bool>	edge_flags(m_coeffsEdge.size());
		for ( int y = 0; y < m_height; ++y ) {
			for ( int x = 0; x < m_width; ++x ) {
				// �G�b�W����
				for ( size_t i = 0; i < rasterizerEdge.size(); ++i ) {
					edge_flags[i] = rasterizerEdge[i].GetEdgeDiscriminantValue();
				}
				
				// Z����
				PixelParam	pp = {};
				bool		valid = false;
				for ( size_t i = 0; i < m_polygon.size(); ++i ) {
					if ( CheckRegion(m_polygon[i].region, edge_flags) ) {
						T w, u, v;
						if ( perspective_correction ) {
							w = 1 / (T)rasterizerParam[i][0].GetShaderParamValue(m_shader_param_q);
							u = rasterizerParam[i][1].GetShaderParamValue(m_shader_param_q) * w;
							v = rasterizerParam[i][2].GetShaderParamValue(m_shader_param_q) * w;
						}
						else {
							w = rasterizerParam[i][0].GetShaderParamValue(m_shader_param_q);
							u = rasterizerParam[i][1].GetShaderParamValue(m_shader_param_q);
							v = rasterizerParam[i][2].GetShaderParamValue(m_shader_param_q);
						}
						T	r = rasterizerParam[i][3].GetShaderParamValue(m_shader_param_q);
						T	g = rasterizerParam[i][4].GetShaderParamValue(m_shader_param_q);
						T	b = rasterizerParam[i][5].GetShaderParamValue(m_shader_param_q);

						if ( !valid || pp.tex_cord[2] > w ) {
							valid = true;
							pp.matrial = m_polygon[i].matrial;
							pp.tex_cord[0] = u;
							pp.tex_cord[1] = v;
							pp.tex_cord[2] = w;
							pp.color[0] = r;
							pp.color[1] = g;
							pp.color[2] = b;
						}
					}
				}

				// �`�揈��
				proc(x, y, valid, pp, user);

				// �p�����[�^�C���N�������g
				for ( auto& ras : rasterizerEdge ) {
					ras.CalcNext(x == (m_width-1));
				}
				for ( auto& vec : rasterizerParam ) {
					for ( auto& ras : vec ) {
						ras.CalcNext(x == (m_width-1));
					}
				}
			}
		}
	}


protected:
	// �̈攻��
	bool CheckRegion(const std::vector<PolygonRegion>& region, const std::vector<bool>& edge_flags) {
		bool	and_flag = true;
		bool	or_flag  = false;
		for ( auto r : region ) {
			bool v = edge_flags[r.edge] ^ r.inverse;
			and_flag &= v;
			or_flag  |= v;
		}
		return (m_culling_cw && and_flag) || (m_culling_ccw && !or_flag);
	}
	
	// �G�b�W����W���Z�o
	RasterizeCoeff	EdgeToRasterizeCoeff(Vec4 v0, Vec4 v1, int param_q)
	{
		TI ix0 = (TI)round(v0[0]);
		TI iy0 = (TI)round(v0[1]);
		TI x0 = (TI)round(v0[0] * (1 << param_q));
		TI y0 = (TI)round(v0[1] * (1 << param_q));
		TI x1 = (TI)round(v1[0] * (1 << param_q));
		TI y1 = (TI)round(v1[1] * (1 << param_q));
		
		RasterizeCoeff	rc;
		rc.dx = y0 - y1;
		rc.dy = x1 - x0;
		rc.c  = -((iy0 * rc.dy) + (ix0 * rc.dx));

		if ( (rc.dy < 0 || (rc.dy == 0 && rc.dx < 0)) ) {
			rc.c--;
		}

		return rc;
	}

	// �|���S���p�����[�^�W���v�Z
	RasterizeCoeff	ShaderParamToRasterizeCoeff(Vec3 vertex[3], int param_q)
	{
		Vec3	vector0 = SubVec3(vertex[1], vertex[0]);
		Vec3	vector1 = SubVec3(vertex[2], vertex[0]);
		Vec3	cross   = CrossVec3(vector0, vector1);

		T		dx = -cross[0] / cross[2];
		T		dy = -cross[1] / cross[2];
		T		c  = (cross[0]*vertex[0][0] + cross[1]*vertex[0][1] + cross[2]*vertex[0][2]) / cross[2];

		RasterizeCoeff	rc;
		rc.dx = (TI)(dx * (1 << param_q));
		rc.dy = (TI)(dy * (1 << param_q));
		rc.c  = (TI)(c  * (1 << param_q));

		return rc;
	}
	


	// -----------------------------------------
	//  CG�p�s��v�Z�⏕�֐�
	// -----------------------------------------
public:
	// �r���[�|�[�g�ݒ�
	static	Mat4 ViewportMat4(int x, int y, int width, int height)
	{
		Mat4 mat = IdentityMat4();
		mat[0][0] = (T)width / (T)2;
		mat[0][3] = (T)x + (width / (T)2);
		mat[1][1] = (T)height / (T)2;
		mat[1][3] = (T)y + (height / (T)2);
		return mat;
	}

	// ���_�ݒ�
	static	Mat4 LookAtMat4(Vec3 eye, Vec3 center, Vec3 up)
	{
		up = NormalizeVec3(up);
		Vec3 f = NormalizeVec3(SubVec3(center, eye));
		Vec3 s = CrossVec3(f, up);
		Vec3 u = CrossVec3(NormalizeVec3(s), f);
		Mat4 mat = { s[0],  s[1],  s[2],  0,
				     u[0],  u[1],  u[2],  0,
				    -f[0], -f[1], -f[2],  0,
			            0,     0,     0,  1};
		return MulMat(mat, TranslatedMat4(NegVec3(eye)));
	}

	// ���s�ړ�
	static	Mat4 TranslatedMat4(Vec3 translated)
	{
		Mat4 mat = IdentityMat4();
		mat[0][3] = translated[0];
		mat[1][3] = translated[1];
		mat[2][3] = translated[2];
		return mat;
	}
	
	// ��]
	static	Mat4 RotateMat4(T angle,  Vec3 up)
	{
		angle *= (T)(3.14159265358979 / 180.0);
		up = NormalizeVec3(up);

		T s = sin(angle);
		T c = cos(angle);
		Mat4 mat = IdentityMat4();
		mat[0][0] = up[0]*up[0]*(1-c)+c;
		mat[0][1] = up[0]*up[1]*(1-c)-up[2]*s;
		mat[0][2] = up[0]*up[2]*(1-c)+up[1]*s;
		mat[1][0] = up[1]*up[0]*(1-c)+up[2]*s;
		mat[1][1] = up[1]*up[1]*(1-c)+c;
		mat[1][2] = up[1]*up[2]*(1-c)-up[0]*s;
		mat[2][0] = up[2]*up[0]*(1-c)-up[1]*s;
		mat[2][1] = up[2]*up[1]*(1-c)+up[0]*s;
		mat[2][2] = up[2]*up[2]*(1-c)+c;
		return mat;
	}

	// ���_�ݒ�
	static	Mat4 PerspectiveMat4(T fovy, T aspect, T zNear, T zFar)
	{
		fovy *= (T)(3.14159265358979 / 180.0);

		T f = (T)(1.0/tan(fovy/2.0));
		Mat4 mat = ZeroMat4();
		mat[0][0] = f / aspect;
		mat[1][1] = f;
		mat[2][2] = (zFar+zNear)/(zNear-zFar);
		mat[2][3] = (2*zFar*zNear)/(zNear-zFar);
		mat[3][2] = -1;
		return mat;
	}
	

	// -----------------------------------------
	//  H/W����
	// -----------------------------------------

protected:
	// ���W�X�^��������
	void WriteHwRegister(uint32_t add, uint32_t data)
	{
		*(volatile uint32_t*)(m_hw_base_addr + add) = data;
	}

	// ���W�X�^�ǂݍ���
	uint32_t ReadHwRegister(uint32_t add)
	{
		return *(volatile uint32_t*)(m_hw_base_addr + add);
	}
	
	// �p�����[�^���W�X�^��������
	void WriteHwParamRegister(uint32_t bank, uint32_t param, uint32_t addr, uint32_t data)
	{
		WriteHwRegister((bank * m_hw_bank_step) + (param * m_hw_params_step) + addr, data);
	}
	
	// �G�b�W�p�����[�^��������
	void WriteHwEdgeParam(uint32_t bank, uint32_t index, RasterizerParameter rp)
	{
		uint32_t addr = index * 3 * 4;
		WriteHwParamRegister(bank, 1, addr, rp[0]);	addr += 4;
		WriteHwParamRegister(bank, 1, addr, rp[1]);	addr += 4;
		WriteHwParamRegister(bank, 1, addr, rp[2]);
	}

	// �V�F�[�_�[�p�����[�^��������
	void WriteHwEdgeParam(uint32_t bank, uint32_t polygon_index, uint32_t param_index, RasterizerParameter rp)
	{
		uint32_t addr = (polygon_index * m_hw_shader_param_num + param_index) * 3 * 4;
		WriteHwParamRegister(bank, 2, addr, rp[0]);	addr += 4;
		WriteHwParamRegister(bank, 2, addr, rp[1]);	addr += 4;
		WriteHwParamRegister(bank, 2, addr, rp[2]);
	}

	// �̈�p�����[�^��������
	void WriteHwEdgeParam(uint32_t bank, uint32_t index, uint32_t flag, uint32_t polality)
	{
		uint32_t addr = index * 2 * 4;
		WriteHwParamRegister(bank, 3, addr, flag);		addr += 4;
		WriteHwParamRegister(bank, 3, addr, polality);
	}

public:
	// H/W������
	void SetupHwCore(uint32_t base_addr, bool auto_config=true)
	{
		// �x�[�X�A�h���X�ݒ�
		m_hw_base_addr = base_addr;

		// �ݒ�ǂݏo��
		if ( auto_config ) {
			m_hw_shader_type      = ReadHwRegister(REG_ADDR_CFG_SHDER_TYPE);
			m_hw_version          = ReadHwRegister(REG_ADDR_CFG_VERSION);
			m_hw_bank_step        = (4 << ReadHwRegister(REG_ADDR_CFG_BANK_ADDR_WIDTH));
			m_hw_params_step      = (4 << ReadHwRegister(REG_ADDR_CFG_PARAMS_ADDR_WIDTH));
			m_hw_bank_num         = ReadHwRegister(REG_ADDR_CFG_BANK_NUM);
			m_hw_edge_num         = ReadHwRegister(REG_ADDR_CFG_EDGE_NUM);
			m_hw_polygon_num      = ReadHwRegister(REG_ADDR_CFG_POLYGON_NUM);
			m_hw_shader_param_num = ReadHwRegister(REG_ADDR_CFG_SHADER_PARAM_NUM);
			m_hw_shader_param_q   = ReadHwRegister(REG_ADDR_CFG_SHADER_PARAM_Q);
			if ( m_hw_shader_param_q > 0 ) {
				m_shader_param_q = m_hw_shader_param_q;
			}

			m_hw_shader_param_has_z        = (m_hw_shader_type & 0x01);
			m_hw_shader_param_has_tex_cord = (m_hw_shader_type & 0x02);
			m_hw_shader_param_has_color    = (m_hw_shader_type & 0x04);
		}
	}

	// �`����{
	void DrawHw(uint32_t bank)
	{
	}



	// -----------------------------------------
	//  �s��v�Z�⏕�֐�
	// -----------------------------------------
public:
	// �P�ʍs�񐶐�
	static	Mat4 IdentityMat4(void)
	{
		Mat4	mat;
		for ( size_t i = 0; i < mat.size(); ++i ) {
			for ( size_t j = 0; j < mat[i].size(); ++j ) {
				mat[i][j] = (i == j) ? (T)1 : (T)0;
			}
		}
		return mat;
	}
	
	// �P�ʍs�񐶐�
	static	Mat4 ZeroMat4(void)
	{
		Mat4	mat;
		for ( size_t i = 0; i < mat.size(); ++i ) {
			for ( size_t j = 0; j < mat[i].size(); ++j ) {
				mat[i][j] = (T)0;
			}
		}
		return mat;
	}


	// �s���Z
	static	Mat4 MulMat(const Mat4 matSrc0, const Mat4 matSrc1)
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
	static	Vec4 MulMat(const Mat4 matSrc, const Vec4 vecSrc)
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
	static	Vec3 MulPerspectiveVec3(const Mat4 matSrc, const Vec3 vecSrc)
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
	

	// �s��̃x�N�^�ւ̓K�p(�ˉe)
	static	Vec4 MulPerspectiveVec4(const Mat4 matSrc, const Vec3 vecSrc)
	{
		Vec4	vecIn;
		Vec4	vecDst;

		vecIn[0] = vecSrc[0];
		vecIn[1] = vecSrc[1];
		vecIn[2] = vecSrc[2];
		vecIn[3] = 1.0f;
		vecDst = MulMat(matSrc, vecIn);	
		vecDst[0] = vecDst[0] / vecDst[3];
		vecDst[1] = vecDst[1] / vecDst[3];
		vecDst[2] = vecDst[2] / vecDst[3];

		return vecDst;
	}

	// �x�N�g���̕������]
	static	Vec3 NegVec3(const Vec3 vecSrc) {
		Vec3 vecDst;
		for ( size_t i = 0; i < vecDst.size(); i++ ) {
			vecDst[i] = -vecSrc[i];
		}
		return vecDst;
	}

	// �x�N�g���̌��Z
	static	Vec3 SubVec3(const Vec3 vec0, const Vec3 vec1) {
		Vec3 vecDst;
		for ( size_t i = 0; i < vecDst.size(); i++ ) {
			vecDst[i] = vec0[i] - vec1[i];
		}
		return vecDst;
	}
	
	// �x�N�g���̊O��
	static	Vec3 CrossVec3(const Vec3 vec0, const Vec3 vec1)
	{
		Vec3	vecCross;
		vecCross[0] = vec0[1]*vec1[2] - vec0[2]*vec1[1];
		vecCross[1] = vec0[2]*vec1[0] - vec0[0]*vec1[2];
		vecCross[2] = vec0[0]*vec1[1] - vec0[1]*vec1[0];
		return vecCross;
	}

	// �m�����v�Z
	static	T NormVec3(const Vec3 vec)
	{
		T norm = 0;
		for ( size_t i = 0; i < vec.size(); i++ ) {
			norm += vec[i] * vec[i];
		}
		return sqrt(norm);
	}

	// �P�ʃx�N�g����
	static	Vec3 NormalizeVec3(const Vec3 vec)
	{
		T norm = NormVec3(vec);
		Vec3	vecDst;
		for ( size_t i = 0; i < vecDst.size(); i++ ) {
			vecDst[i] = vec[i] / norm;
		}
		return vecDst;
	}
};



#endif	// __RYUZ__JELLY_GL__H__


// end of file
