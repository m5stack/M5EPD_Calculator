#include <M5EPD.h>
#include "./epdgui/epdgui.h"
#include "binaryttf.h"
#include "calculator.h"
#include <regex>

const char *kTitles[20] = {
    "AC", "(", ")", "<-",
    "7", "8", "9", "÷",
    "4", "5", "6", "x",
    "1", "2", "3", "-",
    ".", "0", "=", "+"};

enum
{
    OPERATOR_NULL = 0,
    OPERATOR_ADDITION,
    OPERATOR_SUBTRACTION,
    OPERATOR_MULTIPLICATION,
    OPERATOR_DIVISION,
    OPERATOR_TAKEREMAIN,
    OPERATOR_EQUAL,
    OPERATOR_SYMBOL,
    OPERATOR_ALLCLEAR,
    OPERATOR_POINT
};

enum
{
    KEY_AC = 0,
    KEY_LP,
    KEY_RP,
    KEY_BK,
    KEY_7,
    KEY_8,
    KEY_9,
    KEY_DIV,
    KEY_4,
    KEY_5,
    KEY_6,
    KEY_MUL,
    KEY_1,
    KEY_2,
    KEY_3,
    KEY_SUB,
    KEY_POINT,
    KEY_0,
    KEY_EQUAL,
    KEY_PLUS
};

#define DEFAULT_FONT_SIZE 88

Calculator cal;
EPDGUI_Button *btns[20];
M5EPD_Canvas canvas_result(&M5.EPD);
String input_str;
String result_str;
int input_font_size = DEFAULT_FONT_SIZE;

void updateValue()
{
    int pos;
    canvas_result.fillCanvas(0);
    if(result_str.length())
    {
        canvas_result.setTextSize(input_font_size);
        canvas_result.drawString(input_str, 512, 172);
        canvas_result.setTextSize(DEFAULT_FONT_SIZE);
        canvas_result.drawString(result_str, 512, 272);
    }
    else
    {
        canvas_result.setTextSize(input_font_size);
        pos = canvas_result.drawString(input_str, 512, 272);
        if(pos > 480)
        {
            if(input_font_size > 8)
            {
                input_font_size -= 8;
            }
        }
        else if(pos < 400)
        {
            if(input_font_size != DEFAULT_FONT_SIZE)
            {
                input_font_size += 8;
            }
        }
    }
    canvas_result.pushCanvas(12, 12, UPDATE_MODE_A2);
}

void key_ac_cb(epdgui_args_vector_t &args)
{
    input_font_size = DEFAULT_FONT_SIZE;
    input_str = "";
    result_str = "";
    updateValue();
}

void key_bk_cb(epdgui_args_vector_t &args)
{
    if (input_str.length())
    {
        input_str.remove(input_str.length() - 1);
        updateValue();
    }
}

void key_equal_cb(epdgui_args_vector_t &args)
{
    if (input_str.length())
    {
        input_str += "=";
        if(cal.calculate(input_str) == false)
        {
            result_str = "ERROR";
        }
        else
        {
            result_str = cal.getStrResult();
        }
        updateValue();
        input_str = "";
        result_str = "";
        input_font_size = DEFAULT_FONT_SIZE;
    }
}

void key_input_cb(epdgui_args_vector_t &args)
{
    EPDGUI_Button *btn = (EPDGUI_Button *)(args[0]);
    input_str += btn->GetCustomString();
    updateValue();
}

