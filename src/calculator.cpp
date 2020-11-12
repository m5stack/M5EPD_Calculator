#include <cstring>
#include <cmath>
#include "calculator.h"

#define STATUS_IN 0
#define STATUS_OUT 1

Calculator::Calculator()
{
}

Calculator::~Calculator()
{
    clear();
}

double Calculator::getResult()
{
    return _result;
}

String Calculator::getStrResult()
{
    return _str_result;
}

bool Calculator::calculate(String exp)
{
    clear();
    if (preProcess(exp) == false)
    {
        log_e("Error in expression");
        return false;
    }
    if (convertExp(exp) == false)
    {
        log_e("Error in conversion");
        return false;
    }
    if (computeExp() == false)
    {
        log_e("Error in compute");
        return false;
    }
    return true;
}

void Calculator::clear()
{
    while (!_value_stack.empty())
    {
        _value_stack.pop();
    }
    while (!_operator_stack.empty())
    {
        _operator_stack.pop();
    }
    _prn_vector.clear();
}

int Calculator::getOperatorLevel(char _opt_stack)
{
    switch (_opt_stack)
    {
    case '(':
        return 0;
    case '+':
    case '-':
        return 1;
    case '*':
    case '/':
        return 2;
    case '^':
        return 3;
    case ')':
        return 4;
    }
}

bool Calculator::isLegal(char _opt_stack)
{
    const char *kOperatorSet = "+-*/()=.^";
    for (int i = 0; i < strlen(kOperatorSet); i++)
    {
        if (_opt_stack == kOperatorSet[i])
        {
            return true;
        }
    }
    return false;
}

bool Calculator::preProcess(String &str)
{
    String result;
    str.replace("**", "^");
    for (int i = 0; i < str.length(); i++)
    {
        if (isLegal(str[i]) || isdigit(str[i]))
        {
            result += str[i];
        }
        else if (str[i] == ' ')
        {
            continue;
        }
        else
        {
            log_e("Invaild symbol");
            return false;
        }
    }
    str = result;
    return true;
}

bool Calculator::convertExp(String &src)
{
    int state = STATUS_OUT;
    char c;

    bool symbol = true;
    for (int i = 0; i < src.length(); i++)
    {
        calc_meta_data_t meta;
        c = src[i];
        meta.data = c;
        if (isdigit(c))
        {
            meta.is_opt = false;
        }
        else
        {
            meta.is_opt = true;
        }

        bool is_unary = false;
        if ((c == '-'))
        {
            if (i == 0)
            {
                is_unary = true;
            }
            else if (!isdigit(src[i - 1]))
            {
                is_unary = true;
            }
        }
        if (is_unary)
        {
            symbol = !symbol;
            continue;
        }

        if (isdigit(c))
        {
            meta.data = c - '0';
            if (symbol == false)
            {
                symbol = true;
                meta.data = -meta.data;
            }
            _prn_vector.push_back(meta);
            state = STATUS_IN;
        }
        else
        {
            if (state == STATUS_IN && c == '.')
            {
                calc_meta_data_t temp_meta;
                temp_meta.data = '.';
                _prn_vector.push_back(temp_meta);
                continue;
            }
            if (state == STATUS_IN && c != '.')
            {
                calc_meta_data_t temp_meta;
                temp_meta.data = ' ';
                _prn_vector.push_back(temp_meta);
            }
            if (c == '=')
            {
                break;
            }
            else if (c == '(')
            {
                _operator_stack.push(meta);
            }
            else if (c == ')')
            {
                while (!_operator_stack.empty() && _operator_stack.top().data != '(')
                {
                    _prn_vector.push_back(_operator_stack.top());
                    calc_meta_data_t temp_meta;
                    temp_meta.data = ' ';
                    _prn_vector.push_back(temp_meta);
                    _operator_stack.pop();
                }
                _operator_stack.pop();
            }
            else
            {
                while (true)
                {
                    if (_operator_stack.empty() || _operator_stack.top().data == '(')
                    {
                        _operator_stack.push(meta);
                    }

                    else if (getOperatorLevel(c) > getOperatorLevel(_operator_stack.top().data))
                        _operator_stack.push(meta);
                    else
                    {
                        _prn_vector.push_back(_operator_stack.top());
                        calc_meta_data_t temp_meta;
                        temp_meta.data = ' ';
                        _prn_vector.push_back(temp_meta);
                        _operator_stack.pop();
                        continue;
                    }
                    break;
                }
            }
            state = STATUS_OUT;
        }
    }
    while (!_operator_stack.empty())
    {
        _prn_vector.push_back(_operator_stack.top());
        calc_meta_data_t temp_meta;
        temp_meta.data = ' ';
        _prn_vector.push_back(temp_meta);
        _operator_stack.pop();
    }
    return true;
}

bool Calculator::computeExp()
{
    int value = 0;
    int state = STATUS_OUT;
    int c;
    bool point = false;
    double count = 1.0;

    bool symbol = true;
    for (int i = 0; i < _prn_vector.size(); i++)
    {
        c = _prn_vector[i].data;

        if ((c < 10) || (c == '.'))
        {
            if (c < 10)
            {
                if (c < 0)
                {
                    symbol = false;
                }
                c = abs(c);
                value *= 10;
                value += c;
                state = STATUS_IN;
                if (point == true)
                {
                    count *= 10.0;
                }
            }
            if (c == '.')
            {
                point = true;
                continue;
            }
        }
        else
        {
            point = false;
            double ans = value / count;
            count = 1.0;
            if (state == STATUS_IN)
            {
                if (symbol == false)
                {
                    symbol = true;
                    _value_stack.push(-ans);
                }
                else
                {
                    _value_stack.push(ans);
                }

                value = 0;
            }
            double x, y;
            if (c != ' ')
            {
                if(_value_stack.empty())
                {
                    log_e("Expression ERROR");
                    _result = 0;
                    _str_result = "";
                    return false;
                }
                x = _value_stack.top();
                _value_stack.pop();

                if(_value_stack.empty())
                {
                    log_e("Expression ERROR");
                    _result = 0;
                    _str_result = "";
                    return false;
                }
                y = _value_stack.top();
                _value_stack.pop();

                switch (c)
                {
                case '+':
                    _value_stack.push(x + y);
                    break;
                case '-':
                    _value_stack.push(y - x);
                    break;
                case '*':
                    _value_stack.push(x * y);
                    break;
                case '/':
                    if(x == 0)
                    {
                        log_e("Division by 0 ERROR");
                        _result = 0;
                        _str_result = "";
                        return false;
                    }
                    _value_stack.push(y / x);
                    break;
                case '^':
                    _value_stack.push(pow(y, x));
                default:
                    log_e("Operator ERROR");
                }
            }
            state = STATUS_OUT;
        }
    }
    _result = _value_stack.top();
    _str_result = String(_result);
    for(int i = _str_result.length() - 1; i >= 0; i--)
    {
        if(_str_result[i] == '0')
        {
            _str_result.remove(i);
        }
        else
        {
            break;
        }
    }
    if(_str_result[_str_result.length() - 1] == '.')
    {
        _str_result.remove(_str_result.length() - 1);
    }
    return true;
}
