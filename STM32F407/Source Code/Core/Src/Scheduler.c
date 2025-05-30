#include "Scheduler.h"


static Task_t tasks[MAX_TASKS];
static int numTasks = 0;

void scheduler_init(void)
{
    for (int i = 0; i < MAX_TASKS; i++) {
        tasks[i].task = 0;
        tasks[i].period = 0;
        tasks[i].counter = 0;
        tasks[i].ready = 0;
    }
}

int addTask(TaskFunction_t task, uint32_t period)
{
    if (numTasks < MAX_TASKS) {
        tasks[numTasks].task = task;
        tasks[numTasks].period = period;  // Thời gian chờ (ms)
        tasks[numTasks].counter = 0;
        tasks[numTasks].ready = 0;
        numTasks++;
        return 0;
    }
    return -1;  // Không thể thêm task nếu vượt quá MAX_TASKS
}

// Hàm này nên được gọi trong hàm ngắt timer (ví dụ: mỗi 1ms)
void scheduler_update(void)
{
    for (int i = 0; i < numTasks; i++) {
        tasks[i].counter++;
        if (tasks[i].counter >= tasks[i].period) {
            tasks[i].ready = 1;
            tasks[i].counter = 0;
        }
    }
}

// Hàm này được gọi trong vòng lặp chính (while(1))
void scheduler_run(void)
{
    for (int i = 0; i < numTasks; i++) {
        if (tasks[i].ready) {
            tasks[i].task();
            tasks[i].ready = 0;
        }
    }
}
