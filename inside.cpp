#include "inside.h"
#include <QPainter>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QFontDialog>
#include <QDebug>
#include <QTextEdit>
#include <QLine>
#include <QTimer>
#include <QString>
#include <QtCore>
#include <QTableWidget>
#include <QHeaderView>
#include <QComboBox>
#include <QListView>
#include <QMessageBox>
#include "campusbenbu.h"
#include "mainwindow.h"
#include "campusshahe.h"
#define inf (~(0x1<<31))

extern QString finalName;
extern int campusTransferMode;
extern int insideMode;

inside::inside(QWidget *parent) : QMainWindow(parent)
{
    qDebug() << "in inside insideMode:" << insideMode;
    // 设置固定的大小
    this->setFixedSize(1400, 840);
    // 窗口标题
    this->setWindowTitle("导航系统");

    // 通过 map类实例化沙河校区地图
    insideMap = new Map();

    this->navigationStrategy = MinDistance;

    // 字体部分
    QFont font;
    font.setFamily("华文新魏");
    font.setPointSize(18);

    font.setPointSize(36);
    // 大标题 “北邮沙河校区导航”
    QLabel * header = new QLabel(this);
    header->setFont(font);
    QString str;
    if (insideMode == CLASSROOM)
        str = QString("N教学楼内部导航");
    else if (insideMode == DORMITORY)
        str = QString("S4宿舍楼内部导航");
    header->setText(str);
    header->setFixedSize(360, 60);
    header->move(this->width() * 0.5 - header->width() * 0.5 - 50, 50);

    // 更改font使用到不同情况，下面相同代码逻辑同理
    font.setFamily("华文宋体");

    // 返回按钮
    QPushButton * backBtn = new QPushButton(this);
    font.setPointSize(16);
    backBtn->setFont(font);
    backBtn->setText("返回沙河校区");
    backBtn->setGeometry(0, 50, 120, 50);
    connect(backBtn, &QPushButton::clicked, [=](){
        this->shahe = new campusShahe;
        QTimer::singleShot(200, this, [=](){
            this->shahe->setGeometry(this->geometry());
            this->shahe->show();
            this->hide();
        });
    });

    // 小标题 导航查询
    QLabel * searchLabel = new QLabel(this);
    font.setPointSize(26);
    searchLabel->setText("导航查询");
    searchLabel->setFont(font);
    searchLabel->setGeometry(QRect(1155, 80, 150, 60));
    // 小标题 周围信息
    QLabel * showLabel = new QLabel(this);
    showLabel->setText("周围信息");
    showLabel->setFont(font);
    showLabel->setGeometry(QRect(1160, 440, 150, 60));

    // 起点：
    QLabel * srcText = new QLabel(this);
    font.setPointSize(18);
    srcText->setFont(font);
    srcText->setText(QString("起点："));
    srcText->setFixedSize(60, 20);
    srcText->move(1050, 150);
    // 终点：
    QLabel * destText = new QLabel(this);
    font.setPointSize(18);
    destText->setFont(font);
    destText->setText(QString("终点："));
    destText->setFixedSize(60, 20);
    destText->move(1050, 210);
    // 途经点：
    QLabel * passText = new QLabel(this);
    font.setPointSize(18);
    passText->setFont(font);
    passText->setText(QString("途经点："));
    passText->setFixedSize(60, 20);
    passText->move(1050, 270);

    // 起点输入栏
    font.setPointSize(16);
    QLineEdit * srcLine = new QLineEdit(this);
    srcLine->setFixedSize(260, 40);
    srcLine->setFont(font);
    srcLine->move(1120, 140);
    srcLine->setAttribute(Qt::WA_MacShowFocusRect, 0); // 取消掉获得焦点后的蓝色框
    srcLine->setStyleSheet("QLineEdit{padding-left:10px;}"); // 内边距
    // 终点输入栏
    QLineEdit * destLine = new QLineEdit(this);
    destLine->setFixedSize(260, 40);
    destLine->move(1120, 200);
    destLine->setFont(font);
    destLine->setAttribute(Qt::WA_MacShowFocusRect, 0); // 取消掉获得焦点后的蓝色框
    destLine->setStyleSheet("QLineEdit{padding-left:10px};"); // 内边距
    // 途经点输入栏
    QLineEdit * passLine = new QLineEdit(this);
    passLine->setFixedSize(260, 40);
    passLine->move(1120, 260);
    passLine->setFont(font);
    passLine->setPlaceholderText("可选，默认为空");
    passLine->setAttribute(Qt::WA_MacShowFocusRect, 0); // 取消掉获得焦点后的蓝色框
    passLine->setStyleSheet("QLineEdit{padding-left:10px};"); // 内边距


    // 查询的按钮
    QPushButton * searchBtn = new QPushButton(this);
    searchBtn->setText("查询");
    searchBtn->setFixedSize(80, 40);
    searchBtn->move(1042, 370);
    font.setPointSize(20);
    searchBtn->setFont(font);

    // 暂停 继续的按钮
    QPushButton * stopBtn = new QPushButton(this);
    stopBtn->setText("暂停");
    stopBtn->setFixedSize(80, 40);
    stopBtn->move(1152, 370);
    font.setPointSize(20);
    stopBtn->setFont(font);
//    stopBtn->setDisabled(true);

    // 更改计划 按钮
    QPushButton * changeBtn = new QPushButton(this);
    changeBtn->setText("更改计划");
    changeBtn->setFixedSize(110, 40);
    changeBtn->move(1262, 370);
    font.setPointSize(20);
    changeBtn->setFont(font);


    // 周围信息 大栏目 放QTable
    QTableWidget * showAround = new QTableWidget(5, 3, this);
    showAround->setGeometry(QRect(1050, 500, 340, 236));
    showAround->setHorizontalHeaderLabels(QStringList() << "名称" << "距离" << "点击查询");
    showAround->setEditTriggers(QAbstractItemView::NoEditTriggers); // 用户不可编辑
    showAround->setSelectionMode(QAbstractItemView::NoSelection); //设置为不可选中单元格
    showAround->setColumnWidth(0, 130);
    showAround->setColumnWidth(1, 100);
    showAround->setColumnWidth(2, 93);
    showAround->setRowHeight(0, 42);
    showAround->setRowHeight(1, 42);
    showAround->setRowHeight(2, 42);
    showAround->setRowHeight(3, 42);
    showAround->setRowHeight(4, 43);
    // 设置表头文字的格式
    QTableWidgetItem *columnHeaderItem0 = showAround->horizontalHeaderItem(0); //获得水平方向表头的Item对象
    font.setPointSize(16);
    columnHeaderItem0->setFont(font);
//    columnHeaderItem0->setTextColor(QColor(200,111,30)); //设置文字颜色
    QTableWidgetItem *columnHeaderItem1 = showAround->horizontalHeaderItem(1); //获得水平方向表头的Item对象
    font.setPointSize(16);
    columnHeaderItem1->setFont(font);
    QTableWidgetItem *columnHeaderItem2 = showAround->horizontalHeaderItem(2); //获得水平方向表头的Item对象
    font.setPointSize(16);
    columnHeaderItem2->setFont(font);

    // 具体路径按钮 * 5，查看当前点到周围点的信息
    font.setPointSize(16);
    QPushButton * showAroundPath_1 = new QPushButton(this);
    showAroundPath_1->setFixedSize(90, 42);
    showAroundPath_1->setText("具体路径");
    showAroundPath_1->setFont(font);
    showAround->setCellWidget(0, 2, showAroundPath_1);
    showAroundPath_1->setDisabled(true); // 先关闭查询到附近点路径的功能
    // 按钮2，显示到第2个点的路径
    QPushButton * showAroundPath_2 = new QPushButton(this);
    showAroundPath_2->setFixedSize(90, 42);
    showAroundPath_2->setText("具体路径");
    showAroundPath_2->setFont(font);
    showAround->setCellWidget(1, 2, showAroundPath_2);
    showAroundPath_2->setDisabled(true); // 先关闭查询到附近点路径的功能
    // 按钮3，显示到第3个点的路径
    QPushButton * showAroundPath_3 = new QPushButton(this);
    showAroundPath_3->setFixedSize(90, 42);
    showAroundPath_3->setText("具体路径");
    showAroundPath_3->setFont(font);
    showAround->setCellWidget(2, 2, showAroundPath_3);
    showAroundPath_3->setDisabled(true); // 先关闭查询到附近点路径的功能
    // 按钮4，显示到第4个点的路径
    QPushButton * showAroundPath_4 = new QPushButton(this);
    showAroundPath_4->setFixedSize(90, 42);
    showAroundPath_4->setText("具体路径");
    showAroundPath_4->setFont(font);
    showAround->setCellWidget(3, 2, showAroundPath_4);
    showAroundPath_4->setDisabled(true); // 先关闭查询到附近点路径的功能
    // 按钮5，显示到第5个点的路径
    QPushButton * showAroundPath_5 = new QPushButton(this);
    showAroundPath_5->setFixedSize(90, 42);
    showAroundPath_5->setText("具体路径");
    showAroundPath_5->setFont(font);
    showAround->setCellWidget(4, 2, showAroundPath_5);
    showAroundPath_5->setDisabled(true); // 先关闭查询到附近点路径的功能

//    showTime->setFrameStyle(QFrame::Panel);
//    showTime->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);

    // 文字 “选择查询方案”
    QLabel * chooseText = new QLabel(this);
    font.setPointSize(18);
    chooseText->setFont(font);
    chooseText->setText("选择查询方案：");
    chooseText->setGeometry(QRect(1050, 325, 115, 30));

    // 选择方案的下拉菜单
    QComboBox * choosePlan = new QComboBox(this);
    choosePlan->setGeometry(QRect(1210, 325, 110, 30));
    choosePlan->addItem("最短距离");
    choosePlan->addItem("最短时间");
    choosePlan->addItem("途经最短");
    choosePlan->addItem("交通工具");
    choosePlan->insertSeparator(0);
    choosePlan->insertSeparator(2);
    choosePlan->insertSeparator(4);
    choosePlan->insertSeparator(6);
    choosePlan->insertSeparator(8);
    font.setPointSize(16);
    choosePlan->setFont(font);
    choosePlan->setStyleSheet("QComboBox{border:0px;}"
                              "QComboBox QAbstractItemView::item{height:40px;}"); //下拉选项高度
//                              "QComboBox::down-arrow{image:url(:/icon/arrowdown);}" //下拉箭头
                              //"QComboBox::drop-down{border:0px;}"); //下拉按钮


    // 显示当前时间
    QLabel * showTime = new QLabel(this);
    font.setPointSize(28);
    showTime->setFont(font);
    showTime->setGeometry(QRect(35, 760, 280, 30));
    showTime->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter); // 文字居中
    QTimer * timerNow = new QTimer(this);
    timerNow->start(1000);
    connect(timerNow, &QTimer::timeout, [=](){
        QDateTime current_date_time = QDateTime::currentDateTime();
        QString current_date = current_date_time.toString("yyyy.MM.dd ddd hh:mm:ss");
//        QString current_date = current_date_time.toString("hh:mm");
        showTime->setText(current_date);
    });


    // 人的当前位置初始化到界外
    this->curPosition.x = -1000;
    this->curPosition.y = -1000;


