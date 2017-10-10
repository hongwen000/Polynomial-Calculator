#include "calc_interface.h"
#include <QStack>

QMap<QString, Polynomial> calc_interface::variable_table;
scanner_error_t calc_interface::scanner_error;
parser_error_t calc_interface::parser_error;
statement_type_t calc_interface::statement_type;
assign_stm_t calc_interface::assign_stm_info;
test_equal_stm_t calc_interface::test_equal_stm_info;
print_stm_t calc_interface::print_stm_info;
eval_stm_t calc_interface::eval_stm_info;

calc_interface::calc_interface()
{
}

process_result_t calc_interface::process_input_main(QString main_input, QString input_x)
{
    doParse(main_input);
    if (scanner_error.illegal_token) {
        return process_result_t(QString("illegal token at column"));
    }
    if (parser_error.synax_error) {
        return process_result_t(QString("synax error"));
    }
    if (parser_error.undefined_reference) {
        return process_result_t("undefined reference to variable " + parser_error.var_name);
    }

    if (statement_type == ASSIGN_STM) {
        QString eval_result;
        if (input_x != "")
            eval_result = QString::number(assign_stm_info.parsed_input.eval(input_x.toDouble()));
        assign_stm_info.parsed_input.setID(assign_stm_info.variable_name);
        variable_table[assign_stm_info.variable_name] = assign_stm_info.parsed_input;
        return process_result_t(assign_stm_info.parsed_input.printPretty(), assign_stm_info.variable_name, eval_result);
    } else if(statement_type == EVAL_STM) {
        QString eval_result;
        eval_result = QString::number(eval_stm_info.parsed_input.eval(eval_stm_info.x_var));
        return process_result_t(eval_stm_info.parsed_input.printPretty()  + " [x = " + QString::number(eval_stm_info.x_var) + "] = " + eval_result,"", eval_result);
    }
    else if (statement_type == TEST_EQUAL_STM) {
        if (variable_table[test_equal_stm_info.var1.getID()] == variable_table[test_equal_stm_info.var2.getID()])
            return process_result_t("true", "", "");
        else
            return process_result_t("false", "", "");
    } else
    {
        QString ret;
        if (print_stm_info.var.getID() == "all") {
            QMap<QString, Polynomial>::iterator it = variable_table.begin();
            while (it != variable_table.end()) {
                ret += it.key();
                ret += " = ";
                ret += it.value().printPretty();
                ret += '\n';
                ++it;
            }
        } else {
            ret += variable_table[print_stm_info.var.getID()].printPretty();
        }
        return process_result_t(ret, "", "");
    }
}

