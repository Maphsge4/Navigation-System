#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QVector>
#include <QMap>
#include "map.h"

class campusBenbu;
class campusShahe;
class inside;

typedef struct
{
    int x;
    int y;
} MainScenePoint;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    campusShahe * shahe = NULL;

    campusBenbu * benbu = NULL;

    inside * classroom = NULL;

    void paintEvent(QPaintEvent*);

    QVector<MainScenePoint> mainScenePath;

    MainScenePoint mainSceneCurPoint;
private:
    Ui::MainWindow *ui;

signals:

};
#endif // MAINWINDOW_H
