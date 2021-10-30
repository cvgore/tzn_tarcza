#include "tarcza.h"

#define COLOR_AMBER_RGBA 255, 191, 0, 255
#define HAND_SECOND_W 2
#define HAND_SECOND_H 140
struct tarcza_data_t {
    // Logical objects
    Evas_Object* wnd_main; // window
    Evas_Object* conf_cont; // container

    // Face:
    // - Left side
    Evas_Object* l_lbl_clock; // label clock (l[eft]_l[a]b[e]l_clock)
    Evas_Object* l_lbl_date; // label date
    Evas_Object* l_lbl_temp; // label temperature (degrees)
    Evas_Object* l_lbl_batt; // label battery percent
    // - Clock hands
    Evas_Object* h_circ_pin; // circle pin (h[ands]_circ[le]_pin)
    Evas_Object* h_rect_hour; // hour hand rectangle
    Evas_Object* h_rect_min; // minute hand rectangle
    Evas_Object* h_rect_sec; // second hand rectangle

    //
    Evas_Coord screen_width;
    Evas_Coord screen_height;
};

static void update_watch(struct tarcza_data_t *ad, watch_time_h watch_time, int ambient) {
    const int WATCH_TEXT_BUF_LEN = 6;
    char watch_text[WATCH_TEXT_BUF_LEN];
    int hour24, minute;

    if (watch_time == NULL)
        return;

    watch_time_get_hour24(watch_time, &hour24);
    watch_time_get_minute(watch_time, &minute);

    snprintf(watch_text, WATCH_TEXT_BUF_LEN, "%02d:%02d", hour24, minute);
}

static void create_base_gui(struct tarcza_data_t *ad, int width, int height) {
    int ret;
    watch_time_h watch_time = NULL;

    ret = watch_app_get_elm_win(&ad->wnd_main);
    if (ret != APP_ERROR_NONE) {
        DBG_ERR("failed to get window. err = %d", ret);
        return;
    }

    evas_object_resize(ad->wnd_main, width, height);

    ad->conf_cont = elm_conformant_add(ad->wnd_main);
    evas_object_size_hint_weight_set(ad->conf_cont, EVAS_HINT_EXPAND,
    EVAS_HINT_EXPAND);
    elm_win_resize_object_add(ad->wnd_main, ad->conf_cont);
    evas_object_show(ad->conf_cont);

    ad->l_lbl_clock = elm_label_add(ad->conf_cont);
    evas_object_resize(ad->l_lbl_clock, width, height / 3);
    evas_object_move(ad->l_lbl_clock, 0, height / 3);
    evas_object_show(ad->l_lbl_clock);

    ret = watch_time_get_current_time(&watch_time);
    if (ret != APP_ERROR_NONE)
        DBG_ERR("failed to get current time. err = %d", ret);

    update_watch(ad, watch_time, 0);
    watch_time_delete(watch_time);

    Evas* evas_canv = evas_object_evas_get(ad->wnd_main);
    ad->h_rect_hour = evas_object_rectangle_add(evas_canv);

    evas_object_color_set(ad->h_rect_hour, COLOR_AMBER_RGBA);
    evas_object_geometry_set(ad->h_rect_hour, width / 2, height / 2, HAND_SECOND_W, HAND_SECOND_H);
    evas_object_show(ad->h_rect_hour);

    evas_object_show(ad->wnd_main);
}

static bool app_create(int width, int height, void *data) {
    struct tarcza_data_t *ad = data;

    ad->screen_height = height;
    ad->screen_width = width;

    create_base_gui(ad, width, height);

    return true;
}

static void app_control(app_control_h app_control, void *data) {
    /* Handle the launch request. */
}

static void app_pause(void *data) {
    /* Take necessary actions when application becomes invisible. */
}

static void app_resume(void *data) {
    /* Take necessary actions when application becomes visible. */
}

static void app_terminate(void *data) {
    /* Release all resources. */
}

static void app_time_tick(watch_time_h watch_time, void *data) {
    /* Called at each second while your app is visible. Update watch UI. */
    struct tarcza_data_t *ad = data;
    update_watch(ad, watch_time, 0);
}

static void app_ambient_tick(watch_time_h watch_time, void *data) {
    /* Called at each minute while the device is in ambient mode. Update watch UI. */
    struct tarcza_data_t *ad = data;
    update_watch(ad, watch_time, 1);
}

static void app_ambient_changed(bool ambient_mode, void *data) {
    /* Update your watch UI to conf_container to the ambient mode */
}

static void watch_app_region_changed(app_event_info_h event_info, void *user_data) {
    /*APP_EVENT_REGION_FORMAT_CHANGED*/
}

int main(int argc, char *argv[]) {
    struct tarcza_data_t ad = { NULL, };
    int ret = 0;

    watch_app_lifecycle_callback_s event_callback = { 0, };
    app_event_handler_h handlers[5] = { NULL, };

    event_callback.create = app_create;
    event_callback.terminate = app_terminate;
    event_callback.pause = app_pause;
    event_callback.resume = app_resume;
    event_callback.app_control = app_control;
    event_callback.time_tick = app_time_tick;
    event_callback.ambient_tick = app_ambient_tick;
    event_callback.ambient_changed = app_ambient_changed;

    watch_app_add_event_handler(&handlers[APP_EVENT_REGION_FORMAT_CHANGED], APP_EVENT_REGION_FORMAT_CHANGED,
            watch_app_region_changed, &ad);

    ret = watch_app_main(argc, argv, &event_callback, &ad);
    if (ret != APP_ERROR_NONE) {
        DBG_ERR("watch_app_main() is failed. err = %d", ret);
    }

    return ret;
}

