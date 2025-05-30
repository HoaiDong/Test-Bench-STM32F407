/*
 * Flash.h
 *
 *  Created on: May 5, 2025
 *      Author: Admin
 */

#ifndef INC_FLASH_H_
#define INC_FLASH_H_

#include <stdint.h>

// Hàm ghi giá trị vào FLASH
void FLASH_Write(uint32_t address, uint32_t Speed_Disc_Tooth, uint32_t Cur_Sensitive, uint32_t Torque_Sensitive);

// Hàm đọc giá trị từ FLASH
uint32_t FLASH_Read(uint32_t address);


#endif /* INC_FLASH_H_ */
