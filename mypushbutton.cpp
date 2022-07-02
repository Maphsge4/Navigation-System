#include "mypushbutton.h"
#include <QPixmap>
#include <QPropertyAnimation>

MyPushButton::MyPushButton(QString ImgPath)
{
    QPixmap pix;
    pix.load(ImgPath);
//    this->setFixedSize(pix.width(), pix.height());
    this->setFixedSize(500, 250);
    this->setStyleSheet("QPushButton{border:0px;}");
    this->setIcon(pix);
//    this->setIconSize(QSize(pix.width(), pix.height()));
    this->setIconSize(QSize(500, 250));
}

void MyPushButton::zoomDown()
{
    // 创建对象
    QPropertyAnimation * animation = new QPropertyAnimation(this, "geometry");
    // 设置时间间隔
    animation->setDuration(360);
    // 起始位置
    animation->setStartValue(QRect(this->x(), this->y(), this->width(), this->height()));
    // 结束位置
    animation->setEndValue(QRect(this->x(), this->y() + 10, this->width(), this->height()));
    // 设置速度曲线
    animation->setEasingCurve(QEasingCurve::OutBounce);
    // 执行动画
    animation->start();
}

void MyPushButton::zoomUp()
{
    QPropertyAnimation *animation = new QPropertyAnimation(this, "geometry");
    animation->setDuration(360);
    animation->setStartValue(QRect(this->x(), this->y() + 10, this->width(), this->height()));
    animation->setEndValue(QRect(this->x(), this->y(), this->width(), this->height()));
    animation->setEasingCurve(QEasingCurve::OutBounce);
    animation->start();
}
