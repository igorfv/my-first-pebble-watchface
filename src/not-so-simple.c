#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"

#include <string.h>

#define MY_UUID {0xA4, 0x1B, 0xB0, 0xE2, 0xD2, 0x62, 0x4E, 0xA6, 0xAA, 0x30, 0xED, 0xBE, 0x01, 0xE3, 0x8A, 0x02}
PBL_APP_INFO(MY_UUID, "Not so simple", "Igor Frias Vieira", 3, 0 /* App version */, RESOURCE_ID_IMAGE_MENU_ICON, APP_INFO_WATCH_FACE);

Window window;

TextLayer text_date_layer;
TextLayer text_year_layer;
TextLayer text_time_layer;

Layer line_layer;


void line_layer_update_callback(Layer *me, GContext* ctx) {
  (void)me;

  graphics_context_set_stroke_color(ctx, GColorWhite);

  graphics_draw_line(ctx, GPoint(8, 97), GPoint(131, 97));
  graphics_draw_line(ctx, GPoint(8, 98), GPoint(131, 98));

}


void handle_init(AppContextRef ctx) {
  (void)ctx;

  window_init(&window, "Not so simple");
  window_stack_push(&window, true /* Animated */);
  window_set_background_color(&window, GColorBlack);

  resource_init_current_app(&APP_RESOURCES);


  text_layer_init(&text_date_layer, window.layer.frame);
  text_layer_set_text_color(&text_date_layer, GColorWhite);
  text_layer_set_background_color(&text_date_layer, GColorClear);
  text_layer_set_text_alignment(&text_date_layer, GTextAlignmentCenter);
  layer_set_frame(&text_date_layer.layer, GRect(0, 15, 144, 168-15));
  text_layer_set_font(&text_date_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROBOTO_CONDENSED_21)));
  layer_add_child(&window.layer, &text_date_layer.layer);

  text_layer_init(&text_year_layer, window.layer.frame);
  text_layer_set_text_color(&text_year_layer, GColorWhite);
  text_layer_set_background_color(&text_year_layer, GColorClear);
  text_layer_set_text_alignment(&text_year_layer, GTextAlignmentCenter);
  layer_set_frame(&text_year_layer.layer, GRect(0, 30, 144, 168-30));
  text_layer_set_font(&text_year_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROBOTO_BOLD_SUBSET_49)));
  layer_add_child(&window.layer, &text_year_layer.layer);

  text_layer_init(&text_time_layer, window.layer.frame);
  text_layer_set_text_color(&text_time_layer, GColorWhite);
  text_layer_set_background_color(&text_time_layer, GColorClear);
  text_layer_set_text_alignment(&text_time_layer, GTextAlignmentCenter);
  layer_set_frame(&text_time_layer.layer, GRect(0, 100, 144, 168-100));
  text_layer_set_font(&text_time_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROBOTO_BOLD_SUBSET_49)));
  layer_add_child(&window.layer, &text_time_layer.layer);


  layer_init(&line_layer, window.layer.frame);
  line_layer.update_proc = &line_layer_update_callback;
  layer_add_child(&window.layer, &line_layer);


  // TODO: Update display here to avoid blank display on launch?
}


void handle_minute_tick(AppContextRef ctx, PebbleTickEvent *t) {

  (void)ctx;

  // Need to be static because they're used by the system later.
  static char time_text[] = "00:00";
  static char date_month[] = "Xx xxxxxxxxx";
  static char date_day[] = "00";
  static char date_year[] = "0000";
  static char full_date[] = "00 Xx xxxxxxxxx";

  char *time_format;


  // TODO: Only update the date when it's changed.
  string_format_time(date_month, sizeof(date_month), "%m", t->tick_time);
  string_format_time(date_day, sizeof(date_day), "%e", t->tick_time);
  string_format_time(date_year, sizeof(date_year), "%Y", t->tick_time);

  strcpy(full_date, date_day);

  if (strcmp(date_month, "01") == 0) 
  {
    strcat(full_date, " de Janeiro");
  } 
  else if (strcmp(date_month, "02") == 0)
  {
    strcat(full_date, " de Fevereiro");
  } 
  else if (strcmp(date_month, "03") == 0)
  {
    strcat(full_date, " de Março");
  } 
  else if (strcmp(date_month, "04") == 0)
  {
    strcat(full_date, " de Abril");
  } 
  else if (strcmp(date_month, "05") == 0)
  {
    strcat(full_date, " de Maio");
  } 
  else if (strcmp(date_month, "06") == 0)
  {
    strcat(full_date, " de Junho");
  } 
  else if (strcmp(date_month, "07") == 0)
  {
    strcat(full_date, " de Julho");
  } 
  else if (strcmp(date_month, "08") == 0)
  {
    strcat(full_date, " de Agosto");
  } 
  else if (strcmp(date_month, "09") == 0)
  {
    strcat(full_date, " de Setembro");
  } 
  else if (strcmp(date_month, "10") == 0)
  {
    strcat(full_date, " de Outubro");
  } 
  else if (strcmp(date_month, "11") == 0)
  {
    strcat(full_date, " de Novembro");
  } 
  else if (strcmp(date_month, "12") == 0)
  {
    strcat(full_date, " de Dezembro");
  }
  else /* default: */
  {
    strcat(full_date, "");
  }

  text_layer_set_text(&text_date_layer, full_date);

  text_layer_set_text(&text_year_layer, date_year);


  if (clock_is_24h_style()) {
    time_format = "%R";
  } else {
    time_format = "%I:%M";
  }

  string_format_time(time_text, sizeof(time_text), time_format, t->tick_time);

  // Kludge to handle lack of non-padded hour format string
  // for twelve hour clock.
  if (!clock_is_24h_style() && (time_text[0] == '0')) {
    memmove(time_text, &time_text[1], sizeof(time_text) - 1);
  }

  text_layer_set_text(&text_time_layer, time_text);

}


void pbl_main(void *params) {
  PebbleAppHandlers handlers = {
    .init_handler = &handle_init,

    .tick_info = {
      .tick_handler = &handle_minute_tick,
      .tick_units = MINUTE_UNIT
    }

  };
  app_event_loop(params, &handlers);
}
