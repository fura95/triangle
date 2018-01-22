#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QtMath>
#include <omp.h>
#include "triangulation.h"
#include <Fade_2D.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->progressBar->setMaximum(100);
    ui->progressBar->setMinimum(0);
    ui->progressBar->setValue(0);
    setWindowTitle("Наш проект");
    multiple = 1;
    state = 0;
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::changeProgress(long long value)
{
    ui->progressBar->setValue(value);
}

void MainWindow::on_pushButton_2_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(/*this,QString("data.txt"), QDir::homePath()*/);
    if (!fileName.isEmpty())
        loadFile(fileName);
    QFile myFile(fileName);
    if (!myFile.open(QIODevice::ReadOnly | QIODevice::Text))
           return;

    if (x_store.size() == 0 && edgeStore.size() == 0 && polygonStore.size() == 0)
    {
      QTextStream in(&myFile);
      while (!in.atEnd()) {
          QStringList tmp = in.readLine().split(" ");
          x_store.push_back(tmp[0].toDouble());
          y_store.push_back(tmp[1].toDouble());
          f_store.push_back(tmp[2].toDouble());
      }
    }
    else
    {
        x_store.clear();
//        x_store.resize(0);
        y_store.clear();
//        y_store.resize(0);
        f_store.clear();
//        f_store.resize(0);

        if (myTriang_p != 0)
            myTriang_p->erase();
        edgeStore.clear();
//        edgeStore.resize(0);
        polygonStore.clear();
//        polygonStore.resize(0);
        scene.clear();
        scene.clearFocus();
        ui->graphicsView->clearFocus();
        ui->graphicsView->resetTransform();
        ui->graphicsView->resetCachedContent();
        ui->graphicsView->resetMatrix();
        if (myTriang_p != 0)
            delete myTriang_p;

        QTextStream in(&myFile);
        while (!in.atEnd()) {
            QStringList tmp = in.readLine().split(" ");
            x_store.push_back(tmp[0].toDouble());
            y_store.push_back(tmp[1].toDouble());
            f_store.push_back(tmp[2].toDouble());
        }
    }
}
void MainWindow::loadFile(const QString &fileName)
{
    QFile file(fileName);
    data->array.push_back(0.0);
     if (!file.open(QFile::ReadOnly | QFile::Text)) {
            QMessageBox::warning(this, tr("Application"),
                                 tr("Cannot read file %1:\n%2.")
                                 .arg(fileName)
                                 .arg(file.errorString()));
            return;
        }
     ui->lineEdit->setText(file.fileName());
}


void MainWindow::on_pushButton_clicked()
{
    if (state == 0 || state == 5)
    {
        ui->pushButton->setDisabled(1);
        ui->progressBar->setMaximum(0);
        myInit = new Initializer;
        myInit->wind = this;
        connect(myInit, &Initializer::resultReady, this, &MainWindow::handleResults);
        connect(myInit, &Initializer::drawScene, this, &MainWindow::drawScene);
        connect(myInit, &Initializer::finished, myInit, &QObject::deleteLater);
        myInit->start();
    }
}
void MainWindow::setData(GraphData *dataIn)
{
    data = dataIn;
}
void QGraphicsView::wheelEvent(QWheelEvent* event) {

    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    double scaleFactor = 1.15;
    if(event->delta() > 0) {
            scale(scaleFactor, scaleFactor);
    } else {
            scale(1.0 / scaleFactor, 1.0 / scaleFactor);
    }
}

void MainWindow::on_checkBox_clicked()
{

    if (ui->checkBox->isChecked())
    {
#pragma omp parallel for
        for (long long i = 0; i < edgeStore.size(); i++)
            edgeStore[i]->setVisible(true);
    }
    else
    {
#pragma omp parallel for
        for (long long i = 0; i < edgeStore.size(); i++)
            edgeStore[i]->setVisible(false);
    }
}

void MainWindow::on_checkBox_2_clicked()
{
    if (ui->checkBox_2->isChecked())
    {
#pragma omp parallel for
        for (long long i = 0; i < polygonStore.size(); i++)
            polygonStore[i]->setVisible(true);
    }
    else
    {
#pragma omp parallel for
        for (long long i = 0; i < polygonStore.size(); i++)
            polygonStore[i]->setVisible(false);
    }
}

