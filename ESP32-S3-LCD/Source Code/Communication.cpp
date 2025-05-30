#include "Communication.h"
#include "DataType.h"


// Cấu hình chân GPIO cho TWAI (CAN)
#define TWAI_TX_PIN GPIO_NUM_20
#define TWAI_RX_PIN GPIO_NUM_19




twai_message_t rx_message;

void InitCAN(void)
{
    // Cấu hình chung cho TWAI
    twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(TWAI_TX_PIN, TWAI_RX_PIN, TWAI_MODE_NORMAL);
    // Cấu hình tốc độ truyền 500 kbps
    twai_timing_config_t t_config = TWAI_TIMING_CONFIG_500KBITS();
    // Cấu hình bộ lọc để chấp nhận tất cả các khung dữ liệu
    twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();

    // Cài đặt driver TWAI
    twai_driver_install(&g_config, &t_config, &f_config);


    // Bắt đầu TWAI
    twai_start();
}

// Gửi tín hiệu kết nối alive cho STM32 
void CommLCDConnectAlive(void)
{

    twai_clear_transmit_queue();
    // twai_clear_receive_queue();


    // Tạo khung dữ liệu để truyền
    twai_message_t tx_message;
    tx_message.identifier = COMM_LCD_CONNECT_ALIVE; // ID của khung dữ liệu
    tx_message.extd = 0; // Sử dụng khung tiêu chuẩn (11-bit ID)
    tx_message.rtr = 0; // Khung dữ liệu
    tx_message.data_length_code = 0; // Độ dài dữ liệu là 0 byte

    // Truyền khung dữ liệu
    if (twai_transmit(&tx_message, pdMS_TO_TICKS(1000)) == ESP_OK)
    {
    }
    else
    {
        checkAndRecoverTWAI();
    }

    STM32ConnectAlive();
}

void CommLCDRunSystem(void)
{
    twai_clear_transmit_queue();

    // Tạo khung dữ liệu để truyền
    twai_message_t tx_message;
    tx_message.identifier = COMM_LCD_TEST_SYSTEM; // ID của khung dữ liệu
    tx_message.extd = 0; // Sử dụng khung tiêu chuẩn (11-bit ID)
    tx_message.rtr = 0; // Khung dữ liệu
    tx_message.data_length_code = 0; // Độ dài dữ liệu là 0 byte

    // Truyền khung dữ liệu
    if (twai_transmit(&tx_message, pdMS_TO_TICKS(1000)) == ESP_OK)
    {
        
    }
    else
    {
        checkAndRecoverTWAI();
    }
}

// Hàm đọc CAN
unsigned char CANReceive(void)
{
    // Nhận khung dữ liệu
    return twai_receive(&rx_message, pdMS_TO_TICKS(1000));
}


void checkAndRecoverTWAI() {

            twai_clear_transmit_queue();
    twai_clear_receive_queue();

    
    twai_status_info_t status;
    twai_get_status_info(&status);

    Serial.print("TWAI State: "); Serial.println(status.state);
    Serial.print("RX Error Count: "); Serial.println(status.rx_error_counter);
    Serial.print("TX Error Count: "); Serial.println(status.tx_error_counter);

    if (status.state == TWAI_STATE_BUS_OFF) {
        Serial.println("Bus-off detected! Recovering...");
        twai_stop();
        twai_initiate_recovery();
        delay(100);
        if (twai_start() == ESP_OK) {
            Serial.println("TWAI Recovered");
        } else {
            Serial.println("TWAI Recovery Failed");
        }
    }

    
}
