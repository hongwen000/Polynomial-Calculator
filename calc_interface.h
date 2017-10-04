#ifndef CALC_INTERFACE_H
#define CALC_INTERFACE_H
#include <QPair>
#include <QString>

class calc_interface
{
public:
    calc_interface();
    static QPair<QString, QString> process_input_main(const QString& main_input);
    static QString process_input_x(const QString& input_x);
};

#endif // CALC_INTERFACE_H
