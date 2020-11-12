#include <stack>
#include <map>
#include <list>
#include "epdgui.h"

std::list<EPDGUI_Base*> epdgui_object_list;
uint32_t obj_id = 1;
bool _is_auto_update = true;
uint16_t _last_pos_x = 0xFFFF, _last_pos_y = 0xFFFF;

void EPDGUI_AddObject(EPDGUI_Base* object)
{
    obj_id++;
    epdgui_object_list.push_back(object);
}

void EPDGUI_Draw(m5epd_update_mode_t mode)
{
    for(std::list<EPDGUI_Base*>::iterator p = epdgui_object_list.begin(); p != epdgui_object_list.end(); p++)
    {
        (*p)->Draw(mode);
    }
}

void EPDGUI_Process(void)
{
    for(std::list<EPDGUI_Base*>::iterator p = epdgui_object_list.begin(); p != epdgui_object_list.end(); p++)
    {
        (*p)->UpdateState(-1, -1);
    }
}

void EPDGUI_Process(int16_t x, int16_t y)
{
    for(std::list<EPDGUI_Base*>::iterator p = epdgui_object_list.begin(); p != epdgui_object_list.end(); p++)
    {
        // log_d("%d, %d -> %d, %d, %d, %d", x, y, (*p)->getX(), (*p)->getY(), (*p)->getRX(), (*p)->getBY());
        (*p)->UpdateState(x, y);
    }
}

void EPDGUI_Clear(void)
{
    epdgui_object_list.clear();
}

void EPDGUI_Run(void *pargs)
{
    uint32_t last_active_time = 0;

    EPDGUI_Draw(UPDATE_MODE_NONE);
    M5.EPD.UpdateFull(UPDATE_MODE_GC16);

    while (1)
    {
        if (M5.TP.avaliable())
        {
            M5.TP.update();
            bool is_finger_up = M5.TP.isFingerUp();
            if(is_finger_up || (_last_pos_x != M5.TP.readFingerX(0)) || (_last_pos_y != M5.TP.readFingerY(0)))
            {
                _last_pos_x = M5.TP.readFingerX(0);
                _last_pos_y = M5.TP.readFingerY(0);
                if(is_finger_up)
                {
                    EPDGUI_Process();
                    last_active_time = millis();
                }
                else
                {
                    EPDGUI_Process(M5.TP.readFingerX(0), M5.TP.readFingerY(0));
                    last_active_time = 0;
                }
            }
            
            M5.TP.flush();
        }

        if((last_active_time != 0) && (millis() - last_active_time > 2000))
        {
            if(M5.EPD.UpdateCount() > 4)
            {
                M5.EPD.ResetUpdateCount();
                if(_is_auto_update)
                {
                    M5.EPD.UpdateFull(UPDATE_MODE_GL16);
                }
            }
            last_active_time = 0;
        }
    }
}

void EPDGUI_SetAutoUpdate(bool isAuto)
{
    _is_auto_update = isAuto;
}
