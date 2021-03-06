#include "polynomial.h"

Polynomial::Polynomial()
{
}

Polynomial::Polynomial(Polynomial::coef_type coef, Polynomial::exp_type exp)
{
    insertTerm(coef, exp);
}

Polynomial::Polynomial(const QString& str)
{
    QString tmp = str;
    loadText(tmp);
}

void Polynomial::insertTerm(Polynomial::coef_type coef, Polynomial::exp_type exp)
{
    insertTerm(qMakePair(coef, exp));
}

void Polynomial::insertTerm(const Polynomial::term_type& newTerm)
{
    if (newTerm.second > max_allowed_exp || newTerm.second < min_allowed_exp) {
        throw(std::invalid_argument(std::string("New term exponent ") + "exceeded limit"));
    }
    if (newTerm.first > max_allowed_coef || newTerm.first < min_allowed_coef) {
        throw(std::invalid_argument(std::string("New term coeffcient ") + "exceeded limit"));
    }
    if (newTerm.first == 0)
        return;
    for (auto& p : polynomial)
        if (p.second == newTerm.second)
            throw(std::invalid_argument(std::string("already has a term whose exponent is ") + std::to_string(newTerm.second)));
    auto it = polynomial.begin();
    for (; it != polynomial.end(); ++it) {
        if (newTerm.second > it->second)
            break;
    }
    if (newTerm.second > biggest_exp)
        biggest_exp = newTerm.second;
    if (newTerm.second < smallest_exp)
        smallest_exp = newTerm.second;
    polynomial.insert(it, newTerm);
}

QString Polynomial::printPretty() const
{
    if (this->isZero()) {
        return QString("0");
    }
    auto printPosWithSign = [](auto num) {
        if (num > 0 && num != 1)
            return " + " + QString::number(static_cast<double>(num));
        else if (num == 1) {
            return QString(" + ");
        } else if (num == -1) {
            return QString(" - ");
        } else if (num == 0)
            return QString("");
        else
            return " - " + QString::number(static_cast<double>(0 - num));
    };

    auto printFirst = [](auto num) {
        if (num == -1) {
            return QString("-");
        } else if (num == 1)
            return QString("");
        else
            return QString::number(static_cast<double>(num));
    };
    auto printExp = [](exp_type exp) {
        if (exp == 0)
            return QString("");
        else if (exp == 1)
            return QString("x");
        else if (exp > 1)
            return QString("x^") + QString::number(exp);
        else
            return QString("x^") + "(" + QString::number(exp) + ")";
    };

    QString ret;
    for (auto it = polynomial.begin(); it != polynomial.end(); ++it) {
        if (it->first == 0)
            continue;
        if (it == polynomial.begin()) {
            if (it->first == 1 && it->second == 0)
                ret += "1";
            else if (it->first == -1 && it->second == 0)
                ret += "-1";
            else
                ret += printFirst(it->first) + printExp(it->second);

        } else {
            if (it->first == 1 && it->second == 0)
                ret += " + 1";
            else if (it->first == -1 && it->second == 0)
                ret += " - 1";
            else
                ret += printPosWithSign(it->first) + printExp(it->second);
        }
    }
    return ret;
}

bool Polynomial::isZero() const
{
    return polynomial.empty();
}

size_t Polynomial::size() const
{
    return polynomial.size();
}

Polynomial::coef_type Polynomial::get(Polynomial::exp_type N) const
{
    for (auto t : polynomial)
        if (t.second == N)
            return t.first;
    return 0;
}

Polynomial Polynomial::operator+(const Polynomial& rhs) const
{
    return operator_undetermine(rhs, std::plus<coef_type>());
}

Polynomial Polynomial::operator-(const Polynomial& rhs) const
{
    return operator_undetermine(rhs, std::minus<coef_type>());
}

Polynomial Polynomial::operator*(const Polynomial::term_type& term) const
{
    Polynomial ret;
    for (auto this_term : *this) {
        auto coef = this_term.first * term.first;
        auto exp = this_term.second + term.second;
        ret.insertTerm(qMakePair(coef, exp));
    }
    return ret;
}

Polynomial Polynomial::operator*(const Polynomial& rhs) const
{
    Polynomial ret;
    if (this->isZero() || rhs.isZero())
        return ret;
    for (auto term : rhs) {
        auto tmp = *this * term;
        ret += tmp;
    }
    qDebug() << "result is : " << ret.printPretty();
    return ret;
}

