#include "mainwindow.h"
#include "about.h"
#include "calc_interface.h"
#include "draw_figure_window.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <Qt>
#include <iostream>
using namespace std;
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("主界面");
    QWidget::setTabOrder(ui->lineEdit_main, ui->lineEdit_x);
    ui->output_value->setFocusPolicy(Qt::NoFocus);
    ui->listWidget_variables->setFocusPolicy(Qt::NoFocus);
}

QString MainWindow::get_main_input()
{
    return ui->lineEdit_main->text();
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
    try {
        input_main = ui->lineEdit_main->text();
        auto ret = calc_interface::process_input_main(input_main);
        ui->output_main->append(ret.main_result);
        if (ret.variable_result != "" && ui->listWidget_variables->findItems(ret.variable_result, Qt::MatchExactly).size() == 0)
            ui->listWidget_variables->addItem(ret.variable_result);
        ui->lineEdit_main->selectAll();
    } catch (std::out_of_range) {
        ui->output_main->append("Number out of range");
    } catch (std::invalid_argument& e) {
        ui->output_main->append(e.what());
    }
}

void MainWindow::on_pushButton_eval_clicked()
{
    try {
        input_main = ui->lineEdit_main->text();
        input_x = ui->lineEdit_x->text();
        auto ret = calc_interface::process_input_main(input_main, input_x);
        ui->output_main->append(ret.main_result + " [x = " + input_x + "] = " + ret.eval_result);
        if (ret.variable_result != "" && ui->listWidget_variables->findItems(ret.variable_result, Qt::MatchExactly).size() == 0)
            ui->listWidget_variables->addItem(ret.variable_result);
        ui->output_value->setText(ret.eval_result);
    } catch (std::out_of_range) {
        ui->output_main->append("Number out of range");
    } catch (std::invalid_argument& e) {
        ui->output_main->append(e.what());
    }
}

void MainWindow::on_listWidget_variables_itemDoubleClicked(QListWidgetItem* item)
{
    Polynomial& p = calc_interface::variable_table[item->text()];
    ui->lineEdit_main->setText(p.printPretty());
}

void MainWindow::on_pushButton_delete_clicked()
{
    auto items = ui->listWidget_variables->selectedItems();
    foreach (QListWidgetItem* item, items) {
        calc_interface::variable_table.erase(calc_interface::variable_table.find(item->text()));
        delete ui->listWidget_variables->takeItem(ui->listWidget_variables->row(item));
    }
}

bool MainWindow::toFile()
{
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::AnyFile);
    QString path = dialog.getSaveFileName(this, "Save to file", ".");
    QFile file(path);
    if (!file.open(QIODevice::Text | QIODevice::WriteOnly)) {
        return false;
    }
    QTextStream text(&file);
    text << "#MAINBEGIN" << endl;
    text << ui->output_main->document()->toRawText() << endl;
    text << "#MAINEND" << endl;
    text << "#VARBEGIN" << endl;
    QMap<QString, Polynomial>::iterator it = calc_interface::variable_table.begin();
    while (it != calc_interface::variable_table.end()) {
        text << it.key() << " " << it.value().toText() << endl;
        ++it;
    }
    text << "#VAREND" << endl;
    file.close();
    return true;
}

bool MainWindow::loadFile()
{
    QString path = QFileDialog::getOpenFileName(this, "Open file", ".");
    QFile file(path);
    if (!file.open(QIODevice::Text | QIODevice::ReadOnly)) {
        return false;
    }
    QTextStream in(&file);
    QString line = in.readLine();
    if (line != "#MAINBEGIN") {
        return false;
    }
    while (!in.atEnd()) {
        line = in.readLine();
        if (line == "#MAINEND")
            break;
        ui->output_main->append(line);
    }
    if (in.atEnd()) {
        return false;
    }
    line = in.readLine();
    if (line != "#VARBEGIN") {
        return false;
    }
    while (!in.atEnd()) {
        line = in.readLine();
        if (line == "#VAREND")
            break;
        try {
            Polynomial p(line);
            calc_interface::variable_table[QString::fromStdString(p.getID())] = p;
            if (p.getID() != "" && ui->listWidget_variables->findItems(QString::fromStdString(p.getID()), Qt::MatchExactly).size() == 0)
                ui->listWidget_variables->addItem(QString::fromStdString(p.getID()));
        } catch (...) {
            QMessageBox::critical(this, "Error", "Data file corrupted", QMessageBox::Ok);
        }
    }
    return true;
}

void MainWindow::on_actionbc_triggered()
{
    if (!toFile()) {
        QMessageBox::critical(this, "Error", "Error save data file", QMessageBox::StandardButton::Ok);
    }
}

void MainWindow::on_action_2_triggered()
{
    if (!loadFile()) {
        QMessageBox::critical(this, "Error", "Error loading data file", QMessageBox::StandardButton::Ok);
    }
}

void MainWindow::on_pushButton_draw_clicked()
{
    draw_figure_window* dialog = new draw_figure_window(this);
    dialog->show();
}