void creatKeys()
{
    for (int j = 0; j < 5; j++)
    {
        for (int i = 0; i < 4; i++)
        {
            int idx = j * 4 + i;
            btns[idx] = new EPDGUI_Button(kTitles[idx], 12 + i * 132, 300 + j * 132, 120, 120);
            btns[idx]->SetCustomString(kTitles[idx]);
            EPDGUI_AddObject(btns[idx]);
        }
    }

    btns[KEY_EQUAL]->Bind(EPDGUI_Button::EVENT_RELEASED, key_equal_cb);
    btns[KEY_BK]->Bind(EPDGUI_Button::EVENT_RELEASED, key_bk_cb);
    btns[KEY_AC]->Bind(EPDGUI_Button::EVENT_RELEASED, key_ac_cb);

    btns[KEY_0]->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, btns[KEY_0]);
    btns[KEY_0]->Bind(EPDGUI_Button::EVENT_RELEASED, key_input_cb);
    btns[KEY_0]->SetCustomString("0");
    btns[KEY_1]->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, btns[KEY_1]);
    btns[KEY_1]->Bind(EPDGUI_Button::EVENT_RELEASED, key_input_cb);
    btns[KEY_1]->SetCustomString("1");
    btns[KEY_2]->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, btns[KEY_2]);
    btns[KEY_2]->Bind(EPDGUI_Button::EVENT_RELEASED, key_input_cb);
    btns[KEY_2]->SetCustomString("2");
    btns[KEY_3]->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, btns[KEY_3]);
    btns[KEY_3]->Bind(EPDGUI_Button::EVENT_RELEASED, key_input_cb);
    btns[KEY_3]->SetCustomString("3");
    btns[KEY_4]->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, btns[KEY_4]);
    btns[KEY_4]->Bind(EPDGUI_Button::EVENT_RELEASED, key_input_cb);
    btns[KEY_4]->SetCustomString("4");
    btns[KEY_5]->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, btns[KEY_5]);
    btns[KEY_5]->Bind(EPDGUI_Button::EVENT_RELEASED, key_input_cb);
    btns[KEY_5]->SetCustomString("5");
    btns[KEY_6]->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, btns[KEY_6]);
    btns[KEY_6]->Bind(EPDGUI_Button::EVENT_RELEASED, key_input_cb);
    btns[KEY_6]->SetCustomString("6");
    btns[KEY_7]->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, btns[KEY_7]);
    btns[KEY_7]->Bind(EPDGUI_Button::EVENT_RELEASED, key_input_cb);
    btns[KEY_7]->SetCustomString("7");
    btns[KEY_8]->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, btns[KEY_8]);
    btns[KEY_8]->Bind(EPDGUI_Button::EVENT_RELEASED, key_input_cb);
    btns[KEY_8]->SetCustomString("8");
    btns[KEY_9]->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, btns[KEY_9]);
    btns[KEY_9]->Bind(EPDGUI_Button::EVENT_RELEASED, key_input_cb);
    btns[KEY_9]->SetCustomString("9");
    btns[KEY_PLUS]->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, btns[KEY_PLUS]);
    btns[KEY_PLUS]->Bind(EPDGUI_Button::EVENT_RELEASED, key_input_cb);
    btns[KEY_PLUS]->SetCustomString("+");
    btns[KEY_SUB]->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, btns[KEY_SUB]);
    btns[KEY_SUB]->Bind(EPDGUI_Button::EVENT_RELEASED, key_input_cb);
    btns[KEY_SUB]->SetCustomString("-");
    btns[KEY_MUL]->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, btns[KEY_MUL]);
    btns[KEY_MUL]->Bind(EPDGUI_Button::EVENT_RELEASED, key_input_cb);
    btns[KEY_MUL]->SetCustomString("*");
    btns[KEY_DIV]->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, btns[KEY_DIV]);
    btns[KEY_DIV]->Bind(EPDGUI_Button::EVENT_RELEASED, key_input_cb);
    btns[KEY_DIV]->SetCustomString("/");
    btns[KEY_POINT]->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, btns[KEY_POINT]);
    btns[KEY_POINT]->Bind(EPDGUI_Button::EVENT_RELEASED, key_input_cb);
    btns[KEY_POINT]->SetCustomString(".");
    btns[KEY_LP]->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, btns[KEY_LP]);
    btns[KEY_LP]->Bind(EPDGUI_Button::EVENT_RELEASED, key_input_cb);
    btns[KEY_LP]->SetCustomString("(");
    btns[KEY_RP]->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, btns[KEY_RP]);
    btns[KEY_RP]->Bind(EPDGUI_Button::EVENT_RELEASED, key_input_cb);
    btns[KEY_RP]->SetCustomString(")");
}

void preProcess(String &exp)
{
    bool is_first = false;
    for(int i = 0; i < exp.length(); i++)
    {
        if(exp[i] == ' ')
        {
            continue;
        }
        if(exp[i] == '-')
        {
            is_first = true;
        }
    }
    if(is_first)
    {
        exp = "0" + exp;
    }
    else
    {
        exp = "0+" + exp;
    }

    try
    {
        std::regex re("-\\s*(\\d)", std::regex::icase);
        exp = std::regex_replace(exp.c_str(), re, "+(0-$1)").c_str();
    }
    catch (std::regex_error& e) 
    {
        if (e.code() == std::regex_constants::error_badrepeat)
        {
            log_e("Repeat was not preceded by a valid regular expression.");
        }
        else
        {
            log_e("Some other regex exception happened.");
        }
    }
    catch (...) 
    {
        printf("Unknown ERROR");
    }
}

void setup()
{
    // disableCore0WDT();

    M5.begin();
    M5.TP.SetRotation(90);
    M5.EPD.SetRotation(90);
    M5.EPD.Clear(true);

    canvas_result.createCanvas(516, 276);
    canvas_result.loadFont(binaryttf, sizeof(binaryttf));
    canvas_result.createRender(36, 14);
    for(int i = DEFAULT_FONT_SIZE; i >= 8; i-= 8)
    {
        canvas_result.createRender(i, 14);
        canvas_result.setTextSize(i);
        for (int i = 0; i < 10; i++)
        {
            canvas_result.preRender('0' + i);
        }
        canvas_result.preRender('.');
        canvas_result.preRender('-');
        canvas_result.preRender('(');
        canvas_result.preRender(')');
    }
    canvas_result.setTextDatum(BR_DATUM);

    creatKeys();
    updateValue();
}

void loop()
{
    EPDGUI_Run();
    printf("??\n");
    delay(2);
}

