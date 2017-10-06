#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringListModel>
#include <QDebug>
#include <QListWidgetItem>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    QString get_main_input();
    ~MainWindow();

private slots:
    void on_actiongy_triggered();

    void on_actiontc_triggered();


    void on_pushButton_input_clicked();

    void on_pushButton_eval_clicked();

    void on_listWidget_variables_itemDoubleClicked(QListWidgetItem *item);

    void on_pushButton_delete_clicked();

    void on_actionbc_triggered();

    void on_action_2_triggered();

    void on_pushButton_draw_clicked();

private:
    Ui::MainWindow *ui;
    QString input_main;
    QString input_x;
    bool toFile();
    bool loadFile();
    static constexpr int FILE_MAX_LINE = 1000;
};

#endif // MAINWINDOW_H
