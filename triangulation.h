#ifndef TRIANGULATION_H
#define TRIANGULATION_H

#include <vector>
#include <cmath>
#include <Fade_2D.h>
using namespace GEOM_FADE25D;
using namespace std;

class triangulation
{
public:
	struct point
	{
		double x;
		double y;
		double f;
	};
	struct edge
	{
		point first;
		point second;
		double get_len()
		{
			return sqrt((first.x - second.x)*(first.x - second.x) + (first.y - second.y) * (first.y - second.y));
		}
	};
	struct triangle
	{
		point first;
		point second;
		point third;
	};

	vector<point> pointStore;
	vector<edge> edgeStore;
	vector<triangle> triangleStore;
	vector<double>* x_original;
	vector<double>*	y_original;
	vector<double>*	f_original;
    Fade_2D fadeTr;

	triangulation(void);
	triangulation(vector<double>* x_ref, vector<double>* y_ref, vector<double>* f_ref);

	int buildNet(vector<double>* x_ref, vector<double>* y_ref, vector<double>* f_ref);//-------------------	building triangulation net
	int fillPointStore(vector<double>* x_ref, vector<double>* y_ref, vector<double>* f_ref);
	int fillTriangleStore();
	int fillEdgeStore();
	int erase();	//clean stores
	int is_cross(edge, edge);
	bool cmp_len (edge, edge);
	int is_equal(point, point);
	struct sort_class_len
	{
		bool operator() (edge i, edge j)
		{ 
			return (i.get_len() < j.get_len());
		}
	} sort_object_len;    

	int find_triangles(vector<edge>::iterator, vector<edge>*, vector<triangle>*);
	int cmp_point(point,point);
	int corr_triangles(vector<triangle>*);
};
#endif // TRIANGULATION_H

