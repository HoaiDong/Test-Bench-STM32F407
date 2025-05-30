#include "FaultState.h"
#include "DataType.h"
#include "Communication.h"
#include "ui.h"
#include "stdio.h"


extern StateMachine CurrentState;

// Xử lý data frame CAN nhận về 
static void ProcessData(void);

// Thay đổi màu sắc các label sensor tương ứng với fault mask
static void CheckingSystem(FaultCode FaultMask);

void FaultState(void)
{
    while (1)
    {
        _ui_screen_change(&ui_FaultState, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_FaultState_screen_init);

        while (1)
        {
            // Nếu quên dòng này, ESP sẽ reset sau 5 giây
            esp_task_wdt_reset(); 

            
            if (twai_receive(&rx_message, pdMS_TO_TICKS(1000)) == ESP_OK) 
            {
                // twai_clear_receive_queue();
                // Serial.println("OKKKKKKKKK");
                ProcessData();
            }
            else
            {
                    checkAndRecoverTWAI();
            }
            
            if (CheckSTMConnection() == 0)
            {
                lv_obj_set_style_bg_color(ui_Panel57, lv_color_hex(RED_COLOR), LV_PART_MAIN | LV_STATE_DEFAULT);
            }
            else
            {
                lv_obj_set_style_bg_color(ui_Panel57, lv_color_hex(GREEN_COLOR), LV_PART_MAIN | LV_STATE_DEFAULT);
            }

            if (CurrentState != UI_STATE)
            {
                lv_anim_del_all();
                _ui_screen_delete(&ui_FaultState); 
                return;
            }

            // Nếu quên dòng này, ESP sẽ reset sau 5 giây
            esp_task_wdt_reset(); 
            // delay(10);
            // vTaskDelay(pdMS_TO_TICKS(2));

        }
    }
    
}


// Xử lý data frame CAN nhận về 
static void ProcessData(void)
{
    switch (rx_message.identifier)
    {
        case COMM_LCD_CONNECT_ALIVE:
        // Serial.println("ESP received COMM_LCD_CONNECT_ALIVE\n");
            CommLCDConnectAlive();
            break;
        
        case COMM_LCD_STATE_MACHINE:
        // Serial.println("ESP received COMM_LCD_STATE_MACHINE\n");
            CurrentState = (StateMachine)rx_message.data[0];
            break;

        case COMM_LCD_FAULT_CODE:
        // Serial.println("ESP received COMM_LCD_FAULT_CODE\n");
        FaultCode FaultMask = FAULT_NONE;
        FaultMask = (FaultCode)((unsigned short)rx_message.data[0] << 8 | (unsigned short)rx_message.data[1] << 0);
        CheckingSystem(FaultMask);
    }
}


// Thay đổi màu sắc các label sensor tương ứng với fault mask
static void CheckingSystem(FaultCode FaultMask)
{

    if (FaultMask & FAULT_THROTTLE_MISSING)
    {
        lv_obj_set_style_bg_color(ui_Panel49, lv_color_hex(RED_COLOR), LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    else
    {
        lv_obj_set_style_bg_color(ui_Panel49, lv_color_hex(GREEN_COLOR), LV_PART_MAIN | LV_STATE_DEFAULT);
    }

    if (FaultMask & FAULT_LOAD_LEVEL_SWITCH_MISSING) 
    {
        lv_obj_set_style_bg_color(ui_Panel50, lv_color_hex(RED_COLOR), LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    else
    {
        lv_obj_set_style_bg_color(ui_Panel50, lv_color_hex(GREEN_COLOR), LV_PART_MAIN | LV_STATE_DEFAULT);
    }


    if (FaultMask & FAULT_LOAD_LEVEL_SWITCH_MISSING)
    {
        lv_obj_set_style_bg_color(ui_Panel51, lv_color_hex(RED_COLOR), LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    else
    {
        lv_obj_set_style_bg_color(ui_Panel51, lv_color_hex(GREEN_COLOR), LV_PART_MAIN | LV_STATE_DEFAULT);
    }


    if (FaultMask & FAULT_UI_MISSING)
    {
        lv_obj_set_style_bg_color(ui_Panel56, lv_color_hex(RED_COLOR), LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    else
    {
        lv_obj_set_style_bg_color(ui_Panel56, lv_color_hex(GREEN_COLOR), LV_PART_MAIN | LV_STATE_DEFAULT);
    }



    if (FaultMask & FAULT_SPEED_SENSOR_MISSING)
    {
        lv_obj_set_style_bg_color(ui_Panel58, lv_color_hex(RED_COLOR), LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    else
    {
        lv_obj_set_style_bg_color(ui_Panel58, lv_color_hex(GREEN_COLOR), LV_PART_MAIN | LV_STATE_DEFAULT);
    }

    if (FaultMask & FAULT_TORQUE_SENSOR_MISSING)
    {
        lv_obj_set_style_bg_color(ui_Panel53, lv_color_hex(RED_COLOR), LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    else
    {
        lv_obj_set_style_bg_color(ui_Panel53, lv_color_hex(GREEN_COLOR), LV_PART_MAIN | LV_STATE_DEFAULT);
    }




    if (FaultMask & FAULT_VOLTAGE_VOTOL_SENSOR_MISSING)
    {
        
        lv_obj_set_style_bg_color(ui_Panel59, lv_color_hex(RED_COLOR), LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    else
    {
        
        lv_obj_set_style_bg_color(ui_Panel59, lv_color_hex(GREEN_COLOR), LV_PART_MAIN | LV_STATE_DEFAULT);
    }



    if (FaultMask & FAULT_CURRENT_VOTOL_SENSOR_MISSING)
    {
        lv_obj_set_style_bg_color(ui_Panel60, lv_color_hex(RED_COLOR), LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    else
    {
        lv_obj_set_style_bg_color(ui_Panel60, lv_color_hex(GREEN_COLOR), LV_PART_MAIN | LV_STATE_DEFAULT);
    }


    if (FaultMask & FAULT_VOLTAGE_GENERATOR_SENSOR_MISSING)
    {
        lv_obj_set_style_bg_color(ui_Panel61, lv_color_hex(RED_COLOR), LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    else
    {
        lv_obj_set_style_bg_color(ui_Panel61, lv_color_hex(GREEN_COLOR), LV_PART_MAIN | LV_STATE_DEFAULT);
    }


    if (FaultMask & FAULT_CURRENT_GENERATOR_SENSOR_MISSING)
    {
        lv_obj_set_style_bg_color(ui_Panel54, lv_color_hex(RED_COLOR), LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    else
    {
        lv_obj_set_style_bg_color(ui_Panel54, lv_color_hex(GREEN_COLOR), LV_PART_MAIN | LV_STATE_DEFAULT);
    }

}   