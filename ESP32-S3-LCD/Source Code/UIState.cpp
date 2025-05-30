#include "UIState.h"
#include "DataType.h"
#include "Communication.h"
#include "ui.h"
#include "stdio.h"
#include "SensorValue.h"



extern StateMachine CurrentState;

// Xử lý data frame CAN nhận về 
static void ProcessData(void);


static void DisplayThrottle (void);
static void DisplayLoad(void);

static void DisplaySpeed(void);
static void DisplayTorque(void);
static void DisplayCurrentVotol(void);
static void DisplayCurrentGenerator(void);
static void DisplayVoltageVotol(void);
static void DisplayVoltageGenerator(void);



void UIState(void)
{
    _ui_screen_change(&ui_UIState, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_UIState_screen_init);

    while (1)
    {
        // Nếu quên dòng này, ESP sẽ reset sau 5 giây
        esp_task_wdt_reset(); 
            
        if (twai_receive(&rx_message, pdMS_TO_TICKS(1000)) == ESP_OK) 
        {
            ProcessData();
        }
        
        if (CheckSTMConnection() == 0)
        {
            CurrentState = FAULT_STATE;
        }

        if (CurrentState != UI_STATE)
        {
            lv_anim_del_all();
            _ui_screen_delete(&ui_UIState);            
            return;
        }


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

        case COMM_LCD_RPM_TORQUE:
            Speed = rx_message.data[0] << 8 | rx_message.data[1] << 0;
            Torque = rx_message.data[2] << 24 | rx_message.data[3] << 16 | rx_message.data[4] << 8 | rx_message.data[5] << 0;
            DisplaySpeed();
            DisplayTorque();

            break;

        case COMM_LCD_CURRENT:
            CurrentVotol = rx_message.data[0] << 24 | rx_message.data[1] << 16 | rx_message.data[2] << 8 | rx_message.data[3] << 0;
            CurrentGenerator = rx_message.data[4] << 24 | rx_message.data[5] << 16 | rx_message.data[6] << 8 | rx_message.data[7] << 0;
            CurrentVotol /= 100;
            CurrentGenerator /= 100;
            DisplayCurrentVotol();
            DisplayCurrentGenerator();  
            break;

        case COMM_LCD_VOLTAGE:
            VoltageVotol = rx_message.data[0] << 24 | rx_message.data[1] << 16 | rx_message.data[2] << 8 | rx_message.data[3] << 0;
            VoltageGenerator = rx_message.data[4] << 24 | rx_message.data[5] << 16 | rx_message.data[6] << 8 | rx_message.data[7] << 0;

            VoltageVotol /=100;
            VoltageGenerator /=100;
            DisplayVoltageVotol();
            DisplayVoltageGenerator();
            break;
    }
}

// Hiển thị giá trị tay ga 
static void DisplayThrottle (void)
{
    // Cập nhật giá trị tay ga trên LCD 
    char Text[5];
    sprintf(Text, "%d", Throttle);
    lv_label_set_text(ui_Label37, Text);
}

static void DisplayLoad(void)
{
    if (LoadOnOff == SWITCH_OFF)
    {
        _ui_flag_modify(ui_Image7, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE);
        _ui_flag_modify(ui_Image15, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
    }
    else
    {
        _ui_flag_modify(ui_Image7, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
        _ui_flag_modify(ui_Image15, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE);
    }



    	
	switch (LoadLevel)
	{
		case LOAD_1:
			_ui_flag_modify(ui_Container18, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE);
			_ui_flag_modify(ui_Container39, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
            _ui_flag_modify(ui_Container40, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
            _ui_flag_modify(ui_Container41, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
			break;

		case LOAD_2:
            _ui_flag_modify(ui_Container18, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
            _ui_flag_modify(ui_Container39, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE);
            _ui_flag_modify(ui_Container40, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
            _ui_flag_modify(ui_Container41, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
			break;

		case LOAD_3:
            _ui_flag_modify(ui_Container18, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
            _ui_flag_modify(ui_Container39, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
            _ui_flag_modify(ui_Container40, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE);
            _ui_flag_modify(ui_Container41, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
			break;

		case LOAD_4:
            _ui_flag_modify(ui_Container18, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
            _ui_flag_modify(ui_Container39, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
            _ui_flag_modify(ui_Container40, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
            _ui_flag_modify(ui_Container41, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE);
			break;
	}
}


static void DisplaySpeed(void)
{
    // Cập nhật giá trị tốc độ trên LCD 
    char Text[10];
    sprintf(Text, "%d", Speed);
    lv_label_set_text(ui_Label44, Text);
}

static void DisplayTorque(void)
{
    // Cập nhật giá trị momen trên LCD 
    char Text[10];
    sprintf(Text, "%d", Torque);
    lv_label_set_text(ui_Label47, Text);
}

static void DisplayCurrentVotol(void)
{
    // Cập nhật giá trị dòng votol  trên LCD 
    char Text[10];
    sprintf(Text, "%0.2f", CurrentVotol);
    lv_label_set_text(ui_Label50, Text);
}

static void DisplayCurrentGenerator(void)
{
    // Cập nhật giá trị dòng máy phát  trên LCD 
    char Text[10];
    sprintf(Text, "%0.2f", CurrentGenerator);
    lv_label_set_text(ui_Label53, Text);
}

static void DisplayVoltageVotol(void)
{
    // Cập nhật giá trị điện áp votol  trên LCD 
    char Text[10];
    sprintf(Text, "%0.1f", VoltageVotol);
    lv_label_set_text(ui_Label56, Text);
}


static void DisplayVoltageGenerator(void)
{
    // Cập nhật giá trị điện áp máy phát  trên LCD 
    char Text[10];
    sprintf(Text, "%0.1f", VoltageGenerator);
    lv_label_set_text(ui_Label59, Text);
}


