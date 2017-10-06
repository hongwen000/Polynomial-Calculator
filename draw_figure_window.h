#ifndef DRAW_FIGURE_WINDOW_H
#define DRAW_FIGURE_WINDOW_H

#include <QtCharts>
#include <QMainWindow>

namespace Ui {
class draw_figure_window;
}

class draw_figure_window : public QMainWindow
{
    Q_OBJECT

public:
    explicit draw_figure_window(QWidget *parent = 0);
    ~draw_figure_window();

private slots:

    void on_pushButton_clicked();

private:
    Ui::draw_figure_window *ui;
    QChartView* chartView;
};

#endif // DRAW_FIGURE_WINDOW_H