/* ------------------------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------------------------ */

    // *********核心模块*********
    QTimer *timer = new QTimer(this);

    // 点击查询按钮，通过dijkstra返回一个路径
    connect(searchBtn, &QPushButton::clicked, [=](){
        // 进行初始化，清空三个vector
        this->pathAll.clear();
        this->pointsAround.clear();
        this->curToAroundPath.clear();
        update();

        if (srcLine->text() != "" && destLine->text() != "") // 有输入再执行
        {
            if (insideMap->castInside.contains(srcLine->text()) && insideMap->castInside.contains(destLine->text()))
            {
                if (srcLine->text() != destLine->text())
                {
                    this->changeFlag = 0;
                    QString src = srcLine->text();
                    QString dest = destLine->text();
                    QString pass = passLine->text();
                    qDebug() << src;
                    qDebug() << dest;
                    qDebug() << pass;
                    // 方案：最短距离
                    if (choosePlan->currentText() == "最短距离")
                    {
                        this->navigationStrategy = MinDistance;
                        this->pathAll = insideMap->dijkstra(src, dest, MinDistance, INSIDEMAP);
                    }
                    // 方案：途径某点的最短距离
                    else if (choosePlan->currentText() == "途经最短")
                    {
                        this->navigationStrategy = MinDistance;
                        // 起点->途径点
                        this->pathAll = insideMap->dijkstra(src, pass, MinDistance, INSIDEMAP);
                        // 途经点->终点
                        QVector<path> ans = insideMap->dijkstra(pass, dest, MinDistance, INSIDEMAP);
                        // 起点->途径点->终点
                        for (int i = 0; i < ans.size(); i ++)
                        {
                            this->pathAll.push_back(ans[i]);
                        }
                    }
                    // 更新人当前点的信息
                    this->curPosition.pathId = 0;
                    this->curPosition.x = this->pathAll[0].start.x;
                    this->curPosition.y = this->pathAll[0].start.y;

                    update();

                    // 同时开启查询到附近点路径的功能
                    showAroundPath_1->setEnabled(true);
                    showAroundPath_2->setEnabled(true);
                    showAroundPath_3->setEnabled(true);
                    showAroundPath_4->setEnabled(true);
                    showAroundPath_5->setEnabled(true);

                    // 启动计时器 以前500
                    timer->start(100);
                }
                else
                {
                    QMessageBox::warning(this, "警告", "起点和终点相同，请重新输入!");
                }
                }
            else
            {
                QMessageBox::warning(this, "警告", "查询的点不存在，请重新输入!");
            }
        }
        else
        {
            QMessageBox::warning(this, "警告", "请输入起点和终点!");
        }
    });


    // 上面timer计时器到时，更新一次人的位置
    connect(timer, &QTimer::timeout, [=](){
        // 速度值
        int velocity = 15;
        // 根据不同 导航策略 选择 不同的速度
        if (this->navigationStrategy == MinDistance)
            velocity *= 1;
        else if (this->navigationStrategy == MinTime)
            velocity = (int)(velocity * this->pathAll[this->curPosition.pathId].crowd);
        else if (this->navigationStrategy == MinBike)
            velocity *= 3;
        else
            ;

        if (this->pathAll[this->curPosition.pathId].flag == 1) // 当前边水平
        {
            if (this->pathAll[this->curPosition.pathId].direction == 1) // 左->右
            {
                this->curPosition.x += velocity;
                if (this->curPosition.x > this->pathAll[this->curPosition.pathId].end.x) // 如果超出了当前边
                {
                    this->curPosition.pathId++;
                    if(this->curPosition.pathId < this->pathAll.size()) // 还有下一条边
                    {
                        if(this->pathAll[this->curPosition.pathId].flag == 1) // 下一条边是水平
                        {
                            if(this->pathAll[this->curPosition.pathId].direction == 1) // 左->右，位置不用变
                                ;
                            else // 右->左，折回来
                                this->curPosition.x -= 2 * (this->curPosition.x - this->pathAll[this->curPosition.pathId - 1].end.x);
                        }
                        else // 下一条边是垂直
                        {
                            // 转为 下->上
                            if(this->pathAll[this->curPosition.pathId].direction == 1)
                                this->curPosition.y -= (this->curPosition.x - this->pathAll[this->curPosition.pathId - 1].end.x);
                            else // 转为 上->下
                                this->curPosition.y += (this->curPosition.x - this->pathAll[this->curPosition.pathId - 1].end.x);
                            // x坐标应该是上一条边终点的x
                            this->curPosition.x = this->pathAll[this->curPosition.pathId - 1].end.x;
                        }
                    }
                    else // 是最后一条边了
                        this->curPosition.x = this->pathAll[this->curPosition.pathId - 1].end.x;
                }
            }
            else // 右->左
            {
                this->curPosition.x -= velocity;
                if (this->curPosition.x < this->pathAll[this->curPosition.pathId].end.x) // 超出了当前边
                {
                    this->curPosition.pathId++;
                    if(this->curPosition.pathId < this->pathAll.size()) // 还有下一条边
                    {
                        if(this->pathAll[this->curPosition.pathId].flag == 1) // 下一条边水平
                        {
                            if(this->pathAll[this->curPosition.pathId].direction == 1) // 左->右，折回来
                                this->curPosition.x += 2 * (this->pathAll[this->curPosition.pathId - 1].end.x - this->curPosition.x);
                            else // 右->左，位置不用变了
                                ;
                        }
                        else
                        {
                            // 转为 下->上
                            if(this->pathAll[this->curPosition.pathId].direction == 1)
                                this->curPosition.y -= (this->pathAll[this->curPosition.pathId - 1].end.x - this->curPosition.x);
                            else // 转为 上->下
                                this->curPosition.y += (this->pathAll[this->curPosition.pathId - 1].end.x - this->curPosition.x);
                            // x坐标应该是上一条边终点的x
                            this->curPosition.x = this->pathAll[this->curPosition.pathId - 1].end.x;
                        }
                    }
                    else // 是最后一条边了
                        this->curPosition.x = this->pathAll[this->curPosition.pathId - 1].end.x;
                }
            }
        }
        else // 当前边垂直
        {
            if (this->pathAll[this->curPosition.pathId].direction == 1) // 下->上
            {
                this->curPosition.y -= velocity;
                if (this->curPosition.y < this->pathAll[this->curPosition.pathId].end.y) // 超出了当前边
                {
                    this->curPosition.pathId++;
                    if(this->curPosition.pathId < this->pathAll.size()) // 如果还有下一条边
                    {
                        if(this->pathAll[this->curPosition.pathId].flag == 1) // 转为水平边
                        {
                            // 转为 左->右
                            if(this->pathAll[this->curPosition.pathId].direction == 1)
                                this->curPosition.x += (this->pathAll[this->curPosition.pathId - 1].end.y - this->curPosition.y);
                            else // 转为 右->左
                                this->curPosition.x -= (this->pathAll[this->curPosition.pathId - 1].end.y - this->curPosition.y);
                            // y坐标应该是上一条边终点的y坐标
                            this->curPosition.y = this->pathAll[this->curPosition.pathId - 1].end.y;
                        }
                        else // 垂直边
                        {
                            if(this->pathAll[this->curPosition.pathId].direction == 1) // 下->上
                                ;
                            else
                                this->curPosition.y += 2 * (this->pathAll[this->curPosition.pathId - 1].end.y - this->curPosition.y);
                        }
                    }
                    else // 没有下一条边了
                        this->curPosition.y = this->pathAll[this->curPosition.pathId - 1].end.y;
                }
            }
            else // 上->下
            {
                this->curPosition.y += velocity;
                if (this->curPosition.y > this->pathAll[this->curPosition.pathId].end.y) // 超出了当前边
                {
                    this->curPosition.pathId++;
                    if(this->curPosition.pathId < this->pathAll.size()) // 还有下一条边
                    {
                        if(this->pathAll[this->curPosition.pathId].flag == 1) // 转为水平边
                        {
                            // 转为 左->右
                            if(this->pathAll[this->curPosition.pathId].direction == 1)
                                this->curPosition.x += (this->curPosition.y - this->pathAll[this->curPosition.pathId - 1].end.y);
                            else if(this->pathAll[this->curPosition.pathId].direction == 0)// 转为 右->左
                                this->curPosition.x -= (this->curPosition.y - this->pathAll[this->curPosition.pathId - 1].end.y);
                            this->curPosition.y = this->pathAll[this->curPosition.pathId - 1].end.y;
                        }
                        else if(this->pathAll[this->curPosition.pathId].flag == 0) // 垂直边
                        {
                            if(this->pathAll[this->curPosition.pathId].direction == 1) // 下->上
                                this->curPosition.y -= 2 * (this->curPosition.y - this->pathAll[this->curPosition.pathId - 1].end.y);
                            else
                                ;
                        }
                    }
                    else // 没有下一条边
                        this->curPosition.y = this->pathAll[this->curPosition.pathId - 1].end.y;
                }
            }
        }

        // ******很关键，到达终点要及时关闭计时器******
        if(this->curPosition.x == this->pathAll[this->pathAll.size() - 1].end.x)
            if(this->curPosition.y == this->pathAll[this->pathAll.size() - 1].end.y)
            {
                qDebug() << "1";
                timer->stop();
            }
        if(this->curPosition.pathId == this->pathAll.size())
        {
            qDebug() << "2";
            timer->stop();
        }

        // 更新人当前的位置
        update();

//        // 如果要换场景
//        if (this->curPosition.x == 744 && this->curPosition.y == 274 && this->changeFlag)
//        {
//            this->changeFlag = 0;
//            campusTransferMode = BenbuToShahe;
//            this->middleMap = new MainWindow;
//            QTimer::singleShot(1000, this, [=](){
//                this->middleMap->setGeometry(this->geometry());
//                this->middleMap->show();
//                this->hide();
//            });
//        }

        // 目的：实时更改策略 和 查询附近点的功能
        // 加入“当前位置”这个点
        point tmpPoint;
        tmpPoint.id = 23;
        tmpPoint.name = "当前位置";
        tmpPoint.x = this->curPosition.x;
        tmpPoint.y = this->curPosition.y;
        insideMap->castInside["当前位置"] = 23;
        insideMap->p_inside[23] = tmpPoint;
        // 初始化“当前位置”点到其他所有点的距离为inf
        for (int i = 0; i < 25; i ++)
        {
            insideMap->e_inside[23][i].dis = inf;
            insideMap->e_inside[23][i].timeDis = inf;
            insideMap->e_inside[23][i].bikeDis = inf;
        }
        for (int i = 0; i < 25; i ++)
        {
            insideMap->e_inside[i][23].dis = inf;
            insideMap->e_inside[i][23].timeDis = inf;
            insideMap->e_inside[i][23].bikeDis = inf;
        }
        // 到自己为 0
        insideMap->e_inside[23][23].dis = 0;
        insideMap->e_inside[23][23].timeDis = 0;
        insideMap->e_inside[23][23].bikeDis = 0;

        // 下面加入两条边
        int startId, endId;
        // 小心数组越界的问题，一定要判断pathId是否越界
        if (this->curPosition.pathId != this->pathAll.size()) // 没越界
        {
            // startId当前边的起点
            startId = this->pathAll[this->curPosition.pathId].start.id;
            // endId当前边的终点
            endId = this->pathAll[this->curPosition.pathId].end.id;

            insideMap->e_inside[23][startId].tongxing = 1;//通行情况
            insideMap->e_inside[23][startId].dir = this->pathAll[this->curPosition.pathId].flag;//水平竖直
            if(insideMap->e_inside[23][startId].dir == 1) //水平
                insideMap->e_inside[23][startId].dis = insideMap->p_inside[23].x - insideMap->p_inside[startId].x;//距离
            else // 垂直
                insideMap->e_inside[23][startId].dis = insideMap->p_inside[23].y - insideMap->p_inside[startId].y;//距离
            insideMap->e_inside[23][startId].crowd = insideMap->e_inside[startId][endId].crowd;
            insideMap->e_inside[23][startId].bicycle = insideMap->e_inside[startId][endId].bicycle;//自行车


            insideMap->e_inside[23][endId].tongxing = 1;//通行情况
            insideMap->e_inside[23][endId].dir = this->pathAll[this->curPosition.pathId].flag;//水平竖直
            if(insideMap->e_inside[23][endId].dir == 1) //水平
                insideMap->e_inside[23][endId].dis = insideMap->p_inside[23].x - insideMap->p_inside[endId].x;//距离
            else
                insideMap->e_inside[23][endId].dis = insideMap->p_inside[23].y - insideMap->p_inside[endId].y;//距离
            insideMap->e_inside[23][endId].crowd = insideMap->e_inside[startId][endId].crowd;
            insideMap->e_inside[23][endId].bicycle = insideMap->e_inside[startId][endId].bicycle;//自行车
        }
        else // 如果已经到了最后一条边，就不能访问pathId了，需要-1
        {
            // startId当前边的起点
            startId = this->pathAll[this->curPosition.pathId - 1].start.id;
            // endId当前边的终点
            endId = this->pathAll[this->curPosition.pathId - 1].end.id;

            insideMap->e_inside[23][startId].tongxing = 1;//通行情况
            insideMap->e_inside[23][startId].dir = this->pathAll[this->curPosition.pathId - 1].flag;//水平竖直
            if(insideMap->e_inside[23][startId].dir == 1) //水平
                insideMap->e_inside[23][startId].dis = insideMap->p_inside[23].x - insideMap->p_inside[startId].x;//距离
            else
                insideMap->e_inside[23][startId].dis = insideMap->p_inside[23].y - insideMap->p_inside[startId].y;//距离
            insideMap->e_inside[23][startId].crowd = insideMap->e_inside[startId][endId].crowd;
            insideMap->e_inside[23][startId].bicycle = insideMap->e_inside[startId][endId].bicycle;//自行车


            insideMap->e_inside[23][endId].tongxing = 1;//通行情况
            insideMap->e_inside[23][endId].dir = this->pathAll[this->curPosition.pathId - 1].flag;//水平竖直
            if(insideMap->e_inside[23][endId].dir == 1) //水平
                insideMap->e_inside[23][endId].dis = insideMap->p_inside[23].x - insideMap->p_inside[endId].x;//距离
            else
                insideMap->e_inside[23][endId].dis = insideMap->p_inside[23].y - insideMap->p_inside[endId].y;//距离
            insideMap->e_inside[23][endId].crowd = insideMap->e_inside[startId][endId].crowd;
            insideMap->e_inside[23][endId].bicycle = insideMap->e_inside[startId][endId].bicycle;//自行车
        }

        // 边可能是负数，改为正
        if(insideMap->e_inside[23][startId].dis < 0)
            insideMap->e_inside[23][startId].dis = -insideMap->e_inside[23][startId].dis;
        if(insideMap->e_inside[23][endId].dis < 0)
            insideMap->e_inside[23][endId].dis = -insideMap->e_inside[23][endId].dis;
        // 无向图设置
        insideMap->e_inside[startId][23].dis = insideMap->e_inside[23][startId].dis;
        insideMap->e_inside[endId][23].dis = insideMap->e_inside[23][endId].dis;

        // 设置 timeDis
        insideMap->e_inside[startId][23].timeDis = (int)(insideMap->e_inside[startId][23].dis / insideMap->e_inside[startId][23].crowd);
        insideMap->e_inside[23][startId].timeDis = insideMap->e_inside[startId][23].timeDis;

        insideMap->e_inside[endId][23].timeDis = (int)(insideMap->e_inside[endId][23].dis / insideMap->e_inside[endId][23].crowd);
        insideMap->e_inside[23][endId].timeDis = insideMap->e_inside[endId][23].timeDis;

        // 设置 bikeDis
        if (insideMap->e_inside[23][endId].bicycle == true)
            insideMap->e_inside[23][endId].bikeDis = insideMap->e_inside[23][endId].dis;
        else
            insideMap->e_inside[23][endId].bikeDis = inf;
        insideMap->e_inside[endId][23].bikeDis = insideMap->e_inside[23][endId].bikeDis;

        if (insideMap->e_inside[23][startId].bicycle == true)
            insideMap->e_inside[23][startId].bikeDis = insideMap->e_inside[23][startId].dis;
        else
            insideMap->e_inside[23][startId].bikeDis = inf;
        insideMap->e_inside[startId][23].bikeDis = insideMap->e_inside[23][startId].bikeDis;

         // 获取周围最近的5个点的信息，放入this->pointsAround
        this->pointsAround = insideMap->findPointsAround(tmpPoint, INSIDEMAP);

        // 将this->pointsAround放入表格中
        for (int i = 0; i < 5; i ++)
        {
            // 1.名称
            QTableWidgetItem *item = new QTableWidgetItem(this->pointsAround[i].name);
//            item->setBackgroundColor(QColor(0,60,10));
//            item->setTextColor(QColor(200,111,100));
            QFont font1("Helvetica");
            font1.setPointSize(18);
            item->setFont(font1);
            showAround->setItem(i, 0, item);
            // 2.距离
            QString str = QString::number(this->pointsAround[i].dis * 0.23, 'f', 1); // 保留一位小数
            QTableWidgetItem *item1 = new QTableWidgetItem(str + "m");
            item1->setFont(font1);
            showAround->setItem(i, 1, item1);
        }
    });


    // 点击查看到周围点的路径
    // 到第一个点的路径
    connect(showAroundPath_1, &QPushButton::clicked, [=](){
        qDebug() << "获得点击啦啦啦";
        if (showAroundPath_1->text() == "具体路径")
        {
            // 查看的是 当前位置点---this->pointsAround里面第一个点
            this->curToAroundPath = insideMap->dijkstra("当前位置", this->pointsAround[0].name, MinDistance, INSIDEMAP);
            // 停止原来的路径
            timer->stop();
            // 如果到终点了，文字就不能变为继续，否则又会起动计时器，发生数组越界
            if(this->curPosition.x == this->pathAll[this->pathAll.size() - 1].end.x && this->curPosition.y == this->pathAll[this->pathAll.size() - 1].end.y)
                ;
            else // 文字变成“继续”原来的路径
                showAroundPath_1->setText("继续");
            // 在图上画出来这条路径
            update();
        }
        else if (showAroundPath_1->text() == "继续")
        {
            // 文字变回“查看路径”
            showAroundPath_1->setText("具体路径");
            // 如果暂停按钮让当前暂停下来，那么同样改变暂停按钮的状态，下面同理
            if (stopBtn->text() == "继续")
                stopBtn->setText("暂停");
            // 继续原来的路径
            timer->start();
            // 清空当前点到周围点的vector
            this->curToAroundPath.clear();
            update();
        }
    });
    // 到第二个点的路径
    connect(showAroundPath_2, &QPushButton::clicked, [=](){
        if (showAroundPath_2->text() == "具体路径")
        {
            this->curToAroundPath = insideMap->dijkstra("当前位置", this->pointsAround[1].name, MinDistance, INSIDEMAP);
            timer->stop();
            // 如果到终点了，就不能变为继续，否则又回起动计时器
            if(this->curPosition.x == this->pathAll[this->pathAll.size() - 1].end.x && this->curPosition.y == this->pathAll[this->pathAll.size() - 1].end.y)
                ;
            else
                showAroundPath_2->setText("继续");
            update();
        }
        else if (showAroundPath_2->text() == "继续")
        {
            showAroundPath_2->setText("具体路径");
            if (stopBtn->text() == "继续")
                stopBtn->setText("暂停");
            timer->start();
            this->curToAroundPath.clear();
            update();
        }
    });
    // 到第三个点的路径
    connect(showAroundPath_3, &QPushButton::clicked, [=](){
        if (showAroundPath_3->text() == "具体路径")
        {
            this->curToAroundPath = insideMap->dijkstra("当前位置", this->pointsAround[2].name, MinDistance, INSIDEMAP);
            timer->stop();
            // 如果到终点了，就不能变为继续，否则又回起动计时器
            if(this->curPosition.x == this->pathAll[this->pathAll.size() - 1].end.x && this->curPosition.y == this->pathAll[this->pathAll.size() - 1].end.y)
                ;
            else
                showAroundPath_3->setText("继续");
            update();
        }
        else if (showAroundPath_3->text() == "继续")
        {
            showAroundPath_3->setText("具体路径");
            if (stopBtn->text() == "继续")
                stopBtn->setText("暂停");
            timer->start();
            this->curToAroundPath.clear();
            update();
        }
    });
    // 到第四个点的路径
    connect(showAroundPath_4, &QPushButton::clicked, [=](){
        if (showAroundPath_4->text() == "具体路径")
        {
            this->curToAroundPath = insideMap->dijkstra("当前位置", this->pointsAround[3].name, MinDistance, INSIDEMAP);
            timer->stop();
            // 如果到终点了，就不能变为继续，否则又回起动计时器
            if(this->curPosition.x == this->pathAll[this->pathAll.size() - 1].end.x && this->curPosition.y == this->pathAll[this->pathAll.size() - 1].end.y)
                ;
            else
                showAroundPath_4->setText("继续");
            update();
        }
        else if (showAroundPath_4->text() == "继续")
        {
            showAroundPath_4->setText("具体路径");
            if (stopBtn->text() == "继续")
                stopBtn->setText("暂停");
            timer->start();
            this->curToAroundPath.clear();
            update();
        }
    });
    // 到第五个点的路径
    connect(showAroundPath_5, &QPushButton::clicked, [=](){
        if (showAroundPath_5->text() == "具体路径")
        {
            this->curToAroundPath = insideMap->dijkstra("当前位置", this->pointsAround[4].name, MinDistance, INSIDEMAP);
            timer->stop();
            // 如果到终点了，就不能变为继续，否则又回起动计时器
            if(this->curPosition.x == this->pathAll[this->pathAll.size() - 1].end.x && this->curPosition.y == this->pathAll[this->pathAll.size() - 1].end.y)
                ;
            else
                showAroundPath_5->setText("继续");
            update();
        }
        else if (showAroundPath_5->text() == "继续")
        {
            showAroundPath_5->setText("具体路径");
            if (stopBtn->text() == "继续")
                stopBtn->setText("暂停");
            timer->start();
            this->curToAroundPath.clear();
            update();
        }
    });

    // 点击暂停按钮，红点不再更新
    connect(stopBtn, &QPushButton::clicked, [=](){
        if (stopBtn->text() == "暂停")
        {
            timer->stop();
            // 如果到达终点，文字不能变为继续了，否则再次启动定时器会越界
            if(this->curPosition.x == this->pathAll[this->pathAll.size() - 1].end.x && this->curPosition.y == this->pathAll[this->pathAll.size() - 1].end.y)
                ;
            else // 文字变成“继续”原来的路径
                stopBtn->setText("继续");
        }
        else
        {
            timer->start();
            stopBtn->setText("暂停");
        }
    });

    // 点击更改计划按钮，将人当前位置作为起点
    connect(changeBtn, &QPushButton::clicked, [=](){
        timer->stop();
        srcLine->setText("当前位置");
    });
}

