#include "pebble_process_info.h"
#include "pebble.h"

extern const PebbleProcessInfo __pbl_app_info;

GColor BG, FG;
 
static Window *s_main_window;
static TextLayer *s_date_layer, *s_time_layer;
static Layer *s_line_layer;

static void line_layer_update_callback(Layer *layer, GContext* ctx) {
  graphics_context_set_fill_color(ctx, FG);
  graphics_fill_rect(ctx, layer_get_bounds(layer), 0, GCornerNone);
}

static void handle_minute_tick(struct tm *tick_time, TimeUnits units_changed) {
  // Need to be static because they're used by the system later.
  static char s_time_text[] = "00:00";
  static char s_date_text[] = "Xxxxxxxxx 00";

  char *p;

  strftime(s_date_text, sizeof(s_date_text), "%d %b", tick_time);
  p = s_date_text;
  while (*p == '0') {
  	p++;
  }
  text_layer_set_text(s_date_layer, p);

  char *time_format;
  if (clock_is_24h_style()) {
    time_format = "%R";
  } else {
    time_format = "%l:%M";
  }
  strftime(s_time_text, sizeof(s_time_text), time_format, tick_time);

  // I always remove leading zero, because I arbitrarily think that looks better.
  p = s_time_text;
  while (*p == '0') {
  	p++;
  }
  text_layer_set_text(s_time_layer, p);
}

static void main_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);

  s_date_layer = text_layer_create(GRect(8, 66, 136, 100));
  text_layer_set_text_color(s_date_layer, FG);
  text_layer_set_background_color(s_date_layer, GColorClear);
  text_layer_set_font(s_date_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_R_28)));
  layer_add_child(window_layer, text_layer_get_layer(s_date_layer));

  s_time_layer = text_layer_create(GRect(7, 92, 137, 76));
  text_layer_set_text_color(s_time_layer, FG);
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_font(s_time_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_B_48)));
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
}

static void main_window_unload(Window *window) {
  text_layer_destroy(s_date_layer);
  text_layer_destroy(s_time_layer);

  layer_destroy(s_line_layer);
}

static void init() {
  // First things first: are we black or white?
  if (__pbl_app_info.uuid.byte15 % 2) {
  	BG = GColorBlack;
	FG = GColorWhite;
  } else {
	BG = GColorWhite;
  	FG = GColorBlack;
  }

  s_main_window = window_create();
  window_set_background_color(s_main_window, BG);
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload,
  });
  window_stack_push(s_main_window, true);

  tick_timer_service_subscribe(MINUTE_UNIT, handle_minute_tick);
  
  // Prevent starting blank
  time_t now = time(NULL);
  struct tm *t = localtime(&now);
  handle_minute_tick(t, MINUTE_UNIT);
  
  setlocale(LC_ALL, "");
}

static void deinit() {
  window_destroy(s_main_window);

  tick_timer_service_unsubscribe();
}

int main() {
  init();
  app_event_loop();
  deinit();
}
