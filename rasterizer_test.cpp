

#include <stdio.h>
#include "opencv2/opencv.hpp"
#include "JellyGL.h"


typedef	JellyGL<float>	JGL;



std::array<float, 3> table_vertex[8] = {
	{-1, -1, -1},
	{+1, -1, -1},
	{-1, +1, -1},
	{+1, +1, -1},
	{-1, -1, +1},
	{+1, -1, +1},
	{-1, +1, +1},
	{+1, +1, +1},
};

std::array<float, 2> table_tex_cord[4] = {
	{0, 0},
	{1, 0},
	{1, 1},
	{0, 1},
};





cv::Mat img;
cv::Mat imgTex;


void RenderProc(int x, int y, bool polygon, JGL::PixelParam pp, void* user)
{
	if ( polygon ) {
		float u = MAX(0.0f, MIN(1.0f, pp.u));
		float v = MAX(0.0f, MIN(1.0f, pp.v));
		u *= (imgTex.cols-1);
		v *= (imgTex.rows-1);
		
		img.at<cv::Vec3b>(y, x) = imgTex.at<cv::Vec3b>(v, u);
	}
}



void rasterizer_test(void)
{
//	imgTex = cv::imread("DSC_0030.jpg");
	imgTex = cv::imread("Penguins.jpg");
	img = cv::Mat::zeros(480, 640, CV_8UC3);

	JGL	jgl;
	jgl.SetVertexBuffer(std::vector<JGL::Vec3>(table_vertex, std::end(table_vertex)));
	jgl.SetTexCordBuffer(std::vector<JGL::Vec2>(table_tex_cord, std::end(table_tex_cord)));

	std::vector<JGL::Face>	face_table;
	JGL::Face				f;
	
	// ÉLÉÖÅ[ÉuÇÃÇUñ Çê›íË
	f.clear();
	f.push_back(JGL::FacePoint(0, 0));
	f.push_back(JGL::FacePoint(2, 1));
	f.push_back(JGL::FacePoint(3, 2));
	f.push_back(JGL::FacePoint(1, 3));
	face_table.push_back(f);

#if 1
	f.clear();
	f.push_back(JGL::FacePoint(7, 0));
	f.push_back(JGL::FacePoint(6, 1));
	f.push_back(JGL::FacePoint(4, 2));
	f.push_back(JGL::FacePoint(5, 3));
	face_table.push_back(f);

	f.clear();
	f.push_back(JGL::FacePoint(0, 0));
	f.push_back(JGL::FacePoint(1, 1));
	f.push_back(JGL::FacePoint(5, 2));
	f.push_back(JGL::FacePoint(4, 3));
	face_table.push_back(f);

	f.clear();
	f.push_back(JGL::FacePoint(1, 0));
	f.push_back(JGL::FacePoint(3, 1));
	f.push_back(JGL::FacePoint(7, 2));
	f.push_back(JGL::FacePoint(5, 3));
	face_table.push_back(f);

	f.clear();
	f.push_back(JGL::FacePoint(3, 0));
	f.push_back(JGL::FacePoint(2, 1));
	f.push_back(JGL::FacePoint(6, 2));
	f.push_back(JGL::FacePoint(7, 3));
	face_table.push_back(f);

	f.clear();
	f.push_back(JGL::FacePoint(2, 0));
	f.push_back(JGL::FacePoint(0, 1));
	f.push_back(JGL::FacePoint(4, 2));
	f.push_back(JGL::FacePoint(6, 3));
	face_table.push_back(f);
#endif

	jgl.SetFaceBuffer(face_table);
	jgl.MakeDrawList();

	float angle = 0;
	do {
		JGL::Mat4	matRotate		= JGL::RotateMat4(angle, {0, 1, 0});
		JGL::Mat4	matLookAt       = JGL::LookAtMat4({2, 2, +5}, {0, 0, 0}, {0, 1, 0});
		JGL::Mat4	matPerspectivet = JGL::PerspectiveMat4(20.0, 1.0, 0.1, 1000);

		JGL::Mat4	mat = JGL::IdentityMat4();
		mat[0][0] = 100.0;
		mat[1][1] = 100.0;
		mat[0][3] = 640/2;
		mat[1][3] = 480/2;
	//	jgl.SetMatrix(JGL::MulMat(mat, matLookAt));
		jgl.SetMatrix(JGL::MulMat(mat, JGL::MulMat(matPerspectivet, JGL::MulMat(matLookAt, matRotate))));
	//	jgl.SetMatrix(mat);

		img = cv::Mat::zeros(480, 640, CV_8UC3);
		jgl.Draw(640, 480, RenderProc);

		angle += 1;

		cv::imshow("img", img);
	} while ( cv::waitKey(10) != 0x1b );
}


#if 0

std::vector<float[3]>	vertex(table_vertex, std::end(table_vertex));
std::vector<float[2]>	tex_cord(table_tex_cord, std::end(table_tex_cord));


struct TPoint {
	int		vertex;
	int		tex_cord;

	TPoint(int v, int vt) {
		int vertex   = v;
		int tex_cord = vt;
	}
};

std::vector< std::vector<TPoint> >	polygon;


void rasterizer_test(void)
{
	// ÉLÉÖÅ[ÉuÇÃÇUñ Çï`âÊ
	std::vector<TPoint> p;
	
	p.clear();
	p.push_back(TPoint(0, 0));
	p.push_back(TPoint(2, 1));
	p.push_back(TPoint(3, 2));
	p.push_back(TPoint(1, 3));
	polygon.push_back(p);

	p.clear();
	p.push_back(TPoint(7, 0));
	p.push_back(TPoint(6, 1));
	p.push_back(TPoint(4, 2));
	p.push_back(TPoint(5, 3));
	polygon.push_back(p);

	p.clear();
	p.push_back(TPoint(0, 0));
	p.push_back(TPoint(1, 1));
	p.push_back(TPoint(5, 2));
	p.push_back(TPoint(4, 3));
	polygon.push_back(p);

	p.clear();
	p.push_back(TPoint(1, 0));
	p.push_back(TPoint(3, 1));
	p.push_back(TPoint(7, 2));
	p.push_back(TPoint(5, 3));
	polygon.push_back(p);

	p.clear();
	p.push_back(TPoint(3, 0));
	p.push_back(TPoint(2, 1));
	p.push_back(TPoint(6, 2));
	p.push_back(TPoint(7, 3));
	polygon.push_back(p);

	p.clear();
	p.push_back(TPoint(2, 0));
	p.push_back(TPoint(0, 1));
	p.push_back(TPoint(4, 2));
	p.push_back(TPoint(6, 3));
	polygon.push_back(p);


	for ( auto p : polygon ) {

	}
}





#endif