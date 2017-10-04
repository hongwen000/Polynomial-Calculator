#include "mainwindow.h"
#include <QApplication>
#include <iostream>
#include <string>
#include "polynomial.h"
#include "token.h"

int main(int argc, char *argv[])
{

    Polynomial poly;
    poly.insertTerm(qMakePair(-1, 2));
    poly.insertTerm(qMakePair(10, 92));
    qDebug() << poly.derivative().derivative().derivative();
    Polynomial poly2;
    poly2.insertTerm(qMakePair(-2, 2));
    poly2.insertTerm(qMakePair(8, 3));
    qDebug() << (poly * poly2);
    std::string str = "1 + 1";
    scan(str);
    yyparse();
    return 0;
   // QApplication a(argc, argv);
   // MainWindow w;
   // w.show();

   // return a.exec();
}
