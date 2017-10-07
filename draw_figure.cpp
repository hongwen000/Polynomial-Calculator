#include "draw_figure.h"
#include "calc_interface.h"
#include "mainwindow.h"
#include "polynomial.h"
#include "ui_draw_figure.h"
#include <algorithm>
#include <vector>

draw_figure::draw_figure(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::draw_figure)
{
    ui->setupUi(this);
}

draw_figure::~draw_figure()
{
    delete ui;
}

void draw_figure::on_pushButton_confirm_clicked()
{
    QLineSeries* series = new QLineSeries();
    Polynomial::value_type xmin = ui->lineEdit_xmin->text().toDouble();
    Polynomial::value_type xmax = ui->lineEdit_xmax->text().toDouble();
    if (xmax <= xmin || (xmin == 0 && xmax == 0))
        return;
    int x_chart_size = 400;
    int pixel_per_x = 1;
    int x_sample_number = x_chart_size / pixel_per_x;
    Polynomial::value_type dx = (xmax - xmin) / x_sample_number;
    Polynomial::value_type x = xmin;
    calc_interface::doParse(((MainWindow*)parent())->get_main_input());
    std::vector<Polynomial::value_type> vec_x(x_sample_number);
    std::vector<Polynomial::value_type> vec_y(x_sample_number);
    Polynomial::value_type y;
    for (int i = 0; i < x_sample_number; ++i) {
        y = calc_interface::assign_stm_info.parsed_input.eval(x);
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
    QChartView* chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->show();
}
