#ifndef CAMPUSSHAHE_H
#define CAMPUSSHAHE_H

#include <QMainWindow>
#include <QPainter>
#include <QVector>
#include <QMap>
#include "map.h"

class inside;
class campusBenbu;
class MainWindow;
class showDetailedInfo;

class campusShahe : public QMainWindow
{
    Q_OBJECT
public:
    explicit campusShahe(QWidget *parent = nullptr);

    // 重写绘图事件
    void paintEvent(QPaintEvent*);

    QString showNextSchoolBus(QString presentTime);

    QString showNextBus(QString presentTime);

    // 起始点到终止点的路径vector
    QVector<path> pathAll;

    // 记录当前点到周围点的路径
    QVector<path> curToAroundPath;

    // 人的当前位置
    position curPosition;

    // 函数中new实例化出沙河校区地图
    Map *shaheCampus;

    // 周围点的vector
    QVector<point> pointsAround;

    // 换校区的 flag
    int changeFlag;

    // 中间页面
    MainWindow * middleMap = NULL;

    // 内部页面
    inside * insideMap = NULL;

    //
    showDetailedInfo * detailedInfo = NULL;

    // 导航策略
    int navigationStrategy;

signals:
};

#endif // CAMPUSSHAHE_H
