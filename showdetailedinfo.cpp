#include "showdetailedinfo.h"
#include <QTableWidget>
#include <QHeaderView>
#include <QFont>
#include <QLabel>
#include <QDebug>
#include <QTimer>
#include <QPixmap>
#include <QPushButton>
#include <QPainter>
#include <QMessageBox>
#include "map.h"

extern int showDetailedInfoKind;
extern int chooseCanteen;
extern int chooseTransportMethod;

showDetailedInfo::showDetailedInfo(QWidget *parent) : QMainWindow(parent)
{
    if (showDetailedInfoKind == CANTEEN)
    {
        this->setFixedSize(420, 300);
        this->setWindowTitle("餐厅人数实时查询");
        canteen[1] = 30;
        canteen[2] = 100;
        canteen[3] = 90;
        canteen[4] = 56;
        canteen[5] = 18;
        canteen[6] = 120;
        canteen[7] = 50;
        canteen[8] = 40;
        canteen[9] = 76;
        canteen[10] = 32;
        QTableWidget * showCanteen = new QTableWidget(10, 3, this);
        showCanteen->setGeometry(QRect(0, 0, 410, 300));
        showCanteen->setHorizontalHeaderLabels(QStringList() << "食堂" << "人数" << "选择");
        showCanteen->setEditTriggers(QAbstractItemView::NoEditTriggers); // 用户不可编辑
        showCanteen->setSelectionMode(QAbstractItemView::NoSelection); //设置为不可选中单元格
        showCanteen->setColumnWidth(0, 160);
        showCanteen->setColumnWidth(1, 120);
        showCanteen->setColumnWidth(2, 100);
        showCanteen->setRowHeight(0, 42);
        showCanteen->setRowHeight(1, 42);
        showCanteen->setRowHeight(2, 42);
        showCanteen->setRowHeight(3, 42);
        showCanteen->setRowHeight(4, 42);
        showCanteen->setRowHeight(5, 42);
        showCanteen->setRowHeight(6, 42);
        showCanteen->setRowHeight(7, 42);
        showCanteen->setRowHeight(8, 42);
        showCanteen->setRowHeight(9, 42);
        // 设置表头文字的格式
        QTableWidgetItem *columnHeaderItem0 = showCanteen->horizontalHeaderItem(0); //获得水平方向表头的Item对象
        QFont font;
        font.setPointSize(16);
        columnHeaderItem0->setFont(font);
    //    columnHeaderItem0->setTextColor(QColor(200,111,30)); //设置文字颜色
        QTableWidgetItem *columnHeaderItem1 = showCanteen->horizontalHeaderItem(1); //获得水平方向表头的Item对象
        font.setPointSize(16);
        columnHeaderItem1->setFont(font);
        QTableWidgetItem *columnHeaderItem2 = showCanteen->horizontalHeaderItem(2); //获得水平方向表头的Item对象
        font.setPointSize(16);
        columnHeaderItem2->setFont(font);

        QPushButton * selectCanteen_1 = new QPushButton(this);
        selectCanteen_1->setFixedSize(90, 42);
        selectCanteen_1->setText("选择");
        selectCanteen_1->setFont(font);
        showCanteen->setCellWidget(0, 2, selectCanteen_1);

        QPushButton * selectCanteen_2 = new QPushButton(this);
        selectCanteen_2->setFixedSize(90, 42);
        selectCanteen_2->setText("选择");
        selectCanteen_2->setFont(font);
        showCanteen->setCellWidget(1, 2, selectCanteen_2);

        QPushButton * selectCanteen_3 = new QPushButton(this);
        selectCanteen_3->setFixedSize(90, 42);
        selectCanteen_3->setText("选择");
        selectCanteen_3->setFont(font);
        showCanteen->setCellWidget(2, 2, selectCanteen_3);

        QPushButton * selectCanteen_4 = new QPushButton(this);
        selectCanteen_4->setFixedSize(90, 42);
        selectCanteen_4->setText("选择");
        selectCanteen_4->setFont(font);
        showCanteen->setCellWidget(3, 2, selectCanteen_4);

        QPushButton * selectCanteen_5 = new QPushButton(this);
        selectCanteen_5->setFixedSize(90, 42);
        selectCanteen_5->setText("选择");
        selectCanteen_5->setFont(font);
        showCanteen->setCellWidget(4, 2, selectCanteen_5);

        QPushButton * selectCanteen_6 = new QPushButton(this);
        selectCanteen_6->setFixedSize(90, 42);
        selectCanteen_6->setText("选择");
        selectCanteen_6->setFont(font);
        showCanteen->setCellWidget(5, 2, selectCanteen_6);

        QPushButton * selectCanteen_7 = new QPushButton(this);
        selectCanteen_7->setFixedSize(90, 42);
        selectCanteen_7->setText("选择");
        selectCanteen_7->setFont(font);
        showCanteen->setCellWidget(6, 2, selectCanteen_7);

        QPushButton * selectCanteen_8 = new QPushButton(this);
        selectCanteen_8->setFixedSize(90, 42);
        selectCanteen_8->setText("选择");
        selectCanteen_8->setFont(font);
        showCanteen->setCellWidget(7, 2, selectCanteen_8);

        QPushButton * selectCanteen_9 = new QPushButton(this);
        selectCanteen_9->setFixedSize(90, 42);
        selectCanteen_9->setText("选择");
        selectCanteen_9->setFont(font);
        showCanteen->setCellWidget(8, 2, selectCanteen_9);

        QPushButton * selectCanteen_10 = new QPushButton(this);
        selectCanteen_10->setFixedSize(90, 42);
        selectCanteen_10->setText("选择");
        selectCanteen_10->setFont(font);
        showCanteen->setCellWidget(9, 2, selectCanteen_10);

        for (int i = 0; i < 5; i ++)
        {
            QString str = QString("学生食堂 %1楼").arg(i + 1);
            // 1.名称
            QTableWidgetItem *item = new QTableWidgetItem(str);
            QFont font1("Helvetica");
            font1.setPointSize(18);
            item->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
            item->setFont(font1);
            showCanteen->setItem(i, 0, item);
            // 2.距离
            str = QString::number(canteen[i + 1]); // 保留一位小数
            QTableWidgetItem *item1 = new QTableWidgetItem(str + "人");
            item1->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
            item1->setFont(font1);
            showCanteen->setItem(i, 1, item1);
        }
        for (int i = 5; i < 10; i ++)
        {
            QString str = QString("教工食堂 %1楼").arg(i - 4);
            // 1.名称
            QTableWidgetItem *item = new QTableWidgetItem(str);
            QFont font1("Helvetica");
            font1.setPointSize(18);
            item->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
            item->setFont(font1);
            showCanteen->setItem(i, 0, item);
            // 2.距离
            str = QString::number(canteen[i + 1]); // 保留一位小数
            QTableWidgetItem *item1 = new QTableWidgetItem(str + "人");
            item1->setFont(font1);
            item1->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
            showCanteen->setItem(i, 1, item1);
        }

        QTimer::singleShot(1000, this, [=](){
            QMessageBox::information(this, "导航提示", "实时：现在学生食堂5楼人数较少，建议前往！");
        });

        connect(selectCanteen_1, &QPushButton::clicked, [=](){
            chooseCanteen = 1;
            emit detailedInfoBack();
            this->close();
        });
        connect(selectCanteen_2, &QPushButton::clicked, [=](){
            chooseCanteen = 2;
            emit detailedInfoBack();
            this->close();
        });
        connect(selectCanteen_3, &QPushButton::clicked, [=](){
            chooseCanteen = 3;
            emit detailedInfoBack();
            this->close();
        });
        connect(selectCanteen_4, &QPushButton::clicked, [=](){
            chooseCanteen = 4;
            emit detailedInfoBack();
            this->close();
        });
        connect(selectCanteen_5, &QPushButton::clicked, [=](){
            chooseCanteen = 5;
            emit detailedInfoBack();
            this->close();
        });
        connect(selectCanteen_6, &QPushButton::clicked, [=](){
            chooseCanteen = 6;
            emit detailedInfoBack();
            this->close();
        });
        connect(selectCanteen_7, &QPushButton::clicked, [=](){
            chooseCanteen = 7;
            emit detailedInfoBack();
            this->close();
        });
        connect(selectCanteen_8, &QPushButton::clicked, [=](){
            chooseCanteen = 8;
            emit detailedInfoBack();
            this->close();
        });
        connect(selectCanteen_9, &QPushButton::clicked, [=](){
            chooseCanteen = 9;
            emit detailedInfoBack();
            this->close();
        });
        connect(selectCanteen_10, &QPushButton::clicked, [=](){
            chooseCanteen = 10;
            emit detailedInfoBack();
            this->close();
        });

    }
    else if (showDetailedInfoKind == SCHOOLBUS)
    {
        this->setFixedSize(340, 290);
        this->setWindowTitle("校车时间查询");

        schoolBusList.push_back("09:00");
        schoolBusList.push_back("10:00");
        schoolBusList.push_back("11:00");
        schoolBusList.push_back("14:00");
        schoolBusList.push_back("15:00");
        schoolBusList.push_back("16:00");

        QTableWidget * showSchoolBus = new QTableWidget(6, 2, this);
        showSchoolBus->setGeometry(QRect(0, 0, 340, 290));
        showSchoolBus->setHorizontalHeaderLabels(QStringList() << "本部始发" << "沙河始发");
        showSchoolBus->setEditTriggers(QAbstractItemView::NoEditTriggers); // 用户不可编辑
        showSchoolBus->setSelectionMode(QAbstractItemView::NoSelection); //设置为不可选中单元格
        showSchoolBus->setColumnWidth(0, 160);
        showSchoolBus->setColumnWidth(1, 160);
        showSchoolBus->setRowHeight(0, 42);
        showSchoolBus->setRowHeight(1, 42);
        showSchoolBus->setRowHeight(2, 42);
        showSchoolBus->setRowHeight(3, 42);
        showSchoolBus->setRowHeight(4, 42);
        showSchoolBus->setRowHeight(5, 42);
        // 设置表头文字的格式
        QTableWidgetItem *columnHeaderItem0 = showSchoolBus->horizontalHeaderItem(0); //获得水平方向表头的Item对象
        QFont font;
        font.setPointSize(16);
        columnHeaderItem0->setFont(font);
        QTableWidgetItem *columnHeaderItem1 = showSchoolBus->horizontalHeaderItem(1); //获得水平方向表头的Item对象
        font.setPointSize(16);
        columnHeaderItem1->setFont(font);

        for (int i = 0; i < 6; i ++)
        {
            QString str = this->schoolBusList[i];
            // 1.名称
            QTableWidgetItem *item = new QTableWidgetItem(str);
            QFont font1("Helvetica");
            font1.setPointSize(18);
            item->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
            item->setFont(font1);
            showSchoolBus->setItem(i, 0, item);

            QTableWidgetItem *item1 = new QTableWidgetItem(str);
            item1->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
            item1->setFont(font1);
            showSchoolBus->setItem(i, 1, item1);
        }
    }
    else if (showDetailedInfoKind == BUS)
    {
        this->setFixedSize(420, 250);
        this->setWindowTitle("公交车信息");
    }
    else if (showDetailedInfoKind == CHOOSETRANSPORTMETHOD)
    {
        qDebug() << "打开了";
        this->setFixedSize(420, 250);
        this->setWindowTitle("选择交通方式");
        QFont font;
        font.setFamily("华文宋体");
        font.setPointSize(24);
        QLabel * header = new QLabel(this);
        header->setFont(font);
        QString str = QString("请选择您跨校区的交通方式");
        header->setText(str);
        header->setGeometry(60, 20, 400, 50);

        QPushButton * schoolBusBtn = new QPushButton(this);
        schoolBusBtn->setGeometry(30, 120, 160, 80);
        schoolBusBtn->setFont(font);
        schoolBusBtn->setText("校车");
        connect(schoolBusBtn, &QPushButton::clicked, [=](){
            chooseTransportMethod = 1;
            emit detailedInfoBack();
            this->close();
        });
        QPushButton * busBtn = new QPushButton(this);
        busBtn->setGeometry(220, 120, 160, 80);
        busBtn->setFont(font);
        busBtn->setText("公交车");
        connect(busBtn, &QPushButton::clicked, [=](){
            chooseTransportMethod = 2;
            emit detailedInfoBack();
            this->close();
        });
    }
}

void showDetailedInfo::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    if (showDetailedInfoKind == BUS)
    {
        QPixmap pix;
        pix.load(":res/bus.png");
        painter.drawPixmap(0, 0, 420, 250, pix);
    }
}
