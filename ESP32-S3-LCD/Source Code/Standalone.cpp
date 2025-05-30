#include "StandaloneState.h"
#include "DataType.h"
#include "Communication.h"
#include "ui.h"
#include "SensorValue.h"
#include <stdio.h>


extern StateMachine CurrentState;

// Xử lý data frame CAN nhận về 
static void ProcessData(void);

// Vẽ đồ thị RPM với Torque
static void PlottingRPMAndTorque(void);

// Vẽ đồ thị Voltage 
static void PlottingVoltage(void);

// Vẽ đồ thị Current 
static void PlottingCurrent(void);


//====================================================================================================================
void StandaloneState(void)
{
    _ui_screen_change(&ui_StandAlone, LV_SCR_LOAD_ANIM_FADE_ON, 0, 0, &ui_StandAlone_screen_init);
    
   
    while (1)
    {




        // if (twai_receive(&rx_message, pdMS_TO_TICKS(1000)) == ESP_OK) 
        if (twai_receive(&rx_message, pdMS_TO_TICKS(100)) == ESP_OK) 
        {
            ProcessData();
        }
        
    

        if (CheckSTMConnection() == 0)
        {
            CurrentState = FAULT_STATE;
        }

        if (CurrentState != STANDALONE_STATE)
        {
            lv_anim_del_all();
            _ui_screen_delete(&ui_StandAlone);
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

        case COMM_LCD_RPM_TORQUE:
            Speed = (unsigned char)rx_message.data[0] << 8 | (unsigned char)rx_message.data[1] << 0;
            Torque = (unsigned int)rx_message.data[2] << 24 | (unsigned int)rx_message.data[3] << 16 | (unsigned int)rx_message.data[4] << 8 | (unsigned int)rx_message.data[5] << 0;
            PlottingRPMAndTorque();
            break;

        case COMM_LCD_CURRENT:
            CurrentVotol = (float)((unsigned int)rx_message.data[0] << 24 | (unsigned int)rx_message.data[1] << 16 | (unsigned int)rx_message.data[2] << 8 | (unsigned int)rx_message.data[3] << 0);
            CurrentGenerator = (float)((unsigned int)rx_message.data[4] << 24 | (unsigned int)rx_message.data[5] << 16 | (unsigned int)rx_message.data[6] << 8 | (unsigned int)rx_message.data[7] << 0);
            CurrentVotol /= 100;
            CurrentGenerator /= 100;
            PlottingCurrent();
            break;

        case COMM_LCD_VOLTAGE:
            VoltageVotol = (float)((unsigned int)rx_message.data[0] << 24 | (unsigned int)rx_message.data[1] << 16 | (unsigned int)rx_message.data[2] << 8 | (unsigned int)rx_message.data[3] << 0);
            VoltageGenerator = (float)((unsigned int)rx_message.data[4] << 24 | (unsigned int)rx_message.data[5] << 16 | (unsigned int)rx_message.data[6] << 8 | (unsigned int)rx_message.data[7] << 0);
            VoltageVotol /= 100;
            VoltageGenerator /=100;            
            PlottingVoltage();
            break;
    }
}

// Vẽ đồ thị RPM với Torque
static void PlottingRPMAndTorque(void)
{
    if (ui_StandAlone != NULL)
    {
        lv_chart_set_next_value(ui_Chart3, ui_Chart3_series_1, Speed);
        lv_chart_set_next_value(ui_Chart3, ui_Chart3_series_2, Torque);
        lv_chart_refresh(ui_Chart3);

        char Text[10];
        sprintf(Text, "%d", Speed);
        lv_label_set_text(ui_Label29, Text);

        sprintf(Text, "%d", Torque);
        lv_label_set_text(ui_Label31, Text);
    }

    if (ui_StandAlone1 != NULL)
    {
        lv_chart_set_next_value(ui_Chart7, ui_Chart7_series_1, Speed);
        lv_chart_set_next_value(ui_Chart7, ui_Chart7_series_2, Torque);
        lv_chart_refresh(ui_Chart7);

        char Text[10];
        sprintf(Text, "%d", Speed);
        lv_label_set_text(ui_Label137, Text);

        sprintf(Text, "%d", Torque);
        lv_label_set_text(ui_Label140, Text);
    }
}

// Vẽ đồ thị Voltage 
static void PlottingVoltage(void)
{

    if (ui_StandAlone != NULL)
    {
        lv_chart_set_next_value(ui_Chart4, ui_Chart4_series_1, VoltageVotol);
        lv_chart_set_next_value(ui_Chart4, ui_Chart4_series_2, VoltageGenerator);
        lv_chart_refresh(ui_Chart4);

        char Text[10];
        sprintf(Text, "%0.1f", VoltageVotol);
        lv_label_set_text(ui_Label70, Text);

        sprintf(Text, "%0.1f", VoltageGenerator);
        lv_label_set_text(ui_Label63, Text);
    }

    if (ui_StandAlone2 != NULL)
    {
        lv_chart_set_next_value(ui_Chart9, ui_Chart9_series_1, VoltageVotol);
        lv_chart_set_next_value(ui_Chart9, ui_Chart9_series_2, VoltageGenerator);
        lv_chart_refresh(ui_Chart9);

        char Text[10];
        sprintf(Text, "%0.1f", VoltageVotol);
        lv_label_set_text(ui_Label160, Text);

        sprintf(Text, "%0.1f", VoltageGenerator);
        lv_label_set_text(ui_Label156, Text);
    }
    
}

// Vẽ đồ thị Current 
static void PlottingCurrent(void)
{
    if (ui_StandAlone != NULL)
    {
        lv_chart_set_next_value(ui_Chart4, ui_Chart4_series_3, CurrentVotol);
        lv_chart_set_next_value(ui_Chart4, ui_Chart4_series_4, CurrentGenerator);
        lv_chart_refresh(ui_Chart4);

        char Text[10];
        sprintf(Text, "%0.2f", CurrentVotol);
        lv_label_set_text(ui_Label66, Text);

        sprintf(Text, "%0.2f", CurrentGenerator);
        lv_label_set_text(ui_Label75, Text);
    }

    if (ui_StandAlone2 != NULL)
    {
        lv_chart_set_next_value(ui_Chart9, ui_Chart9_series_3, CurrentVotol);
        lv_chart_set_next_value(ui_Chart9, ui_Chart9_series_4, CurrentGenerator);
        lv_chart_refresh(ui_Chart9);

        char Text[10];
        sprintf(Text, "%0.2f", CurrentVotol);
        lv_label_set_text(ui_Label168, Text);

        sprintf(Text, "%0.2f", CurrentGenerator);
        lv_label_set_text(ui_Label164, Text);
    }
}