void calc_interface::doParse(QString main_input)
{
    reset();
    main_input = main_input.remove(QRegExp("\\s"));
    if(main_input.contains("print")){
        statement_type = PRINT_STM;

        if(!QRegExp("[a-zA-Z_]*").exactMatch(main_input)) {
            scanner_error.illegal_token = true;
            return;
        }

        auto vars = main_input.split(QRegExp("print"),QString::SkipEmptyParts);

        if(!(vars.size() == 1)) {
            parser_error.synax_error = true;
            return;
        }

        print_stm_info.var = calc_interface::getVariable(vars[0]);
    } else if(main_input.contains("'")){
        statement_type = ASSIGN_STM;
        if(!QRegExp("[a-zA-Z_']*").exactMatch(main_input)) {
            scanner_error.illegal_token = true;
            return;
        }

        auto vars = main_input.split("'",QString::SkipEmptyParts);

        if(!(vars.size() == 1)) {
            parser_error.synax_error = true;
            return;
        }
        assign_stm_info.parsed_input = calc_interface::getVariable((vars[0])).derivative();
        assign_stm_info.variable_name = "ANS";
    } else if(main_input.contains("==")) {
        statement_type = TEST_EQUAL_STM;


        if(!(QRegExp("[a-zA-Z_].*==[a-zA-Z_].*").exactMatch(main_input))) {
            scanner_error.illegal_token = true;
            return;
        }

        auto vars = main_input.split(QRegExp("=="));

        if(!(vars.size() == 2)) {
            parser_error.synax_error = true;
            return;
        }

        test_equal_stm_info.var1 = calc_interface::getVariable(vars[0]);
        test_equal_stm_info.var2 = calc_interface::getVariable(vars[1]);
    }
    else if(main_input.contains("=") && !main_input.contains("x=")) {
        statement_type = ASSIGN_STM;
        main_input.replace(QRegExp("[=\\(\\),]"), " ");

        if(!(QRegExp("[\\-a-zA-Z0-9_\\.\\s]*").exactMatch(main_input))) {
            scanner_error.illegal_token = true;
            return;
        }
        try{
        assign_stm_info.parsed_input = Polynomial(main_input);
        }catch(std::invalid_argument &e){
            if(QString(e.what()).contains("illegal token")){
                scanner_error.illegal_token = true;
            }
            else{
                parser_error.synax_error = true;
            }
        }
        assign_stm_info.variable_name = assign_stm_info.parsed_input.getID();
    }
    else if(main_input.contains("x=")){
        statement_type = EVAL_STM;
        QRegExp pattern("\\[x=(.*)\\]");
        pattern.indexIn(main_input);
        eval_stm_info.x_var = std::stod(pattern.cap(1).toStdString());
        auto vars = main_input.split(pattern, QString::SkipEmptyParts);
        if(!(vars.size() == 1)) {
            parser_error.synax_error = true;
            return;
        }
        eval_stm_info.parsed_input = calc_interface::getVariable(vars[0]);
    }
    else if (QRegExp("[a-zA-Z_+\\-*/\\(\\)]+").exactMatch(main_input)){
        enum op {
            ADD,
            SUB,
            MUL,
            DIV,
            LPAREN,
            RPAREN,
            END
        };

        QMap<QString, op> opmap = 	{{"+", ADD},
                                     {"-", SUB},
                                     {"*", MUL},
                                     {"/", DIV},
                                     {"(", LPAREN},
                                     {")", RPAREN},
                                     {"#", END}
                                    };

        const char* predence[] = {
            "LLSSSLL",
            "LLSSSLL",
            "LLLLSLL",
            "LLLLSLL",
            "SSSSSEW",
            "LLLLWLL",
            "SSSSSWE"
        };

        QRegExp opList("[+\\-*/\\(\\)\\#]");

        for(int i = 0; i != main_input.size();) {
            if(opList.exactMatch(QString(main_input[i]))) {
                main_input.insert(i, " ");
                i+=2;
                main_input.insert(i, " ");
            } else {
                ++i;
            }
        }

        QStringList tokens = main_input.split(" ", QString::SkipEmptyParts);
        tokens.insert(tokens.end(), "#");

        QStack<QString> operandStack;
        QStack<op> opStack;
        opStack.push(END);
        Polynomial ret, poly1, poly2;
        ret.setID("ANS");
        for(auto it = tokens.begin(); it != tokens.end();) {
            if(opmap.find(*it) != opmap.end()) {
                op op2 = opmap[*it];
                op op1 = opStack.top();
                auto cmp = predence[op1][op2];
                if(cmp == 'S') {
                    opStack.push(op2);
                    ++it;
                } else if(cmp == 'E') {
                    opStack.pop();
                    ++it;
                } else if(cmp == 'L') {
                    QString operand1 = operandStack.pop();
                    QString operand2 = operandStack.pop();
                    if(operand1 != "ANS")
                        poly1 = calc_interface::getVariable(operand1);
                    else
                        poly1 = ret;
                    if(operand2 != "ANS")
                        poly2 = calc_interface::getVariable(operand2);
                    else
                        poly2 = ret;
                    op operand = opStack.pop();
                    if(calc_interface::parser_error.undefined_reference){
                        break;
                    }
                    if(operand == ADD)
                        ret = poly1 + poly2;
                    else if(operand == SUB)
                        ret = poly2 - poly1;
                    else if(operand == MUL)
                        ret = poly1 * poly2;
                    else
                        ret = poly2 / poly1;
                    ret.setID("ANS");
                    operandStack.push("ANS");
                } else {
                    parser_error.synax_error = true;
                    break;
                }
            }
            else {
                operandStack.push(*it);
                ++it;
            }
        }
        assign_stm_info.parsed_input = ret;
        assign_stm_info.variable_name = "ANS";
    }
    else {
        parser_error.synax_error = true;
        return;
    }
}

Polynomial calc_interface::getVariable(const QString& name)
{
    auto ptr = variable_table.find(name);
    if(name == "all") {
        Polynomial ret;
        ret.setID("all");
        return ret;
    }
    if (ptr == variable_table.end()) {
        parser_error.undefined_reference = true;
        parser_error.var_name = name;
        return Polynomial();
    } else {
        return *ptr;
    }
}

void calc_interface::reset()
{
    assign_stm_info = assign_stm_t();
    test_equal_stm_info = test_equal_stm_t();
    print_stm_info = print_stm_t();
    scanner_error.illegal_token = false;
    scanner_error.illegal_token_pos = 0;
    parser_error.synax_error = false;
    parser_error.undefined_reference = false;
    parser_error.var_name = "";
}

process_result_t::process_result_t(const QString& _m, const QString& _v, const QString& _e)
    : main_result(_m)
    , variable_result(_v)
    , eval_result(_e)
{
}
