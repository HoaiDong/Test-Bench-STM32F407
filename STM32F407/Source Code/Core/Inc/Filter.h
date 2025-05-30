#ifndef FILTER_H
#define FILTER_H

#include <stdint.h>

//=========================== Bộ lọc Kalman =============================
// Cấu trúc bộ lọc Kalman
typedef struct 
{
    float q;     // Nhiễu quá trình (độ không chắc trong mô hình)
    float r;     // Nhiễu đo lường (độ không chắc trong giá trị đo)
    float x;     // Giá trị ước lượng hiện tại
    float p;     // Phương sai của lỗi ước lượng
    float k;     // Hệ số Kalman (Kalman gain)
} KalmanFilter;

/**
 * @brief Khởi tạo bộ lọc Kalman
 * @param kf: con trỏ đến biến bộ lọc
 * @param q: độ nhiễu của quá trình (process noise)
 * @param r: độ nhiễu của đo lường (measurement noise)
 * @param initial_value: giá trị ước lượng ban đầu
 */
void Kalman_Init(KalmanFilter* kf, float q, float r, float initial_value);

/**
 * @brief Cập nhật bộ lọc với một giá trị đo mới
 * @param kf: con trỏ đến biến bộ lọc
 * @param measurement: giá trị đo được (ADC, tốc độ...)
 * @return giá trị đã được lọc
 */
float Kalman_Update(KalmanFilter* kf, float measurement);

/**
 * @brief Cập nhật Kalman filter với loại bỏ spike (nhiễu đột biến)
 * 
 * Nếu giá trị đo lệch quá lớn so với giá trị ước lượng hiện tại, 
 * thì sẽ coi là nhiễu và không cập nhật giá trị mới.
 * 
 * @param kf: con trỏ đến bộ lọc Kalman
 * @param measurement: giá trị đo từ cảm biến (ADC, dòng, torque...)
 * @param max_jump: ngưỡng cho phép của sai lệch (nếu lớn hơn thì bỏ qua)
 * @return Giá trị sau lọc (giá trị ước lượng hiện tại hoặc đã cập nhật)
 */
float Kalman_Update_WithSpikeReject(KalmanFilter* kf, float measurement, float max_jump);

//=========================== Bộ lọc Kalman =============================



//=========================== Bộ lọc Median =============================
#define MEDIAN_FILTER_MAX_SIZE  25   // Điều chỉnh tùy nhu cầu

typedef struct {
    uint16_t buffer[MEDIAN_FILTER_MAX_SIZE];
    uint8_t  size;     // median filter size thực sự (3, 5, 7, ...)
    uint8_t  index;    // vị trí ghi tiếp theo
    uint8_t  filled;   // số phần tử hiện có (lúc đầu < size, sau sẽ bằng size)
} MedianFilter_t;

// Khởi tạo filter với kích thước size mong muốn (size <= MEDIAN_FILTER_MAX_SIZE)
void MedianFilter_Init(MedianFilter_t *filter, uint8_t size);

// Thêm giá trị mới vào filter
void MedianFilter_Put(MedianFilter_t *filter, uint16_t value);

// Lấy giá trị median hiện tại
uint16_t MedianFilter_Get(MedianFilter_t *filter);
//=========================== Bộ lọc Median =============================


//=========================== Bộ lọc EMA ================================
/**
 * @brief Cấu trúc cho bộ lọc EMA (Exponential Moving Average).
 *        Dùng cho tín hiệu speed, RPM, áp suất, nhiệt độ, v.v.
 * 
 * value:  Giá trị đã lọc (state)
 * alpha:  Hệ số làm mượt (0 < alpha < 1), càng cao càng ít mượt (bám tín hiệu nhanh hơn)
 * init:   Cờ kiểm tra đã khởi tạo lần đầu chưa
 */
typedef struct {
    float value;
    float alpha;
    uint8_t init;
} EMAFilter_t;

/**
 * @brief Khởi tạo bộ lọc EMA
 * @param filter: Con trỏ tới bộ lọc EMA
 * @param alpha:  Hệ số lọc (0 < alpha < 1). Khuyến nghị 0.15–0.3 cho speed/RPM.
 */
void EMAFilter_Init(EMAFilter_t *filter, float alpha);

/**
 * @brief Cập nhật bộ lọc với giá trị mới
 * @param filter:     Con trỏ tới bộ lọc EMA
 * @param new_value:  Giá trị speed/RPM mới cần lọc
 * @return Giá trị đã lọc mượt
 */
float EMAFilter_Update(EMAFilter_t *filter, float new_value);
//=========================== Bộ lọc EMA ================================


#endif
