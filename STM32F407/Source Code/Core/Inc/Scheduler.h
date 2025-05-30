#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>

#define MAX_TASKS 10

typedef void (*TaskFunction_t)(void);  // Kiểu hàm của các task

typedef struct {
    TaskFunction_t task;   // Con trỏ hàm của task
    uint32_t period;       // Khoảng thời gian (ms) giữa các lần chạy
    uint32_t counter;      // Bộ đếm ms 
    uint8_t ready;         // Cờ đánh dấu task đã sẵn sàng chạy
} Task_t;


// Khởi tạo scheduler
void scheduler_init(void);
// Thêm task mới vào scheduler, trả về 0 nếu thành công, -1 nếu hết chỗ
int addTask(TaskFunction_t task, uint32_t period);
// Hàm cập nhật task (được gọi từ timer interrupt, ví dụ mỗi 1ms)
void scheduler_update(void);
// Hàm kiểm tra và thực thi các task sẵn sàng (gọi trong vòng lặp chính)
void scheduler_run(void);

#endif
