

#include <stdio.h>
#include "opencv2/opencv.hpp"
#include "JellyGL.h"


typedef	JellyGL<float>	JGL;


#if 1

// モデルの頂点リスト
std::array<float, 3> table_vertex[8*2] = {
	{-2, -2, -2},
	{+2, -2, -2},
	{-2, +2, -2},
	{+2, +2, -2},
	{-2, -2, +2},
	{+2, -2, +2},
	{-2, +2, +2},
	{+2, +2, +2},
	{-1, -1, -1},
	{+1, -1, -1},
	{-1, +1, -1},
	{+1, +1, -1},
	{-1, -1, +1},
	{+1, -1, +1},
	{-1, +1, +1},
	{+1, +1, +1},
};

// テクスチャ座標リスト
std::array<float, 2> table_tex_cord[4] = {
	{0, 0},
	{1, 0},
	{1, 1},
	{0, 1},
};


// 描画
cv::Mat img;
cv::Mat imgTex[2];


// 描画プロシージャ
void RenderProc(int x, int y, bool polygon, JGL::PixelParam pp, void* user)
{
	cv::Mat& tex = imgTex[pp.matrial];

	if ( polygon ) {
		float u = MAX(0.0f, MIN(1.0f, pp.u));
		float v = MAX(0.0f, MIN(1.0f, pp.v));
		u *= (tex.cols-1);
		v *= (tex.rows-1);
		
		img.at<cv::Vec3b>(y, x) = tex.at<cv::Vec3b>((int)round(v), (int)round(u));
	}
}



void rasterizer_test(void)
{
//	imgTex = cv::imread("DSC_0030.jpg");
	imgTex[0] = cv::imread("Penguins.jpg");
	imgTex[1] = cv::imread("Chrysanthemum.jpg");
	img = cv::Mat::zeros(480, 640, CV_8UC3);

	JGL	jgl;
	jgl.SetVertexBuffer(std::vector<JGL::Vec3>(table_vertex, std::end(table_vertex)));
	jgl.SetTexCordBuffer(std::vector<JGL::Vec2>(table_tex_cord, std::end(table_tex_cord)));

	jgl.SetModel(0, 0, 8);
	jgl.SetModel(1, 8, 16);

	std::vector<JGL::Face>	face_table;
	JGL::Face				f;
	
	// キューブの６面を設定
	f.matrial = 0;
	f.points.clear();
	f.points.push_back(JGL::FacePoint(0, 0));
	f.points.push_back(JGL::FacePoint(2, 1));
	f.points.push_back(JGL::FacePoint(3, 2));
	f.points.push_back(JGL::FacePoint(1, 3));
	face_table.push_back(f);

	f.points.clear();
	f.points.push_back(JGL::FacePoint(7, 0));
	f.points.push_back(JGL::FacePoint(6, 1));
	f.points.push_back(JGL::FacePoint(4, 2));
	f.points.push_back(JGL::FacePoint(5, 3));
	face_table.push_back(f);

	f.points.clear();
	f.points.push_back(JGL::FacePoint(0, 0));
	f.points.push_back(JGL::FacePoint(1, 1));
	f.points.push_back(JGL::FacePoint(5, 2));
	f.points.push_back(JGL::FacePoint(4, 3));
	face_table.push_back(f);

	f.points.clear();
	f.points.push_back(JGL::FacePoint(1, 0));
	f.points.push_back(JGL::FacePoint(3, 1));
	f.points.push_back(JGL::FacePoint(7, 2));
	f.points.push_back(JGL::FacePoint(5, 3));
	face_table.push_back(f);

	f.points.clear();
	f.points.push_back(JGL::FacePoint(3, 0));
	f.points.push_back(JGL::FacePoint(2, 1));
	f.points.push_back(JGL::FacePoint(6, 2));
	f.points.push_back(JGL::FacePoint(7, 3));
	face_table.push_back(f);

	f.points.clear();
	f.points.push_back(JGL::FacePoint(2, 0));
	f.points.push_back(JGL::FacePoint(0, 1));
	f.points.push_back(JGL::FacePoint(4, 2));
	f.points.push_back(JGL::FacePoint(6, 3));
	face_table.push_back(f);


	// ２個目のキューブの６面を設定
	f.matrial = 1;
	f.points.clear();
	f.points.push_back(JGL::FacePoint(8+0, 0));
	f.points.push_back(JGL::FacePoint(8+2, 1));
	f.points.push_back(JGL::FacePoint(8+3, 2));
	f.points.push_back(JGL::FacePoint(8+1, 3));
	face_table.push_back(f);
							
	f.points.clear();		
	f.points.push_back(JGL::FacePoint(8+7, 0));
	f.points.push_back(JGL::FacePoint(8+6, 1));
	f.points.push_back(JGL::FacePoint(8+4, 2));
	f.points.push_back(JGL::FacePoint(8+5, 3));
	face_table.push_back(f);
							
	f.points.clear();		
	f.points.push_back(JGL::FacePoint(8+0, 0));
	f.points.push_back(JGL::FacePoint(8+1, 1));
	f.points.push_back(JGL::FacePoint(8+5, 2));
	f.points.push_back(JGL::FacePoint(8+4, 3));
	face_table.push_back(f);
							
	f.points.clear();		
	f.points.push_back(JGL::FacePoint(8+1, 0));
	f.points.push_back(JGL::FacePoint(8+3, 1));
	f.points.push_back(JGL::FacePoint(8+7, 2));
	f.points.push_back(JGL::FacePoint(8+5, 3));
	face_table.push_back(f);

	f.points.clear();		
	f.points.push_back(JGL::FacePoint(8+3, 0));
	f.points.push_back(JGL::FacePoint(8+2, 1));
	f.points.push_back(JGL::FacePoint(8+6, 2));
	f.points.push_back(JGL::FacePoint(8+7, 3));
	face_table.push_back(f);
							
	f.points.clear();		
	f.points.push_back(JGL::FacePoint(8+2, 0));
	f.points.push_back(JGL::FacePoint(8+0, 1));
	f.points.push_back(JGL::FacePoint(8+4, 2));
	f.points.push_back(JGL::FacePoint(8+6, 3));
	face_table.push_back(f);
	
	jgl.SetFaceBuffer(face_table);
	jgl.MakeDrawList();

	cv::VideoWriter writer("output.avi", CV_FOURCC_DEFAULT, 30, cv::Size(640, 480), true);

	float angle0 = 0;
	float angle1 = 0;
	do {
		// clear
		img = cv::Mat::zeros(480, 640, CV_8UC3);

		// viewport
		jgl.SetViewport(0, 0, 640, 480);

		// model0
		JGL::Mat4	matRotate0 = JGL::RotateMat4(angle0, {0, 1, 0});	angle0 += 1;
		jgl.SetModelMatrix(0, matRotate0);
		
		// model1
		JGL::Mat4	matRotate1	 = JGL::RotateMat4(angle1, {0, 1, 0});	angle1 -= 2;
		JGL::Mat4	matTranslate = JGL::TranslatedMat4({0, 0, 2});
		jgl.SetModelMatrix(1, JGL::MulMat(matTranslate, matRotate1));
		
		// view
		JGL::Mat4	matLookAt       = JGL::LookAtMat4({5, -8, 20}, {0, 0, 0}, {0, 1, 0});
		JGL::Mat4	matPerspectivet = JGL::PerspectiveMat4(30.0f, 640.0f/480.0f, 0.1f, 1000.0f);
		jgl.SetViewMatrix(JGL::MulMat(matPerspectivet, matLookAt));

		//draw
		jgl.Draw(640, 480, RenderProc);

		// show
		cv::imshow("img", img);
		writer << img;
	} while ( cv::waitKey(10) != 0x1b );
}


