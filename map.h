#ifndef MAP_H
#define MAP_H

#include <QObject>
#include <QVector>
#include <QPair>
#include <QString>
#include <QMap>
#include <string>
#include <map>

// 跨校区模式： 0：默认，1：沙河 -> 本部，2：本部 -> 沙河
#define DefaultRoute 0
#define ShaheToBenbu 1
#define BenbuToShahe 2

// 导航策略： 1：最短距离，2:最短时间，3:自行车
#define MinDistance 1
#define MinTime 2
#define MinBike 3

// 校区: 1 沙河  2 本部   3 内部
#define SHAHECAMPUS 1
#define BENBUCAMPUS 2
#define INSIDEMAP 3

//
#define CLASSROOM 1
#define DORMITORY 2

// 展示具体信息的窗口种类
#define CANTEEN 1
#define SCHOOLBUS 2
#define BUS 3
#define CHOOSETRANSPORTMETHOD 4

// 点 point的结构体声明
typedef struct {
    int id; // 每个点特有的id值
    QString name; // 名称
    int x, y; // xy坐标
    int dis; // 在算周围点距离时候用到
}point;

// 边 edge的结构体声明
typedef struct {
    bool tongxing;
    // 方向：1 水平，0 垂直
    bool dir;
    bool bicycle; // 是否可以骑车通过
    double crowd; // 道路拥挤度
    int dis; // 距离
    int timeDis;
    int bikeDis;
}edge;

// 路径 path的结构体声明
typedef struct path{
    point start; // 起始点的信息
    point end; // 终止点的信息
    // 1 水平，0 垂直
    bool flag;
    // 1 左->右 下->上
    // 0 右->左 上->下
    bool direction;
    bool bicycle; // 是否可以骑车通过
    double crowd; // 道路拥挤度
}path;

// position表示人的当前位置
typedef struct {
    int x, y; // 人当前的xy坐标
    int pathId; // 所处路径在整个路径数组里面的索引
}position;

class Map : public QObject
{
    Q_OBJECT
public:
    explicit Map();

    // 沙河校区
    point p_shahe[100];
    edge e_shahe[100][100];

    // 本部校区
    point p_benbu[100];
    edge e_benbu[100][100];

    // S4宿舍楼 和 教学楼
    point p_inside[25];
    edge e_inside[25][25];

    // 方便查询一个地点名对应的Id，用到了STL的map
    QMap<QString, int> castShahe;   // 沙河校区的映射
    QMap<QString, int> castBenbu;   // 本部的映射
    QMap<QString, int> castInside;  // 内部点的映射


    // 最短路径算法，返回一个path的vector
    QVector<path> dijkstra(QString startName, QString endName, int mode, int campusId);

    // 获取周围点的信息，返回一个point的vector
    QVector<point> findPointsAround(point cur, int campusId);
signals:

};

#endif // MAP_H
