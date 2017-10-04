#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H
#include <QPair>
#include <QList>
#include <string>
#include <stdexcept>
#include <QDebug>
#include <iostream>
#include <functional>
#include <QtMath>

class Polynomial
{
public:
    Polynomial();

    using coef_type = int;
    using exp_type = int;
    using term_type = QPair<coef_type, exp_type>;
    using polynomial_type = QList<term_type>;
    using value_type = double;
    exp_type max_allowed_exp = INT_MAX;
    exp_type min_allowed_exp = INT_MIN;

    exp_type max_allowed_coef = INT_MAX;
    exp_type min_allowed_coef = INT_MIN;

    exp_type smallest_exp = 0;
    exp_type biggest_exp = 0;

    void insertTerm(const term_type& newTerm);					//OK
    QString printPretty() const;								//OK
    bool isZero() const;										//OK
    size_t size() const;										//OK
    Polynomial operator +(const Polynomial& rhs) const;			//OK
    Polynomial operator -(const Polynomial& rhs) const;			//OK
    Polynomial operator *(const term_type& term) const;			//OK
    Polynomial operator *(const Polynomial& rhs) const;			//OK
    Polynomial operator /(const Polynomial& rhs);
    Polynomial& operator +=(const Polynomial& rhs);				//OK
    Polynomial& operator -=(const Polynomial& rhs);				//OK
    Polynomial& operator *=(const Polynomial& rhs);				//OK
    Polynomial& operator /=(const Polynomial& rhs);
    bool operator ==(const Polynomial& rhs);					//OK
    bool operator !=(const Polynomial& rhs);					//OK
    Polynomial derivative();									//OK
    value_type eval(value_type x);								//OK
    friend QDebug operator<<(QDebug out, const Polynomial& operand); //OK
    friend QDebug operator>>(QDebug in, Polynomial& operand);
    polynomial_type::iterator begin();								//OK
    polynomial_type::iterator end();								//OK
    polynomial_type::const_iterator begin() const;					//OK
    polynomial_type::const_iterator end() const;					//OK

private:

    polynomial_type polynomial;
    template<typename F>
    Polynomial operator_undetermine(const Polynomial& rhs, F _operator) const;
};

QDebug operator<<(QDebug out, const Polynomial &operand);
#endif // POLYNOMIAL_H
