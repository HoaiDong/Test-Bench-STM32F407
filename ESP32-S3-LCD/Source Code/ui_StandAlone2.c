// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.5.1
// LVGL version: 8.3.11
// Project name: Demo2

#include "ui.h"

void ui_StandAlone2_screen_init(void)
{
    ui_StandAlone2 = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_StandAlone2, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_flex_flow(ui_StandAlone2, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(ui_StandAlone2, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_add_event_cb(ui_StandAlone2, scr_unloaded_delete_cb, LV_EVENT_SCREEN_UNLOADED, &ui_StandAlone2);
    lv_obj_set_style_bg_color(ui_StandAlone2, lv_color_hex(0xF0F0F0), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_StandAlone2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui_StandAlone2, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_StandAlone2, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_StandAlone2, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_StandAlone2, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_row(ui_StandAlone2, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(ui_StandAlone2, 8, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Panel33 = lv_obj_create(ui_StandAlone2);
    lv_obj_set_width(ui_Panel33, lv_pct(80));
    lv_obj_set_height(ui_Panel33, lv_pct(8));
    lv_obj_set_x(ui_Panel33, -1);
    lv_obj_set_y(ui_Panel33, -194);
    lv_obj_set_align(ui_Panel33, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_Panel33, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE |
                      LV_OBJ_FLAG_GESTURE_BUBBLE | LV_OBJ_FLAG_SNAPPABLE | LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC |
                      LV_OBJ_FLAG_SCROLL_MOMENTUM | LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags
    lv_obj_set_style_bg_color(ui_Panel33, lv_color_hex(0xD9D9D9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Panel33, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_Panel33, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_Panel33, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label94 = lv_label_create(ui_Panel33);
    lv_obj_set_width(ui_Label94, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label94, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_Label94, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label94, "STAND ALONE");
    lv_obj_clear_flag(ui_Label94, LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE | LV_OBJ_FLAG_GESTURE_BUBBLE |
                      LV_OBJ_FLAG_SNAPPABLE | LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                      LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags
    lv_obj_set_style_text_color(ui_Label94, lv_color_hex(0x111111), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Label94, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Label94, &lv_font_montserrat_26, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui_Label94, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_Label94, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_Label94, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_Label94, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_row(ui_Label94, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(ui_Label94, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Container47 = lv_obj_create(ui_StandAlone2);
    lv_obj_remove_style_all(ui_Container47);
    lv_obj_set_width(ui_Container47, lv_pct(100));
    lv_obj_set_height(ui_Container47, lv_pct(92));
    lv_obj_set_align(ui_Container47, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_Container47, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE |
                      LV_OBJ_FLAG_GESTURE_BUBBLE | LV_OBJ_FLAG_SNAPPABLE | LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC |
                      LV_OBJ_FLAG_SCROLL_MOMENTUM | LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags

    ui_Container62 = lv_obj_create(ui_Container47);
    lv_obj_remove_style_all(ui_Container62);
    lv_obj_set_width(ui_Container62, lv_pct(101));
    lv_obj_set_height(ui_Container62, lv_pct(100));
    lv_obj_set_x(ui_Container62, 0);
    lv_obj_set_y(ui_Container62, 1);
    lv_obj_set_align(ui_Container62, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_Container62, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_pad_left(ui_Container62, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_Container62, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_Container62, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_Container62, 8, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Panel40 = lv_obj_create(ui_Container62);
    lv_obj_set_width(ui_Panel40, 461);
    lv_obj_set_height(ui_Panel40, 68);
    lv_obj_set_x(ui_Panel40, 0);
    lv_obj_set_y(ui_Panel40, 171);
    lv_obj_set_align(ui_Panel40, LV_ALIGN_CENTER);
    lv_obj_set_flex_flow(ui_Panel40, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(ui_Panel40, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_clear_flag(ui_Panel40, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_border_color(ui_Panel40, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_Panel40, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_row(ui_Panel40, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(ui_Panel40, 100, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Container63 = lv_obj_create(ui_Panel40);
    lv_obj_remove_style_all(ui_Container63);
    lv_obj_set_width(ui_Container63, lv_pct(45));
    lv_obj_set_height(ui_Container63, lv_pct(266));
    lv_obj_set_align(ui_Container63, LV_ALIGN_CENTER);
    lv_obj_set_flex_flow(ui_Container63, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(ui_Container63, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_clear_flag(ui_Container63, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_Container65 = lv_obj_create(ui_Container63);
    lv_obj_remove_style_all(ui_Container65);
    lv_obj_set_width(ui_Container65, 186);
    lv_obj_set_height(ui_Container65, lv_pct(50));
    lv_obj_set_x(ui_Container65, 2);
    lv_obj_set_y(ui_Container65, -110);
    lv_obj_set_align(ui_Container65, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_Container65, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_pad_left(ui_Container65, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_Container65, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_Container65, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_Container65, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_row(ui_Container65, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(ui_Container65, 8, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label159 = lv_label_create(ui_Container65);
    lv_obj_set_width(ui_Label159, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label159, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label159, -66);
    lv_obj_set_y(ui_Label159, -3);
    lv_obj_set_align(ui_Label159, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label159, "V");
    lv_obj_set_style_text_color(ui_Label159, lv_color_hex(0xDBD00C), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Label159, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Label159, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label160 = lv_label_create(ui_Container65);
    lv_obj_set_width(ui_Label160, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label160, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label160, 17);
    lv_obj_set_y(ui_Label160, -3);
    lv_obj_set_align(ui_Label160, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label160, "0");
    ui_object_set_themeable_style_property(ui_Label160, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR,
                                           _ui_theme_color_Black);
    ui_object_set_themeable_style_property(ui_Label160, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_OPA,
                                           _ui_theme_alpha_Black);
    lv_obj_set_style_text_font(ui_Label160, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label161 = lv_label_create(ui_Container65);
    lv_obj_set_width(ui_Label161, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label161, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label161, 68);
    lv_obj_set_y(ui_Label161, -3);
    lv_obj_set_align(ui_Label161, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label161, "V");
    lv_obj_set_style_text_color(ui_Label161, lv_color_hex(0xDBD00C), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Label161, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Label161, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label162 = lv_label_create(ui_Container65);
    lv_obj_set_width(ui_Label162, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label162, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label162, -39);
    lv_obj_set_y(ui_Label162, 7);
    lv_obj_set_align(ui_Label162, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label162, "Votol");
    lv_obj_set_style_text_color(ui_Label162, lv_color_hex(0xDBD00C), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Label162, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Label162, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Container68 = lv_obj_create(ui_Container63);
    lv_obj_remove_style_all(ui_Container68);
    lv_obj_set_width(ui_Container68, lv_pct(100));
    lv_obj_set_height(ui_Container68, lv_pct(50));
    lv_obj_set_x(ui_Container68, 1);
    lv_obj_set_y(ui_Container68, 38);
    lv_obj_set_align(ui_Container68, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_Container68, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_pad_left(ui_Container68, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_Container68, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_Container68, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_Container68, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_row(ui_Container68, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(ui_Container68, 8, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label167 = lv_label_create(ui_Container68);
    lv_obj_set_width(ui_Label167, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label167, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label167, -64);
    lv_obj_set_y(ui_Label167, 1);
    lv_obj_set_align(ui_Label167, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label167, "I");
    lv_obj_set_style_text_color(ui_Label167, lv_color_hex(0xFA1B2A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Label167, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Label167, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label168 = lv_label_create(ui_Container68);
    lv_obj_set_width(ui_Label168, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label168, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label168, 18);
    lv_obj_set_y(ui_Label168, 1);
    lv_obj_set_align(ui_Label168, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label168, "0");
    ui_object_set_themeable_style_property(ui_Label168, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR,
                                           _ui_theme_color_Black);
    ui_object_set_themeable_style_property(ui_Label168, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_OPA,
                                           _ui_theme_alpha_Black);
    lv_obj_set_style_text_font(ui_Label168, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label169 = lv_label_create(ui_Container68);
    lv_obj_set_width(ui_Label169, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label169, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label169, 69);
    lv_obj_set_y(ui_Label169, 2);
    lv_obj_set_align(ui_Label169, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label169, "A");
    lv_obj_set_style_text_color(ui_Label169, lv_color_hex(0xFA1B2A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Label169, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Label169, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label170 = lv_label_create(ui_Container68);
    lv_obj_set_width(ui_Label170, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label170, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label170, -37);
    lv_obj_set_y(ui_Label170, 7);
    lv_obj_set_align(ui_Label170, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label170, "Votol");
    lv_obj_set_style_text_color(ui_Label170, lv_color_hex(0xFA1B2A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Label170, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_decor(ui_Label170, LV_TEXT_DECOR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Label170, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Container66 = lv_obj_create(ui_Panel40);
    lv_obj_remove_style_all(ui_Container66);
    lv_obj_set_width(ui_Container66, lv_pct(41));
    lv_obj_set_height(ui_Container66, lv_pct(264));
    lv_obj_set_x(ui_Container66, 20);
    lv_obj_set_y(ui_Container66, 2);
    lv_obj_set_align(ui_Container66, LV_ALIGN_CENTER);
    lv_obj_set_flex_flow(ui_Container66, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(ui_Container66, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_clear_flag(ui_Container66, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_Container64 = lv_obj_create(ui_Container66);
    lv_obj_remove_style_all(ui_Container64);
    lv_obj_set_width(ui_Container64, 184);
    lv_obj_set_height(ui_Container64, lv_pct(50));
    lv_obj_set_x(ui_Container64, 218);
    lv_obj_set_y(ui_Container64, -100);
    lv_obj_set_align(ui_Container64, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_Container64, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_pad_left(ui_Container64, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_Container64, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_Container64, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_Container64, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_row(ui_Container64, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(ui_Container64, 8, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label155 = lv_label_create(ui_Container64);
    lv_obj_set_width(ui_Label155, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label155, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label155, -78);
    lv_obj_set_y(ui_Label155, 3);
    lv_obj_set_align(ui_Label155, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label155, "V");
    lv_obj_set_style_text_color(ui_Label155, lv_color_hex(0x31E567), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Label155, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Label155, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label156 = lv_label_create(ui_Container64);
    lv_obj_set_width(ui_Label156, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label156, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label156, 5);
    lv_obj_set_y(ui_Label156, 3);
    lv_obj_set_align(ui_Label156, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label156, "0");
    ui_object_set_themeable_style_property(ui_Label156, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR,
                                           _ui_theme_color_Black);
    ui_object_set_themeable_style_property(ui_Label156, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_OPA,
                                           _ui_theme_alpha_Black);
    lv_obj_set_style_text_font(ui_Label156, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label157 = lv_label_create(ui_Container64);
    lv_obj_set_width(ui_Label157, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label157, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label157, 55);
    lv_obj_set_y(ui_Label157, 3);
    lv_obj_set_align(ui_Label157, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label157, "V");
    lv_obj_set_style_text_color(ui_Label157, lv_color_hex(0x31E567), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Label157, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Label157, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label158 = lv_label_create(ui_Container64);
    lv_obj_set_width(ui_Label158, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label158, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label158, -53);
    lv_obj_set_y(ui_Label158, 8);
    lv_obj_set_align(ui_Label158, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label158, "Gen");
    lv_obj_set_style_text_color(ui_Label158, lv_color_hex(0x31E567), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Label158, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Label158, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Container67 = lv_obj_create(ui_Container66);
    lv_obj_remove_style_all(ui_Container67);
    lv_obj_set_width(ui_Container67, lv_pct(100));
    lv_obj_set_height(ui_Container67, lv_pct(50));
    lv_obj_set_x(ui_Container67, 1);
    lv_obj_set_y(ui_Container67, 38);
    lv_obj_set_align(ui_Container67, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_Container67, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_pad_left(ui_Container67, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_Container67, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_Container67, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_Container67, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_row(ui_Container67, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(ui_Container67, 8, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label163 = lv_label_create(ui_Container67);
    lv_obj_set_width(ui_Label163, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label163, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label163, -78);
    lv_obj_set_y(ui_Label163, 0);
    lv_obj_set_align(ui_Label163, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label163, "I");
    lv_obj_set_style_text_color(ui_Label163, lv_color_hex(0xF50ADA), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Label163, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Label163, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label164 = lv_label_create(ui_Container67);
    lv_obj_set_width(ui_Label164, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label164, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label164, 5);
    lv_obj_set_y(ui_Label164, -1);
    lv_obj_set_align(ui_Label164, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label164, "0");
    ui_object_set_themeable_style_property(ui_Label164, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR,
                                           _ui_theme_color_Black);
    ui_object_set_themeable_style_property(ui_Label164, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_OPA,
                                           _ui_theme_alpha_Black);
    lv_obj_set_style_text_font(ui_Label164, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label165 = lv_label_create(ui_Container67);
    lv_obj_set_width(ui_Label165, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label165, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label165, 54);
    lv_obj_set_y(ui_Label165, 0);
    lv_obj_set_align(ui_Label165, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label165, "A");
    lv_obj_set_style_text_color(ui_Label165, lv_color_hex(0xF50ADA), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Label165, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Label165, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label166 = lv_label_create(ui_Container67);
    lv_obj_set_width(ui_Label166, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label166, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label166, -53);
    lv_obj_set_y(ui_Label166, 7);
    lv_obj_set_align(ui_Label166, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label166, "Gen");
    lv_obj_set_style_text_color(ui_Label166, lv_color_hex(0xF50ADA), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Label166, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Label166, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Chart9 = lv_chart_create(ui_Container62);
    lv_obj_set_width(ui_Chart9, lv_pct(88));
    lv_obj_set_height(ui_Chart9, lv_pct(71));
    lv_obj_set_x(ui_Chart9, -1);
    lv_obj_set_y(ui_Chart9, -25);
    lv_obj_set_align(ui_Chart9, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_Chart9, LV_OBJ_FLAG_CLICKABLE);      /// Flags
    lv_chart_set_type(ui_Chart9, LV_CHART_TYPE_LINE);
    lv_chart_set_point_count(ui_Chart9, 25);
    lv_chart_set_range(ui_Chart9, LV_CHART_AXIS_PRIMARY_Y, 0, 60);
    lv_chart_set_range(ui_Chart9, LV_CHART_AXIS_SECONDARY_Y, 0, 25);
    lv_chart_set_div_line_count(ui_Chart9, 14, 25);
    lv_chart_set_axis_tick(ui_Chart9, LV_CHART_AXIS_PRIMARY_X, 10, 0, 25, 2, false, 50);
    lv_chart_set_axis_tick(ui_Chart9, LV_CHART_AXIS_PRIMARY_Y, 10, 5, 14, 2, true, 50);
    lv_chart_set_axis_tick(ui_Chart9, LV_CHART_AXIS_SECONDARY_Y, 10, 5, 6, 2, true, 25);
    ui_Chart9_series_1 = lv_chart_add_series(ui_Chart9, lv_color_hex(0xDBD00C),
                                                                 LV_CHART_AXIS_PRIMARY_Y);
    static lv_coord_t ui_Chart9_series_1_array[25] = { 0 };
    lv_chart_set_ext_y_array(ui_Chart9, ui_Chart9_series_1, ui_Chart9_series_1_array);
    ui_Chart9_series_2 = lv_chart_add_series(ui_Chart9, lv_color_hex(0x31E567),
                                                                 LV_CHART_AXIS_PRIMARY_Y);
    static lv_coord_t ui_Chart9_series_2_array[25] = { 0 };
    lv_chart_set_ext_y_array(ui_Chart9, ui_Chart9_series_2, ui_Chart9_series_2_array);
    ui_Chart9_series_3 = lv_chart_add_series(ui_Chart9, lv_color_hex(0xFA1B2A),
                                                                 LV_CHART_AXIS_SECONDARY_Y);
    static lv_coord_t ui_Chart9_series_3_array[25] = { 0 };
    lv_chart_set_ext_y_array(ui_Chart9, ui_Chart9_series_3, ui_Chart9_series_3_array);
    ui_Chart9_series_4 = lv_chart_add_series(ui_Chart9, lv_color_hex(0xF50ADA),
                                                                 LV_CHART_AXIS_SECONDARY_Y);
    static lv_coord_t ui_Chart9_series_4_array[25] = { 0 };
    lv_chart_set_ext_y_array(ui_Chart9, ui_Chart9_series_4, ui_Chart9_series_4_array);

    ui_object_set_themeable_style_property(ui_Chart9, LV_PART_TICKS | LV_STATE_DEFAULT, LV_STYLE_LINE_COLOR,
                                           _ui_theme_color_Black);
    ui_object_set_themeable_style_property(ui_Chart9, LV_PART_TICKS | LV_STATE_DEFAULT, LV_STYLE_LINE_OPA,
                                           _ui_theme_alpha_Black);
    ui_object_set_themeable_style_property(ui_Chart9, LV_PART_TICKS | LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR,
                                           _ui_theme_color_Black);
    ui_object_set_themeable_style_property(ui_Chart9, LV_PART_TICKS | LV_STATE_DEFAULT, LV_STYLE_TEXT_OPA,
                                           _ui_theme_alpha_Black);

    ui_Label171 = lv_label_create(ui_Container62);
    lv_obj_set_width(ui_Label171, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label171, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label171, -364);
    lv_obj_set_y(ui_Label171, -186);
    lv_obj_set_align(ui_Label171, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label171, "V");
    ui_object_set_themeable_style_property(ui_Label171, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR,
                                           _ui_theme_color_Black);
    ui_object_set_themeable_style_property(ui_Label171, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_OPA,
                                           _ui_theme_alpha_Black);
    lv_obj_set_style_text_font(ui_Label171, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label172 = lv_label_create(ui_Container62);
    lv_obj_set_width(ui_Label172, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label172, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label172, 359);
    lv_obj_set_y(ui_Label172, -187);
    lv_obj_set_align(ui_Label172, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label172, "A");
    lv_obj_set_style_text_font(ui_Label172, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Panel41 = lv_obj_create(ui_Container62);
    lv_obj_set_width(ui_Panel41, 211);
    lv_obj_set_height(ui_Panel41, 29);
    lv_obj_set_x(ui_Panel41, -1);
    lv_obj_set_y(ui_Panel41, -193);
    lv_obj_set_align(ui_Panel41, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_Panel41, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    ui_object_set_themeable_style_property(ui_Panel41, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_COLOR,
                                           _ui_theme_color_Label);
    ui_object_set_themeable_style_property(ui_Panel41, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_OPA,
                                           _ui_theme_alpha_Label);

    ui_Label173 = lv_label_create(ui_Panel41);
    lv_obj_set_width(ui_Label173, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label173, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_Label173, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label173, "Voltage & Current");
    ui_object_set_themeable_style_property(ui_Label173, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR,
                                           _ui_theme_color_Black);
    ui_object_set_themeable_style_property(ui_Label173, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_OPA,
                                           _ui_theme_alpha_Black);
    lv_obj_set_style_text_font(ui_Label173, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Button4 = lv_btn_create(ui_StandAlone2);
    lv_obj_set_width(ui_Button4, 75);
    lv_obj_set_height(ui_Button4, 40);
    lv_obj_set_x(ui_Button4, -350);
    lv_obj_set_y(ui_Button4, 210);
    lv_obj_set_align(ui_Button4, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Button4, LV_OBJ_FLAG_IGNORE_LAYOUT | LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_Button4, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_img_src(ui_Button4, &ui_img_1996974272, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Button5 = lv_btn_create(ui_StandAlone2);
    lv_obj_set_width(ui_Button5, 75);
    lv_obj_set_height(ui_Button5, 40);
    lv_obj_set_x(ui_Button5, 350);
    lv_obj_set_y(ui_Button5, 210);
    lv_obj_set_align(ui_Button5, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Button5, LV_OBJ_FLAG_IGNORE_LAYOUT | LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_Button5, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_img_src(ui_Button5, &ui_img_2135604835, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_add_event_cb(ui_Button4, ui_event_Button4, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_Button5, ui_event_Button5, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_StandAlone2, ui_event_StandAlone2, LV_EVENT_ALL, NULL);

}
