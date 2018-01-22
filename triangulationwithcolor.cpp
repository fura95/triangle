#include "triangulationwithcolor.h"
#include <QVector>

TriangulationWithColor::TriangulationWithColor(vector<double>* x_ref, vector<double>* y_ref, vector<double>* f_ref) :
    triangulation(x_ref, y_ref, f_ref),
    minValue(0),
    maxValue(0)
{
    buildColorStore();
}

void TriangulationWithColor::buildColorStore()
{
    bool maxValueChanged = false;
    bool minValueChanged = false;
    for (long i = 0; i < triangleStore.size(); i++)
    {

        ColoredTriangle tempTriangle;
        double averageValue = 0;
        averageValue = (triangleStore[i].first.f + triangleStore[i].second.f + triangleStore[i].third.f) / 3.0;
        tempTriangle.centralValue = averageValue;
        tempTriangle.color = 0;
        tempTriangle.first = triangleStore[i].first;
        tempTriangle.second = triangleStore[i].second;
        tempTriangle.third = triangleStore[i].third;
        coloredStore.push_back(tempTriangle);
        if (averageValue > maxValue || maxValueChanged == false)
        {
            maxValue = averageValue;
            maxValueChanged = true;
        }
        if (averageValue < minValue || minValueChanged == true)
        {
            minValue = averageValue;
            minValueChanged;
        }
    }
    for (long i = 0; i < coloredStore.size(); i++)
    {
        double a = coloredStore[i].centralValue - minValue;
        double b = a / (maxValue - minValue);
        coloredStore[i].color = (b*65535);
    }
}
void TriangulationWithColor::makeContours(long long num)
{
    vector<Triangle2*> tmpTriPtr;
    fadeTr.getTrianglePointers(tmpTriPtr);
    IsoContours myIso(tmpTriPtr);
    double min = myIso.getMinHeight(),
            max = myIso.getMaxHeight();
    vector<vector<Segment2> > tmpSeg;
    for (long long i = 0; i < num; i++)
    {
           double currentHeight = min + (i+1)*(max-min)/num;
           myIso.getContours(currentHeight, tmpSeg, false);
    }
    for (long long l = 0; l < tmpSeg.size(); l++)
    {
        for (long long it = 0; it < tmpSeg[l].size(); it++)
        {
            edge tmpEdge;
            tmpEdge.first.x = tmpSeg[l][it].getSrc().x();
            tmpEdge.first.y = tmpSeg[l][it].getSrc().y();
            tmpEdge.second.x = tmpSeg[l][it].getTrg().x();
            tmpEdge.second.y = tmpSeg[l][it].getTrg().y();
            edgeStore.push_back(tmpEdge);
        }
    }
}
