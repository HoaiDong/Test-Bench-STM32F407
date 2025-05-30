#include "Flash.h"
#include "DataType.h"

// Hàm ghi giá trị vào FLASH
void FLASH_Write(uint32_t address, uint32_t Speed_Disc_Tooth, uint32_t Cur_Sensitive, uint32_t Torque_Sensitive)
{
    HAL_FLASH_Unlock(); // Mở khóa FLASH

    // Xóa sector
    FLASH_EraseInitTypeDef eraseInit;
    uint32_t sectorError = 0;

    eraseInit.TypeErase = FLASH_TYPEERASE_SECTORS;
    eraseInit.Sector = FLASH_SECTOR_11; // Sector cần xóa
    eraseInit.NbSectors = 1; // Số sector cần xóa
    eraseInit.VoltageRange = FLASH_VOLTAGE_RANGE_3; // Chọn mức điện áp (3.3V)

    HAL_FLASHEx_Erase(&eraseInit, &sectorError);

    // Ghi dữ liệu số răng đĩa tốc độ
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address, Speed_Disc_Tooth);

    // Ghi dữ liệu độ nhạy cảm biến dòng
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address + 4, Cur_Sensitive);

    // Ghi dữ liệu độ nhạy cảm biến momen
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address + 8, Torque_Sensitive);

    HAL_FLASH_Lock(); // Khóa FLASH
}

// Hàm đọc giá trị từ FLASH
uint32_t FLASH_Read(uint32_t address)
{
    return (*(volatile int*)address); // Đọc trực tiếp từ địa chỉ
}


