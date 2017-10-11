#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H
#include <QDebug>
#include <QPair>
#include <QList>
#include <QtMath>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <string>
#include <complex>
#include <QVector>

class Polynomial {
public:
    using coef_type = long double;
    using exp_type = long int;
    Polynomial();
    Polynomial(coef_type, exp_type);
    explicit Polynomial(const QString&);

    using term_type = QPair<coef_type, exp_type>;
    using polynomial_type = QList<term_type>;
    using value_type = double;
    exp_type max_allowed_exp = std::numeric_limits<int>::max();
    exp_type min_allowed_exp = std::numeric_limits<int>::min();

    coef_type max_allowed_coef = std::numeric_limits<double>::max();
    coef_type min_allowed_coef = std::numeric_limits<double>::lowest();

    exp_type smallest_exp = 0;
    exp_type biggest_exp = 0;

    void insertTerm(const term_type& newTerm); //OK
    void insertTerm(coef_type coef, exp_type exp);
    QString printPretty() const; //OK
    bool isZero() const; //OK
    size_t size() const; //OK
    coef_type get(exp_type) const;
    Polynomial operator+(const Polynomial& rhs) const; //OK
    Polynomial operator-(const Polynomial& rhs) const; //OK
    Polynomial operator*(const term_type& term) const; //OK
    Polynomial operator*(const Polynomial& rhs) const; //OK
    Polynomial operator/(const Polynomial& rhs);
    Polynomial operator-() const;
    Polynomial& operator+=(const Polynomial& rhs); //OK
    Polynomial& operator-=(const Polynomial& rhs); //OK
    Polynomial& operator*=(const Polynomial& rhs); //OK
    Polynomial& operator/=(const Polynomial& rhs);
    bool operator==(const Polynomial& rhs); //OK
    bool operator!=(const Polynomial& rhs); //OK
    Polynomial derivative(); //OK
    Polynomial poly_power(int pow_num);
    value_type eval(value_type x); //OK
    friend QDebug operator<<(QDebug out, const Polynomial& operand); //OK
    friend QDebug operator>>(QDebug in, Polynomial& operand);
    polynomial_type::iterator begin(); //OK
    polynomial_type::iterator end(); //OK
    polynomial_type::const_iterator begin() const; //OK
    polynomial_type::const_iterator end() const; //OK
    void setID(const QString&);
    QString& getID();
    void setNum(const QString&);
    QString &getNum();
    QString toText();
    void loadText(QString line);

private:
    polynomial_type polynomial;
    template <typename F>
    Polynomial operator_undetermine(const Polynomial& rhs, F _operator) const;
    //由于我编程所用的的编译环境 Apple LLVM version 9.0.0 (clang-900.0.37) on MacOS 10.12对long double 支持不好，所以只能直接使用int, double
    std::map<int, std::complex<double> > fft_eval_y(std::vector<std::complex<double> > A, int n, int sign) const;
    QString m_ID;
    QString m_NUM;
};

QDebug operator<<(QDebug out, const Polynomial& operand);
#endif // POLYNOMIAL_H
