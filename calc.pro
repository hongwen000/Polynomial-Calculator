#-------------------------------------------------
#
# Project created by QtCreator 2017-09-29T23:14:37
#
#-------------------------------------------------

QT       += core gui charts
CONFIG += c++14
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = calc
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    about.cpp \
    calc_interface.cpp \
    polynomial.cpp \
    draw_figure_window.cpp

HEADERS += \
        mainwindow.h \
    about.h \
    calc_interface.h \
    polynomial.h \
    draw_figure_window.h

FORMS += \
        mainwindow.ui \
    about.ui \
    draw_figure_window.ui


!macx {
LIBS += -lfl -ly
DISTFILES += \
    calc.ico
    RC_ICONS = calc.ico
}

macx: {
ICON = calc.icns
}
