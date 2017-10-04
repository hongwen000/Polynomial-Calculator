#include "mainwindow.h"
#include "about.h"
#include "ui_mainwindow.h"
#include <iostream>
#include "calc_interface.h"
#include <Qt>
using namespace std;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actiongy_triggered()
{
    About* about = new About();
    about->show();
}

void MainWindow::on_actiontc_triggered()
{
    this->close();
}



void MainWindow::on_pushButton_input_clicked()
{
    input_main = ui->lineEdit_main->text();
    qDebug() << input_main;
    auto ret = calc_interface::process_input_main(input_main);
    ui->output_main->append(ret.first);
    if(ui->listWidget_variables->findItems(ret.second, Qt::MatchExactly).size() == 0)
        ui->listWidget_variables->addItem(ret.second);
    ui->lineEdit_main->selectAll();
}

void MainWindow::on_pushButton_eval_clicked()
{
    input_x = ui->lineEdit_x->text();
    qDebug() << input_x;
    auto ret = calc_interface::process_input_x(input_x);
    ui->output_value->setText(ret);
}
