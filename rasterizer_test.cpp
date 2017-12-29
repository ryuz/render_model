

#include <stdio.h>
#include "opencv2/opencv.hpp"
#include "JellyGL.h"



float table_vertex[8][3] = {
	{-1, -1, -1},
	{+1, -1, -1},
	{-1, +1, -1},
	{+1, +1, -1},
	{-1, -1, +1},
	{+1, -1, +1},
	{-1, +1, +1},
	{+1, +1, +1},
};

float table_tex_cord[4][2] = {
	{0, 0},
	{1, 0},
	{0, 1},
	{1, 1},
};

void rasterizer_test(void)
{
	JellyGL<float>	jg;
	jg.MakeDrawList();
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