Polynomial Polynomial::operator/(const Polynomial& rhs)
{
    Polynomial ret;
    Polynomial lhs = *this;
    while (lhs.biggest_exp >= rhs.biggest_exp && lhs != Polynomial(0, 0)) {
        exp_type newTermExp = lhs.biggest_exp - rhs.biggest_exp;
        coef_type newTermCoef = lhs.get(lhs.biggest_exp) / rhs.get(rhs.biggest_exp);
        lhs -= rhs * Polynomial(newTermCoef, newTermExp);
        ret.insertTerm(qMakePair(newTermCoef, newTermExp));
    }
    return ret;
}

Polynomial Polynomial::operator-() const
{
    return Polynomial() - *this;
}

Polynomial& Polynomial::operator+=(const Polynomial& rhs)
{
    *this = *this + rhs;
    return *this;
}

Polynomial& Polynomial::operator-=(const Polynomial& rhs)
{
    *this = *this - rhs;
    return *this;
}

Polynomial& Polynomial::operator*=(const Polynomial& rhs)
{
    *this = *this * rhs;
    return *this;
}

bool Polynomial::operator==(const Polynomial& rhs)
{
    if (this->size() != rhs.size())
        return false;
    for (size_t i = 0; i < this->size(); ++i)
        if (this->polynomial[i] != rhs.polynomial[i])
            return false;
    return true;
}

bool Polynomial::operator!=(const Polynomial& rhs)
{
    return !(*this == rhs);
}

Polynomial Polynomial::derivative()
{
    Polynomial ret;
    for (auto t : polynomial)
        ret.insertTerm(qMakePair(t.first * t.second, t.second - 1));
    return ret;
}

Polynomial Polynomial::poly_power(int pow_num)
{
    Polynomial ret;
    if (pow_num < 0) {
    } else if (pow_num == 1) {
        ret.insertTerm(1, 0);
    } else {
        ret = *this;
        while (--pow_num) {
            ret *= ret;
        }
    }
    return ret;
}

Polynomial::value_type Polynomial::eval(Polynomial::value_type x)
{
    value_type ret = 0;
    for (auto t : polynomial) {
        ret += t.first * qPow(x, t.second);
    }
    return ret;
}

Polynomial::polynomial_type::iterator Polynomial::end()
{
    return this->polynomial.end();
}

Polynomial::polynomial_type::const_iterator Polynomial::begin() const
{
    return this->polynomial.cbegin();
}

Polynomial::polynomial_type::const_iterator Polynomial::end() const
{
    return this->polynomial.cend();
}

void Polynomial::setID(std::string _ID)
{
    m_ID = _ID;
}

std::string& Polynomial::getID()
{
    return m_ID;
}

void Polynomial::setNum(std::string _NUM)
{
    m_NUM = _NUM;
}

std::string& Polynomial::getNum()
{
    return m_NUM;
}

QString Polynomial::toText()
{
    QString ret;
    for (auto t : polynomial) {
        ret = ret + QString::number(static_cast<double>(t.first)) + ' ' + QString::number(static_cast<double>(t.second)) + ' ';
    }
    return ret;
}

void Polynomial::loadText(QString& line)
{
    polynomial.clear();
    QTextStream l(&line, QIODevice::ReadOnly);
    QString name, coef, exp;
    l >> name;
    this->setID(name.toStdString());
    while (!l.atEnd()) {
        l >> coef >> exp;
        if (coef == "" && exp == "")
            continue;
        insertTerm(qMakePair(std::stod(coef.toStdString()), std::stod(exp.toStdString())));
    }
}

Polynomial::polynomial_type::iterator Polynomial::begin()
{
    return this->polynomial.begin();
}

template <typename F>
Polynomial Polynomial::operator_undetermine(const Polynomial& rhs, F _operator) const
{
    Polynomial ret;
    auto ita = this->polynomial.begin();
    auto itb = rhs.polynomial.begin();
    while (ita != this->polynomial.end() || itb != rhs.polynomial.end()) {
        coef_type coef;
        exp_type exp;
        if (itb == rhs.polynomial.end()) {
            coef = ita->first;
            exp = ita->second;
            ++ita;
        } else if (ita == this->polynomial.end()) {
            coef = _operator(0, itb->first);
            exp = itb->second;
            ++itb;
        } else {
            auto exp_a = ita->second;
            auto exp_b = itb->second;
            if (exp_a > exp_b) {
                coef = ita->first;
                exp = ita->second;
                ++ita;
            } else if (exp_a < exp_b) {
                coef = _operator(0, itb->first);
                exp = itb->second;
                ++itb;
            } else {
                coef = _operator(ita->first, itb->first);
                exp = ita->second;
                ++ita;
                ++itb;
            }
        }
        ret.insertTerm(qMakePair(coef, exp));
    }
    return ret;
}

QDebug operator<<(QDebug out, const Polynomial& operand)
{
    out.nospace() << operand.printPretty();
    return out.maybeSpace();
}
