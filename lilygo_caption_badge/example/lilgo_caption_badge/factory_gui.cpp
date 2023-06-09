#include "factory_gui.h"
#include "Arduino.h"
#include "SD_MMC.h"
#include "lvgl.h"
#include "mqtt.h"

// global variables to manage the list and its items:
static lv_obj_t *mqtt_list;
static lv_obj_t **mqtt_list_items;
static const uint8_t max_lines = 10;  // Adjust this according to the size of your display
static uint8_t cur_lines = 0;
static lv_obj_t *placeholder_label; // Placeholder to display before receiving MQTT messages


LV_FONT_DECLARE(font_Alibaba);
LV_IMG_DECLARE(lilygo1_gif);
static lv_point_t line_points[] = {{-320, 0}, {320, 0}};

static void update_text_subscriber_cb(lv_event_t *e);
static void update_touch_point_subscriber_cb(lv_event_t *e);
static void timer_task(lv_timer_t *t);
static lv_obj_t *dis;

void ui_switch_page(void) {
  static uint8_t n;
  n++;
  lv_obj_set_tile_id(dis, 0, n % UI_PAGE_COUNT, LV_ANIM_ON);
}

void ui_begin() {

  dis = lv_tileview_create(lv_scr_act());
  lv_obj_align(dis, LV_ALIGN_TOP_RIGHT, 0, 0);
  lv_obj_set_size(dis, LV_PCT(100), LV_PCT(100));
  lv_obj_remove_style(dis, 0, LV_PART_SCROLLBAR);

  lv_obj_t *tv1 = lv_tileview_add_tile(dis, 0, 0, LV_DIR_VER);
  lv_obj_t *tv2 = lv_tileview_add_tile(dis, 0, 1, LV_DIR_VER);
  lv_obj_t *tv3 = lv_tileview_add_tile(dis, 0, 2, LV_DIR_VER);
  lv_obj_t *tv4 = lv_tileview_add_tile(dis, 0, 3, LV_DIR_VER);

  /* page 1 MQTT Captions */
  // initialize the list and the array of list items:
  mqtt_list = lv_list_create(tv1);

  // Create a placeholder label to display before receiving MQTT messages
  placeholder_label = lv_label_create(mqtt_list);
  lv_label_set_text(placeholder_label, "Captions will appear here.");

  lv_obj_set_size(mqtt_list, LV_PCT(100), LV_PCT(100));
  mqtt_list_items = (lv_obj_t**) malloc(max_lines * sizeof(lv_obj_t*));


  // Create MQTT Label
  mqtt_label = lv_label_create(tv1); // This label will be used to display incoming MQTT messages
  lv_obj_center(mqtt_label);
  lv_obj_set_style_text_font(mqtt_label, &font_Alibaba, 0);
  lv_label_set_text(mqtt_label, ""); // Initially set it to empty string
  lv_obj_set_style_text_color(mqtt_label, UI_FONT_COLOR, 0);

  // Allocate memory for mqtt_list_items
  mqtt_list_items = (lv_obj_t**) malloc(max_lines * sizeof(lv_obj_t*));
  cur_lines = 0;


  /* page 2 */
  lv_obj_t *main_cout = lv_obj_create(tv2);
  lv_obj_set_size(main_cout, LV_PCT(100), LV_PCT(100));
  lv_obj_clear_flag(main_cout, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_style_border_width(main_cout, 0, 0);
  lv_obj_set_style_bg_color(main_cout, UI_BG_COLOR, 0);

  lv_obj_t *hour_cout = lv_obj_create(main_cout);
  lv_obj_set_size(hour_cout, 140, 140);
  lv_obj_align(hour_cout, LV_ALIGN_CENTER, -85, 0);
  lv_obj_set_style_bg_color(hour_cout, UI_FRAME_COLOR, 0);
  lv_obj_clear_flag(hour_cout, LV_OBJ_FLAG_SCROLLABLE);

  lv_obj_t *min_cout = lv_obj_create(main_cout);
  lv_obj_set_size(min_cout, 140, 140);
  lv_obj_align(min_cout, LV_ALIGN_CENTER, 85, 0);
  lv_obj_set_style_bg_color(min_cout, UI_FRAME_COLOR, 0);
  lv_obj_clear_flag(min_cout, LV_OBJ_FLAG_SCROLLABLE);

  lv_obj_t *seg_text = lv_label_create(main_cout);
  lv_obj_align(seg_text, LV_ALIGN_CENTER, 0, -10);
  lv_obj_set_style_text_font(seg_text, &font_Alibaba, 0);
  lv_label_set_text(seg_text, ":");
  lv_obj_set_style_text_color(seg_text, UI_FONT_COLOR, 0);

  lv_obj_t *hour_text = lv_label_create(hour_cout);
  lv_obj_center(hour_text);
  lv_obj_set_style_text_font(hour_text, &font_Alibaba, 0);
  lv_label_set_text(hour_text, "12");
  lv_obj_set_style_text_color(hour_text, UI_FONT_COLOR, 0);
  lv_obj_add_event_cb(hour_text, update_text_subscriber_cb, LV_EVENT_MSG_RECEIVED, NULL);
  lv_msg_subsribe_obj(MSG_NEW_HOUR, hour_text, (void *)"%02d");

  lv_obj_t *min_text = lv_label_create(min_cout);
  lv_obj_center(min_text);
  lv_obj_set_style_text_font(min_text, &font_Alibaba, 0);
  lv_label_set_text(min_text, "34");
  lv_obj_set_style_text_color(min_text, UI_FONT_COLOR, 0);
  lv_obj_add_event_cb(min_text, update_text_subscriber_cb, LV_EVENT_MSG_RECEIVED, NULL);
  lv_msg_subsribe_obj(MSG_NEW_MIN, min_text, (void *)"%02d");

  static lv_style_t style_line;
  lv_style_init(&style_line);
  lv_style_set_line_width(&style_line, 4);
  lv_style_set_line_color(&style_line, UI_BG_COLOR);
  lv_style_set_line_rounded(&style_line, true);

  lv_obj_t *line;
  line = lv_line_create(main_cout);
  lv_line_set_points(line, line_points, 2);
  lv_obj_add_style(line, &style_line, 0);
  lv_obj_center(line);

  /* page 3 */
  lv_obj_t *logo_img = lv_gif_create(tv3);
  lv_obj_center(logo_img);
  lv_gif_set_src(logo_img, &lilygo1_gif);

   /* page 4 */
  lv_obj_t *debug_label = lv_label_create(tv4);
  String text;
  esp_chip_info_t t;
  esp_chip_info(&t);
  text = "chip : ";
  text += ESP.getChipModel();
  text += "\n";
  text += "psram size : ";
  text += ESP.getPsramSize() / 1024;
  text += " KB\n";
  text += "flash size : ";
  text += ESP.getFlashChipSize() / 1024;
  text += " KB\n";

  extern bool inited_sd;
  if (inited_sd) {
    text += "SD card found\r\nSize : ";
    text += SD_MMC.cardSize() / 1024;
    text += " kb";
  } else {
    text += "SD card not found";
  }

  lv_label_set_text(debug_label, text.c_str());
  lv_obj_align(debug_label, LV_ALIGN_TOP_LEFT, 0, 0);

  lv_obj_t *bat_label = lv_label_create(tv3);
  lv_obj_align_to(bat_label, debug_label, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);
  lv_obj_add_event_cb(bat_label, update_text_subscriber_cb, LV_EVENT_MSG_RECEIVED, NULL);
  lv_msg_subsribe_obj(MSG_NEW_VOLT, bat_label, (void *)"VOLT : %d mV");

  lv_obj_t *touch_label = lv_label_create(tv3);
  lv_obj_align_to(touch_label, bat_label, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);
  lv_obj_add_event_cb(touch_label, update_touch_point_subscriber_cb, LV_EVENT_MSG_RECEIVED, NULL);
  lv_label_set_text_fmt(touch_label, "");
  lv_msg_subsribe_obj(MSG_NEW_TOUCH_POINT, touch_label, (void *)"%s");

  lv_timer_t *timer = lv_timer_create(timer_task, 500, seg_text);
}

static void timer_task(lv_timer_t *t) {
  lv_obj_t *seg = (lv_obj_t *)t->user_data;
  static bool j;
  if (j)
    lv_obj_add_flag(seg, LV_OBJ_FLAG_HIDDEN);
  else
    lv_obj_clear_flag(seg, LV_OBJ_FLAG_HIDDEN);
  j = !j;
}

static void update_text_subscriber_cb(lv_event_t *e) {
  lv_obj_t *label = lv_event_get_target(e);
  lv_msg_t *m = lv_event_get_msg(e);

  const char *fmt = (const char *)lv_msg_get_user_data(m);
  const int32_t *v = (const int32_t *)lv_msg_get_payload(m);

  lv_label_set_text_fmt(label, fmt, *v);
}

// Add a new MQTT message to the list.
// Removes the oldest message if the maximum number of messages has been reached:
void add_mqtt_message(const char* msg) {
  
  // Create a new label for the message
  lv_obj_t* new_label = lv_label_create(mqtt_list);
  lv_label_set_text(new_label, msg);

    // Replace the oldest message if we've reached max_lines
  if (cur_lines >= max_lines) {
    lv_obj_t *old_label = mqtt_list_items[cur_lines % max_lines];
    lv_obj_del(old_label);
  }

  mqtt_list_items[cur_lines % max_lines] = new_label;
  cur_lines++;

  lv_obj_scroll_to_view_recursive(new_label, LV_ANIM_ON);
}


static void update_touch_point_subscriber_cb(lv_event_t *e) {
  lv_obj_t *label = lv_event_get_target(e);
  lv_msg_t *m = lv_event_get_msg(e);

  const char *fmt = (const char *)lv_msg_get_user_data(m);
  const char *t = (const char *)lv_msg_get_payload(m);

  lv_label_set_text_fmt(label, fmt, t);
}