void inside::paintEvent(QPaintEvent *)
{
    // 设置画家
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing); // 抗锯齿

    // 设置校园地图
    QPixmap pix;
    if (insideMode == CLASSROOM)
        pix.load(":/res/classroom.jpeg");
    else
        pix.load(":/res/dorm.jpeg");
    painter.drawPixmap(0, 150, pix.width(), pix.height(), pix);

    // 分割线
    painter.drawLine(1050, 430, 1390, 430);

    // 设置pen和brush
    QPen pen(Qt::red);
    pen.setWidth(8);
    painter.setPen(pen);
    QBrush brush(Qt::red);
    painter.setBrush(brush);

    if (this->navigationStrategy == MinBike)
    {
        if (this->insideMap != NULL)
        {
            for (int i = 1; i <= 74; i++)
                for (int j = 1; j <= 74; j++)
                {
                    if (this->insideMap->e_benbu[i][j].bicycle == false)
                    {
                        // 当前路径所在边的起点 （*0.6是因为图片缩小了0.6倍，+28是偏移）
                        painter.drawEllipse(QPoint(this->insideMap->p_inside[i].x, this->insideMap->p_inside[i].y + 150), 5, 5);
                        // 当前路径所在边的终点
                        painter.drawEllipse(QPoint(this->insideMap->p_inside[j].x, this->insideMap->p_inside[j].y + 150), 5, 5);
                        // 当前路径所在边 起点到终点的连线
                        painter.drawLine(QPoint(this->insideMap->p_inside[i].x, this->insideMap->p_inside[i].y + 150), QPoint(this->insideMap->p_inside[j].x, this->insideMap->p_inside[j].y + 150));
                    }
                }
        }
    }
    else
        ;

    // 设置pen和brush
    pen.setColor(Qt::blue);
    pen.setWidth(6);
    painter.setPen(pen);
    brush.setColor(Qt::blue);
    painter.setBrush(brush);

    if (this->navigationStrategy == MinTime)
    {
        for (int i = 0; i < this->pathAll.size(); i ++)
        {
            // 根据当前所在边的拥挤度选择颜色
            if (this->pathAll[i].crowd >= 0.5)
            {
                pen.setColor(Qt::green);
                brush.setColor(Qt::green);
                painter.setPen(pen);
                painter.setBrush(brush);
                // 当前路径所在边的起点 （*0.6是因为图片缩小了0.6倍，+28是偏移）
                painter.drawEllipse(QPoint(this->pathAll[i].start.x, this->pathAll[i].start.y + 150), 5, 5);
                // 当前路径所在边的终点
                painter.drawEllipse(QPoint(this->pathAll[i].end.x, this->pathAll[i].end.y + 150), 5, 5);
                // 当前路径所在边 起点到终点的连线
                painter.drawLine(QPoint(this->pathAll[i].start.x, this->pathAll[i].start.y + 150), QPoint(this->pathAll[i].end.x, this->pathAll[i].end.y + 150));
            }
            else if (this->pathAll[i].crowd >= 0.2)
            {
                pen.setColor(Qt::yellow);
                brush.setColor(Qt::yellow);
                painter.setPen(pen);
                painter.setBrush(brush);
                // 当前路径所在边的起点 （*0.6是因为图片缩小了0.6倍，+28是偏移）
                painter.drawEllipse(QPoint(this->pathAll[i].start.x, this->pathAll[i].start.y + 150), 5, 5);
                // 当前路径所在边的终点
                painter.drawEllipse(QPoint(this->pathAll[i].end.x, this->pathAll[i].end.y + 150), 5, 5);
                // 当前路径所在边 起点到终点的连线
                painter.drawLine(QPoint(this->pathAll[i].start.x, this->pathAll[i].start.y + 150), QPoint(this->pathAll[i].end.x, this->pathAll[i].end.y + 150));
            }
            else if (this->pathAll[i].crowd >= 0)
            {
                pen.setColor(Qt::red);
                brush.setColor(Qt::red);
                painter.setPen(pen);
                painter.setBrush(brush);
                // 当前路径所在边的起点 （*0.6是因为图片缩小了0.6倍，+28是偏移）
                painter.drawEllipse(QPoint(this->pathAll[i].start.x, this->pathAll[i].start.y + 150), 5, 5);
                // 当前路径所在边的终点
                painter.drawEllipse(QPoint(this->pathAll[i].end.x, this->pathAll[i].end.y + 150), 5, 5);
                // 当前路径所在边 起点到终点的连线
                painter.drawLine(QPoint(this->pathAll[i].start.x, this->pathAll[i].start.y + 150), QPoint(this->pathAll[i].end.x, this->pathAll[i].end.y + 150));
            }
        }
    }
    else
    {
        // 用蓝线将整条路径画出
        for (int i = 0; i < this->pathAll.size(); i ++)
        {
            // 当前路径所在边的起点 （*0.6是因为图片缩小了0.6倍，+28是偏移）
            painter.drawEllipse(QPoint(this->pathAll[i].start.x, this->pathAll[i].start.y + 150), 5, 5);
            // 当前路径所在边的终点
            painter.drawEllipse(QPoint(this->pathAll[i].end.x, this->pathAll[i].end.y + 150), 5, 5);
            // 当前路径所在边 起点到终点的连线
            painter.drawLine(QPoint(this->pathAll[i].start.x, this->pathAll[i].start.y + 150), QPoint(this->pathAll[i].end.x, this->pathAll[i].end.y + 150));
        }
    }


    // 用红点表示人的当前位置
    painter.setPen(Qt::red);
    painter.setBrush(Qt::red);
    painter.drawEllipse(QPoint(this->curPosition.x, this->curPosition.y + 150), 6, 6);


    // 用绿线画出人到周围点的路径
    pen.setColor(Qt::red);
    pen.setWidth(4);
    painter.setPen(pen);
    brush.setColor(Qt::red);
    painter.setBrush(brush);
    for (int i = 0; i < this->curToAroundPath.size(); i ++)
    {
        // 起点
        painter.drawEllipse(QPoint(this->curToAroundPath[i].start.x, this->curToAroundPath[i].start.y + 150), 4, 4);
        // 终点
        painter.drawEllipse(QPoint(this->curToAroundPath[i].end.x, this->curToAroundPath[i].end.y + 150), 4, 4);
        // 起点到终点的连线
        painter.drawLine(QPoint(this->curToAroundPath[i].start.x, this->curToAroundPath[i].start.y + 150), QPoint(this->curToAroundPath[i].end.x, this->curToAroundPath[i].end.y + 150));
    }
}
