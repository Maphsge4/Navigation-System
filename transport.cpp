#include "transport.h"
#include <QPainter>
#include <QTimer>
#include <QDebug>

transport::transport(QWidget *parent) : QMainWindow(parent)
{
    connect(Shahe, &campusShahe::showMiddleMap, [=](){
        qDebug() << "收到信号";
            this->setGeometry(Shahe->geometry());
            Shahe->close();
            this->show();
        QTimer::singleShot(3000, this, [=](){
            Benbu->setGeometry(Shahe->geometry());
            this->close();
            Benbu->show();
        });
    });
}

void transport::paintEvent(QPaintEvent *)
{
    // 设置画家
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing); // 抗锯齿

    // 设置校园地图
    QPixmap pix;
    pix.load(":/res/transport.png");
    painter.drawPixmap(28, 62, pix.width(), pix.height(), pix);

}
