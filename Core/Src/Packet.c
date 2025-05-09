#include "Packet.h"
#include <string.h>

uint8_t DataFrame[BUFFER_SIZE]; // Bộ đệm lưu trữ packet data lấy được từ buffer để xử lý


uint8_t CircularBuffer[BUFFER_SIZE];  // Circular buffer

uint16_t HeadBuffer = 0;  // Thể hiện vị trí tiếp theo trong buffer để nhận data
uint16_t TailBuffer = 0;  // Thể hiện vị trí data cũ nhất trong buffer
uint16_t Count = 0; // Số lượng byte có trong buffer

static const unsigned short CRC16Tabble[] = { 0x0000, 0x1021, 0x2042, 0x3063, 0x4084,
  0x50a5, 0x60c6, 0x70e7, 0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad,
  0xe1ce, 0xf1ef, 0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7,
  0x62d6, 0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,
  0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485, 0xa56a,
  0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d, 0x3653, 0x2672,
  0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4, 0xb75b, 0xa77a, 0x9719,
  0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc, 0x48c4, 0x58e5, 0x6886, 0x78a7,
  0x0840, 0x1861, 0x2802, 0x3823, 0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948,
  0x9969, 0xa90a, 0xb92b, 0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50,
  0x3a33, 0x2a12, 0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b,
  0xab1a, 0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41,
  0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49, 0x7e97,
  0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70, 0xff9f, 0xefbe,
  0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78, 0x9188, 0x81a9, 0xb1ca,
  0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f, 0x1080, 0x00a1, 0x30c2, 0x20e3,
  0x5004, 0x4025, 0x7046, 0x6067, 0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d,
  0xd31c, 0xe37f, 0xf35e, 0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214,
  0x6277, 0x7256, 0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c,
  0xc50d, 0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
  0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c, 0x26d3,
  0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634, 0xd94c, 0xc96d,
  0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab, 0x5844, 0x4865, 0x7806,
  0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3, 0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e,
  0x8bf9, 0x9bd8, 0xabbb, 0xbb9a, 0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1,
  0x1ad0, 0x2ab3, 0x3a92, 0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b,
  0x9de8, 0x8dc9, 0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0,
  0x0cc1, 0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8,
  0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0 
};



// FUNCTION PROTOTYPE
//========================================================================================
static uint16_t CRC16(uint16_t Tail, uint32_t len);  // Tính giá trị CRC 16 cho packet data
//Hàm tạo CRC16 cho đóng gói dữ liệu
static uint16_t CRC16Packet(uint8_t* buffer, uint16_t length);
//========================================================================================




// Kiểm tra Circular Buffer có đang empty data hay không. True là empty, false là không empty
uint8_t IsBufferEmpty(void) 
{
    return Count == 0;
}

// Kiểm tra Circular Buffer có đang full data hay không. True là full, false là chưa full. 
uint8_t IsBufferFull(void)  
{
    return Count == BUFFER_SIZE;
}

// Hàm thêm dữ liệu vào Circular Buffer
void EnqueueBuffer(uint8_t data)
{
    if (IsBufferFull() == 0)
    {
        CircularBuffer[HeadBuffer] = data;
        HeadBuffer = (HeadBuffer + 1) % BUFFER_SIZE;
        Count++;
    }
    else
    {
    	HeadBuffer = 0;  // Thể hiện vị trí tiếp theo trong buffer để nhận data
    	TailBuffer = 0;  // Thể hiện vị trí data cũ nhất trong buffer
    	Count = 0; // Số lượng byte có trong buffer
    }
}

// Lấy 1 data từ Circular Buffer ra
uint8_t DequeueBuffer(void)  
{
    if (IsBufferEmpty() == 0)
    {
        uint8_t Value = CircularBuffer[TailBuffer];
        TailBuffer = (TailBuffer + 1) % BUFFER_SIZE;
        Count--;
        return Value;
    }
}


