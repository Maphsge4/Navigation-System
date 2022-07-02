#include "mainwindow.h"

#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QDateTime>

QFile myFile("/Users/bai/Desktop/log.txt");

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if(!myFile.open(QIODevice::WriteOnly | QIODevice::Append))
    {
        qDebug() << myFile.errorString();
    }
    QTextStream textStream(&myFile);
    QDateTime current_date_time = QDateTime::currentDateTime();
    QString current_date = current_date_time.toString("yyyy.MM.dd ddd hh:mm:ss");
    textStream << current_date;
    QString str = QString("   用户进入应用程序。\n").toUtf8();
    textStream << str;
    textStream.flush();

    MainWindow w;
    w.show();
    return a.exec();
    myFile.close();
}
