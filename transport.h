#ifndef TRANSPORT_H
#define TRANSPORT_H

#include <QMainWindow>
#include <QPainter>
#include "campusbenbu.h"
#include "campusshahe.h"

class transport : public QMainWindow
{
    Q_OBJECT
public:
    explicit transport(QWidget *parent = nullptr);

    void paintEvent(QPaintEvent *);

    campusBenbu * Benbu = new campusBenbu;
    campusShahe * Shahe = new campusShahe;
signals:

};

#endif // TRANSPORT_H
