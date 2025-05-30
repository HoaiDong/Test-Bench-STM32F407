#include "Filter.h"
#include <math.h>   // dùng fabsf
#include <string.h> // for memset, memcpy

//=========================== Bộ lọc Kalman =============================
void Kalman_Init(KalmanFilter* kf, float q, float r, float initial_value) 
{
    kf->q = q;                  // Nhiễu mô hình
    kf->r = r;                  // Nhiễu cảm biến
    kf->x = initial_value;      // Giá trị khởi tạo
    kf->p = 1.0f;               // Phương sai ban đầu
    kf->k = 0.0f;               // Kalman gain ban đầu
}

float Kalman_Update(KalmanFilter* kf, float measurement) 
{
    // Cập nhật dự đoán (prediction)
    kf->p += kf->q;

    // Cập nhật đo lường (measurement)
    kf->k = kf->p / (kf->p + kf->r);
    kf->x += kf->k * (measurement - kf->x);
    kf->p *= (1.0f - kf->k);

    return kf->x;
}



float Kalman_Update_WithSpikeReject(KalmanFilter* kf, float measurement, float max_jump) 
{
    // Nếu độ lệch giữa giá trị đo và ước lượng vượt quá ngưỡng cho phép
    if (fabsf(measurement - kf->x) > max_jump) {
        // Coi là nhiễu đột biến (spike) ⇒ bỏ qua, giữ nguyên giá trị cũ
        return kf->x;
    }

    // Nếu không bị spike ⇒ cập nhật bình thường
    return Kalman_Update(kf, measurement);
}
//=========================== Bộ lọc Kalman =============================







//=========================== Bộ lọc Median =============================
// Khởi tạo filter
void MedianFilter_Init(MedianFilter_t *filter, uint8_t size) {
    if (size > MEDIAN_FILTER_MAX_SIZE) size = MEDIAN_FILTER_MAX_SIZE;
    if (size < 1) size = 1; // Không cho phép size = 0
    filter->size = size;
    filter->index = 0;
    filter->filled = 0;
    memset(filter->buffer, 0, sizeof(filter->buffer));
}

// Thêm giá trị mới vào filter
void MedianFilter_Put(MedianFilter_t *filter, uint16_t value) {
    filter->buffer[filter->index] = value;
    filter->index = (filter->index + 1) % filter->size;
    if (filter->filled < filter->size) filter->filled++;
}

// Lấy giá trị median hiện tại
uint16_t MedianFilter_Get(MedianFilter_t *filter) {
    uint16_t temp[MEDIAN_FILTER_MAX_SIZE];
    uint8_t n = filter->filled ? filter->filled : 1;  // Tránh chia cho 0

    memcpy(temp, filter->buffer, n * sizeof(uint16_t));
    // Sắp xếp mảng temp để tìm median (bubble sort, nhanh với size nhỏ)
    for (uint8_t i = 0; i < n - 1; i++) {
        for (uint8_t j = i + 1; j < n; j++) {
            if (temp[i] > temp[j]) {
                uint16_t t = temp[i];
                temp[i] = temp[j];
                temp[j] = t;
            }
        }
    }
    // Lấy giá trị giữa
    if (n % 2 == 1) {
        return temp[n / 2];
    } else {
        return (temp[n / 2 - 1] + temp[n / 2]) / 2;
    }
}
//=========================== Bộ lọc Median =============================


//=========================== Bộ lọc EMA ================================
/**
 * @brief Khởi tạo bộ lọc EMA.
 *        Đặt giá trị ban đầu, hệ số alpha và cờ init.
 */
void EMAFilter_Init(EMAFilter_t *filter, float alpha) {
    filter->value = 0.0f;
    filter->alpha = alpha;
    filter->init = 0; // Chưa nhận giá trị đầu tiên
}

/**
 * @brief Cập nhật bộ lọc EMA với giá trị mới.
 *        Lần đầu tiên sẽ nhận giá trị ngay; sau đó áp dụng EMA.
 * 
 * @param filter     Con trỏ tới bộ lọc EMA
 * @param new_value  Giá trị speed/RPM mới (float)
 * @return           Giá trị đã lọc (float)
 */
float EMAFilter_Update(EMAFilter_t *filter, float new_value) {
    if (!filter->init) {
        filter->value = new_value; // Lần đầu tiên, lấy giá trị ngay
        filter->init = 1;
    } else {
        filter->value = filter->alpha * new_value + (1.0f - filter->alpha) * filter->value;
    }
    return filter->value;
}
//=========================== Bộ lọc EMA ================================