void Initializer::run()
{
    wind->myTriang_p = new TriangulationWithColor(&(wind->x_store), &(wind->y_store), &(wind->f_store));
//    connect(wind->myTriang_p, &triangulation::changeProgress, wind, &MainWindow::changeProgress);
//    wind->a = new Fade_2D;
//    wind->ui->progressBar->setMaximum(100);
//    wind->ui->progressBar->setValue(0);
//    long long num = wind->x_store.size();
//    for(long long i = 0; i < num; i++)
//    {
//        wind->a->insert(Point2(wind->x_store[i], wind->y_store[i]));
//        wind->ui->progressBar->setValue(((double)i/num)*100);
//    }

    emit drawScene();
}
void MainWindow::handleResults()
{
//    state = 5;
}
void MainWindow::drawScene()
{
    scene.clear();
    QPen myPen;
    QBrush myBrush;

    QVector <QColor> colors;
    double r=255;
    double g=0;
    double b=0;
    for(int i=0;i<=65535;i++)
    {
        if((i>=0)&&(i<=13107)){g+=0.0194552529182;}
        if((i>13107)&&(i<=26214))
        {
            r-=0.0194552529182;
        }
        if((i>26214)&&(i<=39321)){b+=0.0194552529182;}
        if((i>39321)&&(i<=52428)){g-=0.0194552529182;}
        if((i>52428)&&(i<=65535)){r+=0.0194552529182;}
        QColor col((int)r,(int)g, (int)b);
        colors.push_back(col);
    }


    myBrush.setStyle(Qt::SolidPattern);
    myBrush.setColor(Qt::green);
    myPen.setStyle(Qt::SolidLine);
    myPen.setCosmetic(1);
    QVector <QPointF> temp_triang_store;
    for (unsigned long i = 0; i < myTriang_p->coloredStore.size(); i++)
    {
        QPointF firstPt(myTriang_p->coloredStore[i].first.x, -myTriang_p->coloredStore[i].first.y),
                secondPt(myTriang_p->coloredStore[i].second.x, -myTriang_p->coloredStore[i].second.y),
                thirdPt(myTriang_p->coloredStore[i].third.x, -myTriang_p->coloredStore[i].third.y);
        temp_triang_store.resize(3);
        temp_triang_store[0] = firstPt;
        temp_triang_store[1] = secondPt;
        temp_triang_store[2] = thirdPt;

        myBrush.setColor(colors[myTriang_p->coloredStore[i].color]);
        myPen.setColor(colors[myTriang_p->coloredStore[i].color]);
        polygonStore.push_back(scene.addPolygon(/**tmp_polyg_p*/QPolygonF(temp_triang_store)/*newTriangPoligon*/, myPen, myBrush));

        temp_triang_store.clear();
    }
//    long long num = ui->lineEdit_2->text().toLong();
    long long num = ui->spinBox->value();
    myTriang_p->makeContours(num);
    myPen.setStyle(Qt::SolidLine);
    myPen.setColor(QColor(Qt::black));
    myPen.setJoinStyle(Qt::RoundJoin);
    myPen.setCosmetic(1);
    for (unsigned long i = 0; i < (myTriang_p->edgeStore.size()); i++)
    {
       edgeStore.push_back(scene.addLine(multiple*(myTriang_p->edgeStore[i]).first.x,
                                        -multiple*(myTriang_p->edgeStore[i]).first.y,
                                         multiple*(myTriang_p->edgeStore[i]).second.x,
                                        -multiple*(myTriang_p->edgeStore[i]).second.y,
                                         myPen));
    }
    if (ui->checkBox_2->isChecked() == false)
    #pragma omp parallel for
        for (long long i = 0; i < polygonStore.size(); i++)
            polygonStore[i]->setVisible(false);
    else
    #pragma omp parallel for
        for (long long i = 0; i < polygonStore.size(); i++)
            polygonStore[i]->setVisible(true);
    if (ui->checkBox->isChecked() == false)
    #pragma omp parallel for
        for (long long i = 0; i < edgeStore.size(); i++)
            edgeStore[i]->setVisible(false);

    double scaleFactor = ui->graphicsView->height() / scene.height(), scaleFactor1 = ui->graphicsView->width() / scene.width();
    int k = 0;
    if (scaleFactor1 < scaleFactor) scaleFactor = scaleFactor1;
    ui->graphicsView->scale(scaleFactor, scaleFactor);


    ui->graphicsView->setScene(&(scene));
    state = 5;
    ui->progressBar->setMaximum(100);
    ui->progressBar->setValue(100);
    ui->pushButton->setEnabled(1);
}

