#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QLineEdit>
#include <QPushButton>
#include <QProgressBar>
#include <QFile>
#include <QFileDialog>
#include <QVector>
#include <vector>
#include <QGraphicsItem>
#include <QWheelEvent>
#include <QTextStream>
#include <QPolygonF>
#include <QBrush>
#include <QGraphicsPolygonItem>
#include <QThread>
#include "triangulation.h"
#include "triangulationwithcolor.h"
#include <Fade_2D.h>
using namespace GEOM_FADE25D;
namespace Ui
{
class Fade_25D;
class MainWindow;
class Initializer;
}

class Initializer : public QThread
{
public:
    class MainWindow* wind;
    Q_OBJECT
        void run() Q_DECL_OVERRIDE;
    signals:
        void resultReady();
        void drawScene();

};

struct GraphData
{
public:
    QFile file;
    QVector <double> array;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    friend Initializer;
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setData(GraphData *dataIn);

private slots:
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();
    void on_checkBox_clicked();
    void on_checkBox_2_clicked();
    void handleResults();
    void drawScene();
    void changeProgress(long long value);

private:
    void loadFile(const QString &fileName);

//My fields:
    Ui::MainWindow *ui;
    GraphData *data;
    QGraphicsScene scene;
    vector<double>  x_store,
                    y_store,
                    f_store;
    TriangulationWithColor* myTriang_p;
    vector<QGraphicsLineItem*> edgeStore;
    vector<QGraphicsPolygonItem*> polygonStore;
    long long multiple;
    char state;
    QThread* supportThread;
    Initializer* myInit;
};


#endif // MAINWINDOW_H
