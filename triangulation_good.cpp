#include "triangulation.h"
#include <algorithm>
#include <vector>
#include <omp.h>
#include <cmath>
using namespace std;

double eps = 0.0000001;
triangulation::triangulation() :
x_original(NULL),
y_original(NULL),
f_original(NULL)
{
};
triangulation::triangulation(vector<double>* x_ref, vector<double>* y_ref, vector<double>* f_ref) :
	x_original(x_ref),
	y_original(y_ref),
	f_original(f_ref)
{
	fillPointStore(x_ref, y_ref, f_ref);
	fillEdgeStore();
	fillTriangleStore();
};

int triangulation::is_equal(point p1, point p2)
{
	if (p1.x == p2.x && p1.y == p2.y)
		return 1;
	else return 0;
}

int triangulation::is_cross(edge s_in, edge s_out) // for edges
{
	point a1, a2, b1, b2;
	//double ta, tb;
	a1 = s_in.first; // a
	a2 = s_in.second; // b
	b1 = s_out.first;
	b2 = s_out.second;
	double ta1=(b2.x-b1.x)*(a1.y-b1.y)-(b2.y-b1.y)*(a1.x-b1.x);
	double t=(b2.y-b1.y)*(a2.x-a1.x)-(b2.x-b1.x)*(a2.y-a1.y);
	double tb1=(a2.x-a1.x)*(a1.y-b1.y)-(a2.y-a1.y)*(a1.x-b1.x);
	if (t == 0 && t == ta1 && t == tb1) // edges on the same line
	{
		edge s;
		s.first.x = 0;
		s.first.y = 0;
		s.second.x = 0;
		s.second.y = 0;
		if (is_equal(a1,b1) == 1)
		{
			s.first.x = a2.x;
			s.first.y = a2.y;
			s.first.f = a2.f;
			s.second.x = b2.x;
			s.second.y = b2.y;
			s.second.f = b2.f;
		}
		if (is_equal(a1,b2) == 1)
		{
			s.first.x = a2.x;
			s.first.y = a2.y;
			s.first.f = a2.f;
			s.second.x = b1.x;
			s.second.y = b1.y;
			s.second.f = b1.f;
		}
		if (is_equal(a2,b1) == 1)
		{
			s.first.x = a1.x;
			s.first.y = a1.y;
			s.first.f = a1.f;
			s.second.x = b2.x;
			s.second.y = b2.y;
			s.second.f = b2.f;
		}
		if (is_equal(a2,b2) == 1)
		{
			s.first.x = a1.x;
			s.first.y = a1.y;
			s.first.f = a1.f;
			s.second.x = b1.x;
			s.second.y = b1.y;
			s.second.f = b1.f;
		}

		if (abs(s_in.get_len() + s_out.get_len() - s.get_len()) <= eps)
			return 0; // two edges in line with the general point
		else 
			if (s.get_len() == 0)
				return 0; // edges are disjoint
			else
				return 2; // edges coincide or overlap
	}
	if (t == 0 && t != ta1 && t != tb1) return 0; //parallel edges
        if (ta1 / t > 0 && ta1 / t < 1 && tb1 / t > 0 && tb1 / t < 1) return 1; // edges intersect
	return 0;
}

bool triangulation::cmp_len (edge elem1, edge elem2)
{
	return elem1.get_len() < elem2.get_len();
}

int triangulation::fillPointStore(vector<double>* x_ref, vector<double>* y_ref, vector<double>* f_ref)
{
	vector<point> points;
	int n = x_ref->size();
        vector<double>::iterator x, y, f;
        x = x_ref->begin();
        y = y_ref->begin();
        f = f_ref->begin();
	for (int i = 0; i < n; i++)
	{
		point p;
		p.x = *x;
		p.y = *y;
		p.f = *f;
		x++;
		y++;
		f++;
		vector<point>::iterator p_iter;
		int k = 0;
		for (p_iter = pointStore.begin(); p_iter != pointStore.end(); p_iter++)
		{
			point point_t = *p_iter;
			if (p.x == point_t.x && p.y == point_t.y)
				k++;
		}
		if (k == 0)
		{
			pointStore.push_back(p);
		}
	}
	return 0;
}

int triangulation::fillEdgeStore()
{
	vector<edge> edges_h;
	vector<point>::iterator p_iter_1;
    long long num_of_p = pointStore.size();
    for (long long i = 0; i < num_of_p; i++)
	{
#pragma omp parallel for
                for (long long j = i; j < num_of_p; j++)
				{
					edge ed;
					ed.first = pointStore[i];
					ed.second = pointStore[j];
					if (ed.first.x == ed.second.x && ed.first.y == ed.second.y)
					{}
					else
        #pragma omp critical
						edges_h.push_back(ed);
				}
	}
	sort(edges_h.begin(), edges_h.end(), sort_object_len);
	edge e = *edges_h.begin();
	edgeStore.push_back(e);
	vector<edge>::iterator e_iter_h;
	for ( e_iter_h = ++edges_h.begin(); e_iter_h != edges_h.end(); e_iter_h++)
	{
		edge s_1 = *e_iter_h;
		long long k = 0;
		int num_of_e = edgeStore.size();
		#pragma omp parallel for
			for (int i = 0; i < num_of_e; i++)
			{
				edge s_2 = edgeStore[i];
				if (is_cross(s_1, s_2) == 1)
					k++;
				if (is_cross(s_1, s_2) == 2)
					k++;
			}
		if (k == 0)
			edgeStore.push_back(s_1);
	}
	
	return 0;
}

