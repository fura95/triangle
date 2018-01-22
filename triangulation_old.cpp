#include "mainwindow.h"
#include "triangulation.h"
#include <algorithm>
#include <vector>
using namespace std;


triangulation::triangulation() :
x_original(NULL),
y_original(NULL),
f_original(NULL)
{
}
triangulation::triangulation(vector<double> *x_ref, vector<double>* y_ref, vector<double>* f_ref) :
    x_original(x_ref),
    y_original(y_ref),
    f_original(f_ref)
{
    fillPointStore(x_ref, y_ref, f_ref);
    fillEdgeStore();
    fillTriangleStore();
}
//int Triangulation::fillTriangleStore(){ return 0; };

int triangulation::is_equal(point p1, point p2)
{
    if (p1.x == p2.x && p1.y == p2.y)
        return 1;
    else return 0;
}

int triangulation::is_cross(edge s_in, edge s_out)
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
    if (t == 0 && t == ta1 && t == tb1)
    {
        edge s;
        s.first.x = 0;
        s.first.y = 0;
        s.second.x = 0;
        s.second.y = 0;
        if (is_equal(a1,b1) == 1)
        {
            s.first = a2;
            s.second = b2;
        }
        if (is_equal(a1,b2) == 1)
        {
            s.first = a2;
            s.second = b1;
        }
        if (is_equal(a2,b1) == 1)
        {
            s.first = a1;
            s.second = b2;
        }
        if (is_equal(a2,b2) == 1)
        {
            s.first = a1;
            s.second = b1;
        }

        if (s_in.get_len() + s_out.get_len() == s.get_len())
            return 0;
        else
            if (s.get_len() == 0)
                return 0;
            else
                return 2;
    }
    if (t == 0 && t != ta1 && t != tb1) return 0;
        if (ta1 / t > 0 && ta1 / t < 1 && tb1 / t > 0 && tb1 / t < 1) return 1;
    return 0;
}

bool triangulation::cmp_len (edge elem1, edge elem2)
{
    return elem1.get_len() < elem2.get_len();
}

int triangulation::fillPointStore(vector<double>* x_ref, vector<double>* y_ref, vector<double>* f_ref)
{
    int n = x_ref->size();
    for (int i = 0; i < n; i++)
    {
        point p;
        p.x = x_ref->back();
        p.y = y_ref->back();
        p.f = f_ref->back();
        x_ref->pop_back();
        y_ref->pop_back();
        f_ref->pop_back();
        pointStore.push_back(p);
    }
    return 0;
}

int triangulation::fillEdgeStore()
{
    vector<edge> edges_h;
    vector<point>::iterator p_iter_1;
    for ( p_iter_1 = pointStore.begin(); p_iter_1 != pointStore.end(); p_iter_1++)
    {
        vector<point>::iterator p_iter_2;
        for ( p_iter_2 = p_iter_1; p_iter_2 != pointStore.end(); p_iter_2++)
        {
            edge ed;
            ed.first = *p_iter_1;
            ed.second = *p_iter_2;
            if (ed.first.x == ed.second.x && ed.first.y == ed.second.y)
            {}
            else
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
        vector<edge>::iterator e_iter;
        for (e_iter = edgeStore.begin(); e_iter != edgeStore.end(); e_iter++)
        {
            edge s_2 = *e_iter;
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

    return 0;
}


int triangulation::buildNet(vector<double>* x_ref, vector<double>* y_ref, vector<double>* f_ref)
{
    fillPointStore(x_ref, y_ref, f_ref);
    fillEdgeStore();
    fillTriangleStore();
    return 0;
}


int triangulation::erase()
{
    pointStore.clear();
    edgeStore.clear();
    triangleStore.clear();
    return 0;
}
