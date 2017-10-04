#include "calc_interface.h"

calc_interface::calc_interface()
{

}

QPair<QString, QString> calc_interface::process_input_main(const QString &main_input)
{
    return qMakePair(QString("main_result"), QString("p1"));
}

QString calc_interface::process_input_x(const QString &input_x)
{
    return "eval_result";
}
