#include "calc_interface.h"
#include "mainwindow.h"
#include "polynomial.h"
#include "token.h"
#include <QApplication>
#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    qDebug() << Polynomial(QString("x 2 1 1 0")).poly_power(2).printPretty();
    return a.exec();
}
