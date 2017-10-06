#ifndef DRAW_FIGURE_H
#define DRAW_FIGURE_H

#include <QDialog>
#include<QtCharts>

namespace Ui {
class draw_figure;
}

class draw_figure : public QDialog
{
    Q_OBJECT

public:
    explicit draw_figure(QWidget *parent = 0);
    ~draw_figure();

private slots:
    void on_pushButton_confirm_clicked();

private:
    Ui::draw_figure *ui;
};

#endif // DRAW_FIGURE_H