#else



std::array<float, 3> table_vertex[4] = {
	{1,     1, 0},
	{639,   1, 0},
	{639, 479, 0},
	{  1, 479, 0},
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
	imgTex = cv::imread("Penguins.jpg");
	img = cv::Mat::zeros(480, 640, CV_8UC3);

	JGL	jgl;
	jgl.SetVertexBuffer(std::vector<JGL::Vec3>(table_vertex, std::end(table_vertex)));
	jgl.SetTexCordBuffer(std::vector<JGL::Vec2>(table_tex_cord, std::end(table_tex_cord)));

	std::vector<JGL::Face>	face_table;
	JGL::Face				f;
	
	// １面を設定
	f.clear();
	f.push_back(JGL::FacePoint(0, 0));
	f.push_back(JGL::FacePoint(1, 1));
	f.push_back(JGL::FacePoint(2, 2));
	f.push_back(JGL::FacePoint(3, 3));
	face_table.push_back(f);

	jgl.SetFaceBuffer(face_table);
	jgl.MakeDrawList();

//	JGL::Mat4	matRotate		= JGL::RotateMat4(angle, {0, 1, 0});
//	JGL::Mat4	matLookAt       = JGL::LookAtMat4({2, 2, +5}, {0, 0, 0}, {0, 1, 0});
//	JGL::Mat4	matPerspectivet = JGL::PerspectiveMat4(20.0, 1.0, 0.1, 1000);

	JGL::Mat4	mat = JGL::IdentityMat4();
	jgl.SetMatrix(mat);

	img = cv::Mat::zeros(480, 640, CV_8UC3);
	jgl.Draw(640, 480, RenderProc);

	cv::imshow("img", img);
	cv::waitKey();
}




#endif