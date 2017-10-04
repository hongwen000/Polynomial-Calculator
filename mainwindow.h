#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringListModel>
#include <QDebug>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actiongy_triggered();

    void on_actiontc_triggered();


    void on_pushButton_input_clicked();

    void on_pushButton_eval_clicked();

private:
    Ui::MainWindow *ui;
    QString input_main;
    QString input_x;
};

#endif // MAINWINDOW_H
