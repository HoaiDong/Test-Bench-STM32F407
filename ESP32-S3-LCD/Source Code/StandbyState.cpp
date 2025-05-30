#include "StandbyState.h"
#include "DataType.h"
#include "Communication.h"
#include "ui.h"
#include "stdio.h"
#include "SensorValue.h"

extern StateMachine CurrentState;

// Xử lý data frame CAN nhận về 
static void ProcessData(void);

static SAFE_STATE CheckSafeCondition(void);

static void DisplayThrottle(void);
static void DisplayLoad(void);
static void DisplayUIConnection(CONNECTION_STATUS Status);

void StandbyState(void)
{
    _ui_screen_change(&ui_StandbyState, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_StandbyState_screen_init);


    while (1)
    {

        if (twai_receive(&rx_message, pdMS_TO_TICKS(1000)) == ESP_OK) 
        {
            ProcessData();
        }
        else
        {
            checkAndRecoverTWAI();
        }

        
        if (CheckSafeCondition() == UN_SAFE)
        {
            lv_anim_del_all();
            // Blink_Animation(ui_Panel2, 0);
        }
        else
        {
            lv_anim_del_all();
        }

        if (CheckSTMConnection() == 0)
        {
            CurrentState = FAULT_STATE;
        }

        if (CurrentState != STANDBY_STATE)
        {
            lv_anim_del_all();
            _ui_screen_delete(&ui_StandbyState);
            return;
        }

        esp_task_wdt_reset(); 
    }
    
}


// Xử lý data frame CAN nhận về 
static void ProcessData(void)
{
    switch (rx_message.identifier)
    {
        case COMM_LCD_CONNECT_ALIVE:
            CommLCDConnectAlive();
            break;
        
        case COMM_LCD_STATE_MACHINE:
            CurrentState = (StateMachine)rx_message.data[0];
            break;

        case COMM_LCD_THROTTLE:
            Throttle = (unsigned int)rx_message.data[0] << 24 | (unsigned int)rx_message.data[1] << 16 | (unsigned int)rx_message.data[2] << 8 | (unsigned int)rx_message.data[3] << 0;
            DisplayThrottle();
            break;

        case COMM_LCD_LOAD:
            LoadOnOff = (SWITCH_STATUS)rx_message.data[0];
            LoadLevel = (LOAD_LEVEL)rx_message.data[1];
            DisplayLoad();
            break;

        case COMM_LCD_UI_CONNECTION_STATUS:
            UIConnectStatus = (CONNECTION_STATUS)rx_message.data[0];
            DisplayUIConnection(UIConnectStatus);
            break;

    }
}

// Kiểm tra giá trị an toàn 
static SAFE_STATE CheckSafeCondition(void)
{
    if (Throttle >= 15)
    {
        return UN_SAFE;
    }

    if (LoadOnOff == SWITCH_ON)
    {
        return UN_SAFE;
    }

    if (LoadLevel != LOAD_1)
    {
        return UN_SAFE;
    }

    return SAFE;
}

static void DisplayThrottle(void)
{
    // // Cập nhật giá trị tay ga trên LCD 
    char ThrottleValueText[5];
    sprintf(ThrottleValueText, "%d%%", Throttle);
    lv_label_set_text(label_value, ThrottleValueText);

    lv_meter_set_indicator_end_value(meter, throttle_meter_value, Throttle);

}

static void DisplayLoad(void)
{
    // Cập nhật switch load on off 
    if (LoadOnOff == SWITCH_ON)
    {        
        lv_obj_set_style_bg_color(ui_Panel48, lv_color_hex(GREEN_COLOR), LV_PART_MAIN); 
        _ui_label_set_property(ui_Label6, _UI_LABEL_PROPERTY_TEXT, "ON");
        lv_obj_set_style_text_color(ui_Label6, lv_color_hex(GREEN_COLOR), LV_PART_MAIN);
    }
    else
    {
        lv_obj_set_style_bg_color(ui_Panel48, lv_color_hex(RED_COLOR), LV_PART_MAIN); 
        _ui_label_set_property(ui_Label6, _UI_LABEL_PROPERTY_TEXT, "OFF");
        lv_obj_set_style_text_color(ui_Label6, lv_color_hex(RED_COLOR), LV_PART_MAIN);
    }




    // Cập nhật giá trị slider 
    switch (LoadLevel)
    {
        case LOAD_1:
        
        lv_obj_set_style_bg_color(ui_Panel55, lv_color_hex(GREEN_COLOR), LV_PART_MAIN);
        lv_obj_set_style_bg_color(ui_Panel62, lv_color_hex(RED_COLOR), LV_PART_MAIN);
        lv_obj_set_style_bg_color(ui_Panel63, lv_color_hex(RED_COLOR), LV_PART_MAIN);
        lv_obj_set_style_bg_color(ui_Panel64, lv_color_hex(RED_COLOR), LV_PART_MAIN);
        break;

        case LOAD_2:
        lv_obj_set_style_bg_color(ui_Panel55, lv_color_hex(GREEN_COLOR), LV_PART_MAIN);
        lv_obj_set_style_bg_color(ui_Panel62, lv_color_hex(GREEN_COLOR), LV_PART_MAIN);
        lv_obj_set_style_bg_color(ui_Panel63, lv_color_hex(RED_COLOR), LV_PART_MAIN);
        lv_obj_set_style_bg_color(ui_Panel64, lv_color_hex(RED_COLOR), LV_PART_MAIN);
        break;

        case LOAD_3:
        lv_obj_set_style_bg_color(ui_Panel55, lv_color_hex(GREEN_COLOR), LV_PART_MAIN);
        lv_obj_set_style_bg_color(ui_Panel62, lv_color_hex(GREEN_COLOR), LV_PART_MAIN);
        lv_obj_set_style_bg_color(ui_Panel63, lv_color_hex(GREEN_COLOR), LV_PART_MAIN);
        lv_obj_set_style_bg_color(ui_Panel64, lv_color_hex(RED_COLOR), LV_PART_MAIN);
        break;

        case LOAD_4:
        lv_obj_set_style_bg_color(ui_Panel55, lv_color_hex(GREEN_COLOR), LV_PART_MAIN);
        lv_obj_set_style_bg_color(ui_Panel62, lv_color_hex(GREEN_COLOR), LV_PART_MAIN);
        lv_obj_set_style_bg_color(ui_Panel63, lv_color_hex(GREEN_COLOR), LV_PART_MAIN);
        lv_obj_set_style_bg_color(ui_Panel64, lv_color_hex(GREEN_COLOR), LV_PART_MAIN);
        break;
    }
}


static void DisplayUIConnection(CONNECTION_STATUS Status)
{
    // Cập nhật trạng thái kết nối UI
    if ( UIConnectStatus == DISCONNECTED)
    {
        lv_obj_set_style_bg_color(ui_Panel4, lv_color_hex(WHITE_COLOR), LV_PART_MAIN);
        _ui_flag_modify(ui_Image1, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE);
        _ui_flag_modify(ui_Image2, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
    }
    else
    {
       lv_obj_set_style_bg_color(ui_Panel4, lv_color_hex(GREEN_COLOR), LV_PART_MAIN);
        _ui_flag_modify(ui_Image1, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
        _ui_flag_modify(ui_Image2, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE);
    }
}