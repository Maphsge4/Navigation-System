#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QTimer>
#include <QPainter>
#include <QDebug>
#include <QLabel>
#include <QDateTime>
#include <QFile>
#include <QIODevice>
#include <QTextStream>
#include <QPalette>
#include "campusbenbu.h"
#include "campusshahe.h"
#include "inside.h"
#include "mypushbutton.h"

// 如果要跨校区的话，记录终点名
QString finalName;
// 跨校区模式
int campusTransferMode = DefaultRoute;
// 显示宿舍，还是教学楼
int insideMode;
//
int showDetailedInfoKind;
//
int chooseCanteen;
//
int chooseTransportMethod;
//
extern QFile myFile;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    qDebug() << "in main campusTransferMode:" << campusTransferMode;
    qDebug() << "now finalName:" << finalName;

    ui->setupUi(this);
    // 设置固定的大小
    this->setFixedSize(1400, 840);
    // 窗口标题
    this->setWindowTitle("导航系统");

    // 字体部分
    QFont font;
    font.setFamily("华文新魏");

    // ****让label显示黑色****
    QPalette pe;
    pe.setColor(QPalette::WindowText, Qt::black);


    font.setPointSize(42);
    // 大标题 “北邮沙河校区导航”
    QLabel * header = new QLabel(this);
    font.setBold(true);
    header->setFont(font);
    QString str = QString("北邮校园导览系统");
    header->setText(str);
//    header->setPalette(pe);
    header->setFixedSize(350, 60);
    header->move(this->width() * 0.5 - header->width() * 0.5, 0);

    // 显示当前时间
    font.setFamily("华文宋体");
    QLabel * showTime = new QLabel(this);
    font.setPointSize(24);
    font.setBold(true);
    showTime->setFont(font);
//    showTime->setPalette(pe);
    showTime->setGeometry(QRect(35, 800, 250, 30));
    showTime->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter); // 文字居中
    QTimer * timerNow = new QTimer(this);
    timerNow->start(1000);
    connect(timerNow, &QTimer::timeout, [=](){
        QDateTime current_date_time = QDateTime::currentDateTime();
        QString current_date = current_date_time.toString("yyyy.MM.dd ddd hh:mm:ss");
        showTime->setText(current_date);
    });

    // 打开 沙河校区
    MyPushButton * shaheBtn = new MyPushButton(":res/shaheLogo4.png");
    shaheBtn->setParent(this);
    shaheBtn->move(540, 73);
    connect(shaheBtn, &QPushButton::clicked, [=](){
        shaheBtn->zoomDown();
        shaheBtn->zoomUp();
        shahe = new campusShahe;
        QTimer::singleShot(500, this, [=](){
            shahe->setGeometry(this->geometry());
            this->hide();
            shahe->show();
        });
    });

    // 打开 本部
    MyPushButton * benbuBtn = new MyPushButton(":res/benbuLogo4.png");
    benbuBtn->setParent(this);
    benbuBtn->move(210, 530);
    connect(benbuBtn, &QPushButton::clicked, [=](){
        benbuBtn->zoomDown();
        benbuBtn->zoomUp();
        benbu = new campusBenbu;
        QTimer::singleShot(500, this, [=](){
            benbu->setGeometry(this->geometry());
            this->hide();
            benbu->show();
        });
    });

    // 设置路径
    this->mainScenePath.push_back({1300, 1098});
    this->mainScenePath.push_back({1300, 1065});
    this->mainScenePath.push_back({1394, 1065});
    this->mainScenePath.push_back({1394, 963});
    this->mainScenePath.push_back({1295, 793});
    this->mainScenePath.push_back({1217, 656});
    this->mainScenePath.push_back({1115, 488});
    this->mainScenePath.push_back({1056, 381});
    this->mainScenePath.push_back({978, 307});
    this->mainScenePath.push_back({949, 200});
    this->mainScenePath.push_back({1048, 160});
    this->mainScenePath.push_back({1048, 125});

    this->mainSceneCurPoint = {-1000, -1000};

    QTimer * timer = new QTimer(this);
    static int index = 0; // 序号

    if (campusTransferMode == ShaheToBenbu) // 沙河 -> 本部
    {
        QTextStream textStream(&myFile);
        QDateTime current_date_time = QDateTime::currentDateTime();
        QString current_date = current_date_time.toString("yyyy.MM.dd ddd hh:mm:ss");
        textStream << current_date;
        QString str = QString("   用户搭乘交通工具，正在前往本部。\n").toUtf8();
        textStream << str;
        textStream.flush();

        index = 11;
        this->mainSceneCurPoint = this->mainScenePath[index];
        timer->start(1000);
        // 从沙河走到本部
        connect(timer, &QTimer::timeout, [=](){
            this->mainSceneCurPoint = this->mainScenePath[--index];
            update();
            if (index == 0)
            {
                timer->stop();
                index = 0;
            }
        });
        // 到达本部，打开本部界面
        QTimer::singleShot(12000, this, [=](){
            benbu = new campusBenbu;
            benbu->setGeometry(this->geometry());
            this->hide();
            benbu->show();
        });
    }
    else if (campusTransferMode == BenbuToShahe) // 本部 -> 沙河
    {
        QTextStream textStream(&myFile);
        QDateTime current_date_time = QDateTime::currentDateTime();
        QString current_date = current_date_time.toString("yyyy.MM.dd ddd hh:mm:ss");
        textStream << current_date;
        QString str = QString("   用户搭乘交通工具，正在前往沙河。\n").toUtf8();
        textStream << str;
        textStream.flush();
        index = 0;
        this->mainSceneCurPoint = this->mainScenePath[index];
        timer->start(1000);

        connect(timer, &QTimer::timeout, [=](){
            this->mainSceneCurPoint = this->mainScenePath[++index];
            update();
            if (index == 11)
            {
                timer->stop();
                index = 0;
            }
        });

        QTimer::singleShot(12000, this, [=](){
            shahe = new campusShahe;
            shahe->setGeometry(this->geometry());
            this->hide();
            shahe->show();
        });
    }
}

// 析构函数
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent*)
{
    // 设置画家
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing); // 抗锯齿

//    // *****设置背景颜色为白色*****
//    painter.setBrush(Qt::white);
//    painter.drawRect(this->rect());
    // 设置校园地图
    QPixmap pix;
    pix.load(":/res/main.JPG");
//    painter.drawPixmap(30, 70, 1340, 720, pix);
    painter.drawPixmap(30, 70, pix.width() * 0.5, pix.height() * 0.52, pix);

    // 设置pen和brush
    QPen pen(Qt::red);
    pen.setWidth(8);
    painter.setPen(pen);
    QBrush brush(Qt::red);
    painter.setBrush(brush);

//    qDebug() << "in paintEvent now curPoint x:" << this->mainSceneCurPoint.x;
//    qDebug() << "in paintEvent now curPoint y:" << this->mainSceneCurPoint.y;

    painter.drawEllipse(QPoint(this->mainSceneCurPoint.x * 0.5 + 30, this->mainSceneCurPoint.y * 0.52 + 70), 6, 6);
}

