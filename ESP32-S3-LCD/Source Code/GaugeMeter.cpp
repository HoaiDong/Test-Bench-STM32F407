#include "GaugeMeter.h"
#include "lvgl.h"

// Biến lưu kim đồng hồ để cập nhật sau này
static lv_obj_t *meter;
static lv_meter_indicator_t *needle;
static lv_obj_t *label_value;

void ui_throttle_meter_create(void) {
    // Tạo meter
    meter = lv_meter_create(lv_scr_act());
    lv_obj_set_size(meter, 300, 300);
    lv_obj_center(meter);

    // Thêm scale (vạch chia)
    lv_meter_scale_t *scale = lv_meter_add_scale(meter);

    // Vạch nhỏ: 1 đơn vị (0 - 100)
    lv_meter_set_scale_ticks(meter, scale, 101, 2, 10, lv_palette_main(LV_PALETTE_GREY));

    // Vạch lớn: mỗi 10 đơn vị
    lv_meter_set_scale_major_ticks(meter, scale, 10, 4, 20, lv_color_black(), 10);

    // Góc hiển thị: 135 đến 45 độ (tổng 270 độ)
    lv_meter_set_scale_range(meter, scale, 0, 100, 270, 135);

    // Kim chỉ giá trị throttle
    needle = lv_meter_add_needle_line(meter, scale, 4, lv_palette_main(LV_PALETTE_BLUE), -10);

    // Label hiển thị giá trị số
    label_value = lv_label_create(lv_scr_act());
    lv_obj_set_style_text_font(label_value, &lv_font_montserrat_48, 0);
    lv_label_set_text(label_value, "0%");
    lv_obj_align(label_value, LV_ALIGN_CENTER, 0, -20);

    // Label đơn vị (Throttle)
    lv_obj_t *label_unit = lv_label_create(lv_scr_act());
    lv_label_set_text(label_unit, "Throttle");
    lv_obj_align(label_unit, LV_ALIGN_CENTER, 0, 40);
}

// Hàm cập nhật giá trị throttle từ 0–100
void ui_throttle_meter_update(uint8_t throttle_value) {
    if(throttle_value > 100) throttle_value = 100; // Giới hạn

    lv_meter_set_indicator_value(meter, needle, throttle_value);
    lv_label_set_text_fmt(label_value, "%d%%", throttle_value);
}
