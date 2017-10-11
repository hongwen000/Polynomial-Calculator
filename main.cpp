#include <QApplication>
#include "mainwindow.h"
#include "polynomial.h"
int main(int argc, char* argv[])
{
    Polynomial B, C;
    B.loadText(QString("B 2 6 1 1 2 0"));
    C.loadText(QString("C 1 3 1 1 2 0"));
    qDebug() << B.printPretty();
    qDebug() << C.printPretty();
    Polynomial ret = B * C;
    qDebug() << ret.printPretty();
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
