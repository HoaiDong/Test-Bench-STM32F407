#include "InitState.h"
#include "Communication.h"
#include "DataType.h"
#include "ui.h"


extern StateMachine CurrentState;

// Xử lý data frame CAN nhận về 
static void ProcessData(void);

// Thay đổi màu sắc các label sensor tương ứng với fault mask
static void CheckingSystem(FaultCode FaultMask);

void InitState(void)
{
    if(ui_InitState !=  NULL)
    {
        _ui_screen_change(&ui_InitState, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_InitState_screen_init);
    }    


    while (1)
    {
        esp_task_wdt_reset(); 


        if (twai_receive(&rx_message, pdMS_TO_TICKS(100)) == ESP_OK) 
        {
            ProcessData();
        }
        {
            checkAndRecoverTWAI();
        }
        
        if (CheckSTMConnection() == 0)
        {  
            lv_obj_set_style_bg_color(ui_Init_Panel10, lv_color_hex(RED_COLOR), LV_PART_MAIN);
            lv_label_set_text(ui_Init_Label10, "Inactive");
        }
        else
        {
            lv_obj_set_style_bg_color(ui_Init_Panel10, lv_color_hex(GREEN_COLOR), LV_PART_MAIN);
            lv_label_set_text(ui_Init_Label10, "Active");
        }
    

        if (CurrentState != INITIALIZATION_STATE)
        {
            lv_anim_del_all();
            _ui_screen_delete(&ui_InitState);
            return;
        }

        esp_task_wdt_reset(); 
    }
}

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

        case COMM_LCD_FAULT_CODE:
            FaultCode FaultMask = FAULT_NONE;
            FaultMask = (FaultCode)(rx_message.data[0] << 8 | rx_message.data[1] << 0);
            CheckingSystem(FaultMask);
            
    }
}

// Thay đổi màu sắc các label sensor tương ứng với fault mask
static void CheckingSystem(FaultCode FaultMask)
{
    if (FaultMask & FAULT_TORQUE_SENSOR_MISSING)
    {
        lv_obj_set_style_bg_color(ui_Init_Panel1, lv_color_hex(RED_COLOR), LV_PART_MAIN);
        lv_label_set_text(ui_Init_Label1, "Inactive");
    }
    else
    {
        lv_obj_set_style_bg_color(ui_Init_Panel1, lv_color_hex(GREEN_COLOR), LV_PART_MAIN);
        lv_label_set_text(ui_Init_Label1, "Active");
    }

    if (FaultMask & FAULT_SPEED_SENSOR_MISSING)
    {
        lv_obj_set_style_bg_color(ui_Init_Panel2, lv_color_hex(RED_COLOR), LV_PART_MAIN);
        lv_label_set_text(ui_Init_Label2, "Inactive");
    }
    else
    {
        lv_obj_set_style_bg_color(ui_Init_Panel2, lv_color_hex(GREEN_COLOR), LV_PART_MAIN);
        lv_label_set_text(ui_Init_Label2, "Active");
    }


    if (FaultMask & FAULT_THROTTLE_MISSING)
    {
        lv_obj_set_style_bg_color(ui_Init_Panel4, lv_color_hex(RED_COLOR), LV_PART_MAIN);
        lv_label_set_text(ui_Init_Label4, "Inactive");
    }
    else
    {
        lv_obj_set_style_bg_color(ui_Init_Panel4, lv_color_hex(GREEN_COLOR), LV_PART_MAIN);
        lv_label_set_text(ui_Init_Label4, "Active");
    }

    // // if (FaultMask & FAULT_LOAD_LEVEL_SWITCH_MISSING) Đang bị thiếu load on offoff
    // // {

    // // }

    // if (FaultMask & FAULT_LOAD_LEVEL_SWITCH_MISSING)
    // {
    //     lv_obj_set_style_bg_color(ui_Panel19, lv_color_hex(RED_COLOR), LV_PART_MAIN | LV_STATE_DEFAULT);
    // }
    // else
    // {
    //     lv_obj_set_style_bg_color(ui_Panel19, lv_color_hex(GREEN_COLOR), LV_PART_MAIN | LV_STATE_DEFAULT);
    // }
    

    if (FaultMask & FAULT_VOLTAGE_VOTOL_SENSOR_MISSING)
    {
        lv_obj_set_style_bg_color(ui_Init_Panel12, lv_color_hex(RED_COLOR), LV_PART_MAIN);
        lv_label_set_text(ui_Init_Label12, "Inactive");
    }
    else
    {
        lv_obj_set_style_bg_color(ui_Init_Panel12, lv_color_hex(GREEN_COLOR), LV_PART_MAIN);
        lv_label_set_text(ui_Init_Label12, "Active");
    }

    if (FaultMask & FAULT_CURRENT_VOTOL_SENSOR_MISSING)
    {
        lv_obj_set_style_bg_color(ui_Init_Panel18, lv_color_hex(RED_COLOR), LV_PART_MAIN);
        lv_label_set_text(ui_Init_Label18, "Inactive");
    }
    else
    {
        lv_obj_set_style_bg_color(ui_Init_Panel18, lv_color_hex(GREEN_COLOR), LV_PART_MAIN);
        lv_label_set_text(ui_Init_Label18, "Active");
    }
    
    if (FaultMask & FAULT_VOLTAGE_GENERATOR_SENSOR_MISSING)
    {
        lv_obj_set_style_bg_color(ui_Init_Panel25, lv_color_hex(RED_COLOR), LV_PART_MAIN);
        lv_label_set_text(ui_Init_Label25, "Inactive");
    }
    else
    {
        lv_obj_set_style_bg_color(ui_Init_Panel25, lv_color_hex(GREEN_COLOR), LV_PART_MAIN);
        lv_label_set_text(ui_Init_Label25, "Active");
    }
    

    if (FaultMask & FAULT_CURRENT_GENERATOR_SENSOR_MISSING)
     {
        lv_obj_set_style_bg_color(ui_Init_Panel27, lv_color_hex(RED_COLOR), LV_PART_MAIN);
        lv_label_set_text(ui_Init_Label27, "Inactive");
    }
    else
    {
        lv_obj_set_style_bg_color(ui_Init_Panel27, lv_color_hex(GREEN_COLOR), LV_PART_MAIN);
        lv_label_set_text(ui_Init_Label27, "Active");
    }
    


}