int triangulation::fillTriangleStore()
{
	vector<triangle> tri;
	vector<edge>::iterator e_iter_h;
	vector<edge> e;
	for ( e_iter_h = edgeStore.begin(); e_iter_h != edgeStore.end(); e_iter_h++)
	{
		e.push_back(*e_iter_h);
	}
	while (e.size() != 0)
	{
		e_iter_h = e.begin();
		vector<triangle> t;
		vector<edge>::iterator e_it;
		e_it = e_iter_h;
		if(e_it == e.end())
			return 0;
        int res = find_triangles(e_it, &e, &t);
		vector<triangle>::iterator t_it;
		for (t_it = t.begin(); t_it != t.end(); t_it++)
		{
			tri.push_back(*t_it);
		}
	}

	corr_triangles(&tri);  // filling triagleStore
	return 0;
}


int triangulation::buildNet(vector<double>* x_ref, vector<double>* y_ref, vector<double>* f_ref)
{
	fillPointStore(x_ref, y_ref, f_ref);
	fillEdgeStore();
	fillTriangleStore();
	return 0; 
}


int triangulation::erase() // clean
{
	pointStore.clear();
	edgeStore.clear();
	triangleStore.clear();
	return 0;
}


int triangulation::cmp_point(point elem1,point elem2)
{
	if (elem1.x == elem2.x && elem1.y == elem2.y )
		return 1;
	else return 0;
}

int triangulation::find_triangles(vector<edge>::iterator e_1, vector<edge> *es, vector<triangle> *t) // for edge with iterator e_1
{
	edge e = *e_1;
	triangle tk;
	vector<edge>::iterator e_iter_h;
	for ( e_iter_h = es->begin(); e_iter_h != es->end(); e_iter_h++)
	{
		edge e_h = *e_iter_h;
		if (cmp_point(e.first, e_h.first) == 1)
		{
			vector<edge>::iterator e_iter;
			for ( e_iter = es->begin(); e_iter != es->end(); e_iter++)
			{
				edge e_p = *e_iter;
				if (cmp_point(e.second, e_p.first) == 1 && cmp_point(e_h.second, e_p.second) == 1)
				{
					tk.first = e.first;
					tk.second = e.second;
					tk.third = e_h.second;
					t->push_back(tk);
				}
				if (cmp_point(e.second, e_p.second) == 1 && cmp_point(e_h.second, e_p.first) == 1)
				{
					tk.first = e.first;
					tk.second = e.second;
					tk.third = e_h.second;
					t->push_back(tk);
				}
			}
		}

		if (cmp_point(e.first, e_h.second) == 1)
		{
			vector<edge>::iterator e_iter;
			for ( e_iter = es->begin(); e_iter != es->end(); e_iter++)
			{
				edge e_p = *e_iter;
				if (cmp_point(e.second, e_p.first) == 1 && cmp_point(e_h.first, e_p.second) == 1)
				{
					tk.first = e.first;
					tk.second = e.second;
					tk.third = e_h.first;
					t->push_back(tk);
				}
				if (cmp_point(e.second, e_p.second) == 1 && cmp_point(e_h.first, e_p.first) == 1)
				{
					tk.first = e.first;
					tk.second = e.second;
					tk.third = e_h.first;
					t->push_back(tk);
				}
			}
		}
	}
	es->erase(e_1);
	return t->size();
}

int triangulation::corr_triangles(vector<triangle> *tri)
{
	vector<triangle>::iterator t_iter = tri->begin();
	while (t_iter != tri->end())
	{
		vector<triangle>::iterator tr;
		tr = t_iter;
		t_iter++;
		triangle t = *tr;
		vector<point>::iterator p_iter;
        for ( p_iter = pointStore.begin(); p_iter != pointStore.end(); p_iter++)
		{
			point p = *p_iter;
			double k1, k2, k3; //absolut values of vector protuct 
			k1=(t.first.x-p.x)*(t.second.y-t.first.y)-(t.second.x-t.first.x)*(t.first.y-p.y);
			k2=(t.second.x-p.x)*(t.third.y-t.second.y)-(t.third.x-t.second.x)*(t.second.y-p.y);
			k3=(t.third.x-p.x)*(t.first.y-t.third.y)-(t.first.x-t.third.x)*(t.third.y-p.y);
			if ((k1 > 0 && k2 > 0 && k3 > 0) || (k1 < 0 && k2 < 0 && k3 < 0))
			{
				tr->first.x = 0;
				tr->first.y = 0;
				tr->second.x = 0;
				tr->second.y = 0;
				tr->third.x = 0;
				tr->third.y = 0;
			}
		}
	}

	vector<triangle>::iterator tr; // filling triangleStore
	for ( tr = tri->begin(); tr != tri->end(); tr++)
	{
		if(tr->first.x == 0 && tr->first.y == 0 && tr->second.x == 0 && tr->second.y == 0 && tr->third.x == 0 && tr->third.y == 0)
		{}
		else triangleStore.push_back(*tr);
	}
	return 0;
}
