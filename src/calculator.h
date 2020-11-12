
#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <Arduino.h>
#include <stack>
#include <vector>

typedef struct
{
    bool is_opt = false;
    int data;
}calc_meta_data_t;

class Calculator
{
    public:
        Calculator();
        ~Calculator();
        bool calculate(String exp);
        double getResult();
        String getStrResult();

    private:
        std::stack<calc_meta_data_t> _operator_stack;
        std::stack<char> _opt_stack;
        std::stack<double> _value_stack;
        std::vector<calc_meta_data_t> _prn_vector;
        double _result;
        String _str_result;
        String _prn_exp;

    private:
        void clear();
        int getOperatorLevel(char opt);
        bool isLegal(char opt); 
        bool preProcess(String &str);
        bool convertExp(String &src);
        bool convertExp(String &from, String &to);
        bool computeExp();
};
       
double getResult();
 
#endif