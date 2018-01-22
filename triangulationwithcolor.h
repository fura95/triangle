#ifndef TRIANGULATIONWITHCOLOR
#define TRIANGULATIONWITHCOLOR
#include "triangulation.h"
#include <vector>
#include <QVector>
class TriangulationWithColor : public triangulation
{
public:
    struct ColoredTriangle : triangulation::triangle
    {
//        triangulation::point centralPoint;
        int color;
        double centralValue;
    };
    QVector <ColoredTriangle> coloredStore;
    double maxValue;
    double minValue;

    TriangulationWithColor(vector<double>* x_ref, vector<double>* y_ref, vector<double>* f_ref);
    void makeContours(long long num);
    void buildColorStore();
};

#endif // TRIANGULATIONWITHCOLOR

