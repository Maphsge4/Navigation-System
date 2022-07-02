#ifndef INSIDE_H
#define INSIDE_H

#include <QMainWindow>
#include <QPainter>
#include <QVector>
#include <QMap>
#include "map.h"

class campusShahe;
class campusBenbu;
class MainWindow;

class inside : public QMainWindow
{
    Q_OBJECT
public:
    explicit inside(QWidget *parent = nullptr);

    void paintEvent(QPaintEvent *);

    // 起始点到终止点的路径vector
    QVector<path> pathAll;

    // 记录当前点到周围点的路径
    QVector<path> curToAroundPath;

    // 人的当前位置
    position curPosition;

    // 函数中new实例化出沙河校区地图
    Map *insideMap;

    // 周围点的vector
    QVector<point> pointsAround;

    // 换校区的 flag
    int changeFlag;

    campusShahe * shahe;

    // 导航策略
    int navigationStrategy;
signals:

};

#endif // INSIDE_H
