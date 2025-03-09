// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

#define LAYER_CYCLE_START 0
#define LAYER_CYCLE_END 4

enum layer_names {
    _Macro_ = 0,
    _NumPad_ = 1,
    _Media_ = 2,
    _Gaming_ = 3
};

#ifdef OLED_ENABLE 

typedef enum {
    LAYER,
    CAPS,
    NUM,
} latest_event_t;

// we will track things here
latest_event_t event;

void render_layer(uint8_t layer) {
    switch (layer) {
        case _Macro_ :
            oled_write("Macro", false);
            break;
        case _NumPad_ :
            oled_write("Num/Calc", false);
            break;
        case _Gaming_ : 
            oled_write("Gaming", false);
            break;
        case _Media_ :
            oled_write("Media", false);
            break;
        default:
            oled_write("Undefined", false);
    }
}

led_t led_state_t = host_keyboard_led_state();
bool oled_task_user(void) {
    oled_set_cursor(0, 1);

    switch (latest_event_t) {
        case LAYER:
            render_layer(get_highest_layer(default_layer_state));
            break;

        case CAPS:
            oled_write_P(led_task.caps_lock ? PSTR("Caps Lock On") : PSTR("Caps Lock Off"), false);
            break;

        case NUM:
            oled_write_P(led_task.num_lock ? PSTR("NUM ") : PSTR("NAV "), false);;
            break;

        default: // unreachable?
            break;
    }
    return false;
}

layer_state_t layer_state_set_user(layer_state_t state) {
    latest_event_t.event = LAYER;
    return state;
}

layer_state_t default_layer_state_set_user(layer_state_t state) {
    latest_event_t.event = LAYER;
    return state;
}

bool led_update_user(led_t led_state) {
    if (current_led_state.caps_lock != current_led_state.caps_lock) latest_event_t.event = CAPS;
    if (current_led_state.num_lock != current_led_state.num_lock) latest_event_t.event = NUM;
    current_led_state = led_state;
    return false;
}

#endif

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*
     * ┌───┬───┬───┬───┐
     * │ X │ X │LYR│ACT│
     * ├───┼───┼───┼───┤
     * │NUM│ / │ * │ - │
     * ├───┼───┼───┼───┤
     * │ 7 │ 8 │ 8 │ + │
     * ├───┼───┼───┼───┤
     * │ 4 │ 5 │ 6 │ + │
     * ├───┼───┼───┼───┤
     * │ 1 │ 2 │ 3 │ENT│
     * ├───┼───┼───┼───┤
     * │B S│ 0 │ . │ENT│
     * └───┴───┴───┴───┘
     */
    
    // still work in progress with the layers. 

    // Macro Layer 
    [_Macro_] = LAYOUT(
        KC_NO, KC_NO, TO(_NumPad_), KC_NO, 
        KC_NUM, KC_PSLS, KC_PAST, KC_PMNS,
        KC_P7, KC_P8, KC_P9, KC_PPLS,
        KC_P4, KC_P5, KC_P6, KC_NO,
        KC_P1, KC_P2, KC_P3, KC_PENT,
        KC_NO, KC_P0, KC_PDOT, KC_NO
    ),

    // Numpad/Calculator Layer, 
    [_NumPad_] = LAYOUT(
        KC_NO, KC_NO, TO(_Media_), KC_CALC, 
        KC_NUM, KC_PSLS, KC_PAST, KC_PMNS,
        KC_P7, KC_P8, KC_P9, KC_PPLS,
        KC_P4, KC_P5, KC_P6, KC_NO,
        KC_P1, KC_P2, KC_P3, KC_PENT,
        KC_BSPC, KC_P0, KC_PDOT, KC_NO
    ),

    [_Media_] = LAYOUT(
        KC_NO, KC_NO, TO(_Gaming_), KC_CALC,
        KC_NUM, KC_PSLS, KC_PAST, KC_PMNS,
        KC_P7, KC_P8, KC_P9, KC_PPLS,
        KC_P4, KC_P5, KC_P6, KC_NO,
        KC_P1, KC_P2, KC_P3, KC_PENT,
        KC_BSPC, KC_P0, KC_PDOT, KC_NO
    ),

    // rotated 90 degrees clockwise
    [_Gaming_] = LAYOUT(
        KC_NO, KC_NO, TO(_Macro_), TT(), 
        KC_4, KC_R, KC_F, KC_SPC,
        KC_3, KC_E, KC_D, KC_C,
        KC_2, KC_W, KC_S, KC_X,
        KC_1, KC_Q, KC_A, KC_Z,
        KC_ESC, KC_TAB, KC_LSFT, KC_LCTL
    )
};