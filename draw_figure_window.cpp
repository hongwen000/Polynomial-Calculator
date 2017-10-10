#include "draw_figure_window.h"
#include "calc_interface.h"
#include "mainwindow.h"
#include "polynomial.h"
#include "ui_draw_figure_window.h"
#include <QMessageBox>
#include <algorithm>
#include <vector>

draw_figure_window::draw_figure_window(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::draw_figure_window)
{
    ui->setupUi(this);
    chartView = new QChartView();
    chartView->setRenderHint(QPainter::Antialiasing);
    ui->gridLayout->addWidget(chartView, 1, 0);
    ui->gridLayout->setRowStretch(0, 1);
    ui->gridLayout->setRowStretch(0, 10);
    setWindowTitle("作图窗口");
}

draw_figure_window::~draw_figure_window()
{
    delete ui;
}

void draw_figure_window::on_pushButton_clicked()
{
    QLineSeries* series = new QLineSeries();
    Polynomial::value_type xmin = ui->lineEdit_xmin->text().toDouble();
    Polynomial::value_type xmax = ui->lineEdit_xmax->text().toDouble();
    if (xmax <= xmin || (xmin == 0 && xmax == 0)) {
        QMessageBox::critical(this, "Error", "x的范围输入有误", QMessageBox::Ok);
        return;
    }
    int x_chart_size = chartView->size().width() < 400 ? 400 : chartView->size().width();
    int pixel_per_x = 1;
    int x_sample_number = x_chart_size / pixel_per_x;
    Polynomial::value_type dx = (xmax - xmin) / x_sample_number;
    Polynomial::value_type x = xmin;
    try {
        calc_interface::doParse(((MainWindow*)parent())->get_main_input());
    } catch (...) {
        QMessageBox::critical(this, "Error", "多项式输入有误，建议在主窗口中点\"输入\"验证正确后再作图", QMessageBox::Ok);
        return;
    }
    if (calc_interface::statement_type != ASSIGN_STM || calc_interface::parser_error.synax_error || calc_interface::parser_error.undefined_reference || calc_interface::scanner_error.illegal_token) {
        QMessageBox::critical(this, "Error", "多项式输入有误，建议在主窗口中点\"输入\"验证正确后再作图", QMessageBox::Ok);
        return;
    }
    std::vector<Polynomial::value_type> vec_x(x_sample_number);
    std::vector<Polynomial::value_type> vec_y(x_sample_number);
    Polynomial::value_type y;
    for (int i = 0; i < x_sample_number; ++i) {
        try {
            y = calc_interface::assign_stm_info.parsed_input.eval(x);
        } catch (...) {
            QMessageBox::critical(this, "Error", "计算中出现错误，可能出现非法运算", QMessageBox::Ok);
            return;
        }

        vec_y[i] = y;
        vec_x[i] = x;
        x += dx;
    }
    for (int i = 0; i < x_sample_number; ++i) {
        series->append(vec_x[i], vec_y[i]);
    }
    QChart* chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(series);
    chart->createDefaultAxes();
    chartView->setChart(chart);
}