// Hàm tính giá trị CRC 16 của data chứa trong frame của RING BUFFER 
uint16_t CRC16(uint16_t Tail, uint32_t len)
{
  uint16_t CheckSum = 0;
  for (uint32_t i = 0; i < len; i++)
  {
	  CheckSum = CRC16Tabble[(((CheckSum >> 8) ^ CircularBuffer[Tail]) & 0xFF)] ^ (CheckSum << 8);
    Tail = (Tail + 1) % BUFFER_SIZE;
  }
  return CheckSum;
}


//Hàm kiểm tra frame của data nhận về có đúng không để xử lý
int16_t TryDecodePacket(void) 
{
  uint8_t DataStart = CircularBuffer[TailBuffer];   // Lấy giá trị start byte của frame

  // Start byte không hợp lệ sẽ end hàm
  if (DataStart != 2) 
  {
    return -1;
  }

  uint16_t DataLength = 0;  // Giá trị Lenght Byte
  // Lấy giá trị Length Byte
  if (DataStart == 2) 
  {
    DataLength = (uint16_t)CircularBuffer[(TailBuffer + 1) % BUFFER_SIZE];  // Lấy giá trị Length Byte

    // End hàm nếu như giá trị Length đó < 1, không có byte nào hết
    if (DataLength < 1) 
    {
    				return -1;
    }
  }

  // Số lượng byte hiện có phải nhiều hơn số lượng byte của 1 data frame thì mới cho kiểm tra tiếp 
  if (Count < DataLength + 5)
  {
	  return -2;
  }

  // Kiểm tra End Byte có hợp lệ không, nếu không thì end hàm với End Byte có giá trị 0x03
  if (CircularBuffer[(TailBuffer + DataStart + DataLength + 2) % BUFFER_SIZE] != 3) 
  {
      return -1;
  }

  // Xử dụng hàm CRC16 để tính giá trị crc cho data nhận được trong frame
  uint16_t CRC_Calc = CRC16((TailBuffer + DataStart) % BUFFER_SIZE, DataLength);

  // Xử lý little endian 2 byte CRC trong frame
  uint16_t CRC_Rx = (uint16_t)CircularBuffer[(TailBuffer + DataStart + DataLength) % BUFFER_SIZE] << 8 | 
                          (uint16_t)CircularBuffer[(TailBuffer + DataStart + DataLength + 1) % BUFFER_SIZE]; //Chuyển sang little endian của crc trong buffer

  // Kiểm tra crc tính được và crc có trong frame có hợp lệ không
  if (CRC_Calc == CRC_Rx) 
  {
    // Lấy 1 data frame từ Circular Buffer ra 1 mảng riêng để dễ xử lý
    for(int i = 0; i < DataLength + 5; i++)
    {
      DataFrame[i] = DequeueBuffer();
    }
    return 1;
  } 
  else 
  {   
      DequeueBuffer();
      return -1;
  }
}


//Hàm đóng gói dữ liệu
uint8_t* PacketData(uint8_t arr[], uint16_t size)
{
  //Tạo khung dữ liệu
  static uint8_t packetdata[256];
  memset(packetdata, 0, 256);     //Đặt giá trị mặc định cho toàn bộ khung

  //Thêm start byte và length byte
  packetdata[0] = 0x02;
  packetdata[1] = size;

  //Thêm dữ liệu vào khung theo kiểu big edian
  for(int i = 0; i < size; i++) 
  {
    packetdata[2 + i] = arr[i];
  }

  //Tạo 2 byte CRC16 và thêm vào khung theo kiểu big edian
  uint16_t crc16 = CRC16Packet(arr, size);
  packetdata[size - 1 + 3] = (uint8_t)(crc16 >> 8);
  packetdata[size - 1 + 4] = (uint8_t)(crc16 & 0xFF); 

  //Thêm end byte vào cuối khung
  packetdata[size - 1 + 5] = 0x03; 

  return packetdata;
}

//Hàm tạo CRC16 cho đóng gói dữ liệu
static uint16_t CRC16Packet(uint8_t* buffer, uint16_t length)
{
  unsigned short cksum = 0;

  for (unsigned int i = 0; i < length; i++) 
  {
    cksum = CRC16Tabble[(((cksum >> 8) ^ *buffer++) & 0xFF)] ^ (cksum << 8);
  }

  return cksum;
}
