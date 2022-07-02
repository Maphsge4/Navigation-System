#ifndef SHOWDETAILEDINFO_H
#define SHOWDETAILEDINFO_H

#include <QMainWindow>
#include <QVector>
#include <QString>

class showDetailedInfo : public QMainWindow
{
    Q_OBJECT
public:
    explicit showDetailedInfo(QWidget *parent = nullptr);

    int canteen[11];

    QVector<QString> schoolBusList;

    void paintEvent(QPaintEvent *);
signals:
    void detailedInfoBack();
};

#endif // SHOWDETAILEDINFO_H
