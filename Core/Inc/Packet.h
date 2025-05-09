#ifndef PACKET_H
#define PACKET_H

#include <stdint.h>


#define BUFFER_SIZE  999  // Kích thước bộ đệm

extern uint8_t DataFrame[BUFFER_SIZE]; // Bộ đệm lưu trữ packet data lấy được từ buffer để xử lý

// Sử dụng kĩ thuật circular buffer để quản lý data từ serial port
extern uint8_t CircularBuffer[BUFFER_SIZE];  // Circular buffer
extern uint16_t HeadBuffer;  // Thể hiện vị trí tiếp theo trong buffer để nhận data
extern uint16_t TailBuffer;  // Thể hiện vị trí data cũ nhất trong buffer
extern uint16_t Count; // Số lượng byte có trong buffer




// Các hàm quản lý buffer
uint8_t IsBufferEmpty(void); // Kiểm tra Circular Buffer có đang empty data hay không. True là empty, false là không empty
uint8_t IsBufferFull(void);  // Kiểm tra Circular Buffer có đang full data hay không. True là full, false là chưa full.
void EnqueueBuffer(uint8_t data); // Thêm 1 data vào Circular Buffer
uint8_t DequeueBuffer(void);  // Lấy 1 data từ Circular Buffer ra




int16_t TryDecodePacket(void);  //Hàm kiểm tra frame của data nhận về có đúng không để xử lý


uint8_t* PacketData(uint8_t arr[], uint16_t size);  //Hàm đóng gói dữ liệu UART

#endif
