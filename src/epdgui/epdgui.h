#ifndef __EPDGUI_H
#define __EPDGUI_H

#include "epdgui_base.h"
#include "epdgui_button.h"
#include "epdgui_switch.h"
#include "epdgui_textbox.h"
#include "epdgui_mutexswitch.h"

void EPDGUI_AddObject(EPDGUI_Base* object);
void EPDGUI_Draw(m5epd_update_mode_t mode = UPDATE_MODE_GC16);
void EPDGUI_Process(void);
void EPDGUI_Process(int16_t x, int16_t y);
void EPDGUI_Clear(void);
void EPDGUI_Run(void *pargs = NULL);
void EPDGUI_SetAutoUpdate(bool isAuto);

#endif //__EPDGUI_H