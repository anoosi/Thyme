/**
 * @file
 *
 * @author Jonathan Wilson
 *
 * @brief Radio Button
 *
 * @copyright Thyme is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "gadgetradiobutton.h"
#include "gamewindowmanager.h"

void Gadget_Radio_Set_Text(GameWindow *radio_button, Utf16String text)
{
#ifdef GAME_DLL // temporary since we can't change the definition of Win_Send_System_Msg at this point and we can't cast a
                // pointer to an unsigned int on 64 bit
    g_theWindowManager->Win_Send_System_Msg(radio_button, GGM_SET_LABEL, reinterpret_cast<unsigned int>(&text), 0);
#endif
}

WindowMsgHandledType Gadget_Radio_Button_Input(
    GameWindow *radio_button, unsigned int message, unsigned int data_1, unsigned int data_2)
{
#ifdef GAME_DLL
    return Call_Function<WindowMsgHandledType, GameWindow *, unsigned int, unsigned int, unsigned int>(
        PICK_ADDRESS(0x006D8610, 0x00A59F40), radio_button, message, data_1, data_2);
#else
    return MSG_IGNORED;
#endif
}

WindowMsgHandledType Gadget_Radio_Button_System(
    GameWindow *radio_button, unsigned int message, unsigned int data_1, unsigned int data_2)
{
#ifdef GAME_DLL
    return Call_Function<WindowMsgHandledType, GameWindow *, unsigned int, unsigned int, unsigned int>(
        PICK_ADDRESS(0x006D8AA0, 0x00A5A3CD), radio_button, message, data_1, data_2);
#else
    return MSG_IGNORED;
#endif
}
