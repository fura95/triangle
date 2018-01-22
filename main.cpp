#include "mainwindow.h"
#include "triangulation.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GraphData data;
    MainWindow w;
    w.setData(&data);
    w.show();

    return a.exec();
}
