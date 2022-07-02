#ifndef MYPUSHBUTTON_H
#define MYPUSHBUTTON_H

#include <QWidget>
#include <QPushButton>

class MyPushButton : public QPushButton
{
    Q_OBJECT
public:
    MyPushButton(QString ImgPath);

    // 弹起函数
    void zoomUp();
    void zoomDown();

signals:

};

#endif // MYPUSHBUTTON_H
