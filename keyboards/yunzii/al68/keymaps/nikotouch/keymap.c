/* Copyright 2024 nemohashi
 *
 * 56式ポケベル入力 - Yunzii AL68 キーマップ
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 */

#include QMK_KEYBOARD_H
#include "nikotouch.h"

// ============================================================
// 56式コンボ定義
// ============================================================
enum combos {
    COMBO_56_STAR,  // 5+6 = 濁点（*キー相当）
    COMBO_45_ZERO,  // 4+5 = 0キー相当
    COMBO_COUNT
};

// コンボキー定義（NIKOTOUCHレイヤーのキーを使用）
const uint16_t PROGMEM combo_56[] = {NK_5, NK_6, COMBO_END};
const uint16_t PROGMEM combo_45[] = {NK_4, NK_5, COMBO_END};

combo_t key_combos[COMBO_COUNT] = {
    [COMBO_56_STAR] = COMBO(combo_56, CMB_56),
    [COMBO_45_ZERO] = COMBO(combo_45, CMB_45),
};

// コンボのタイミング設定
uint16_t get_combo_term(uint16_t index, combo_t *combo) {
    return COMBO_TERM_56;
}

// コンボをNIKOTOUCHレイヤーでのみ有効にする
bool combo_should_trigger(uint16_t combo_index, combo_t *combo, uint16_t keycode, keyrecord_t *record) {
    return layer_state_is(NT_NIKOTOUCH);
}

// ============================================================
// キーマップ定義
// ============================================================
// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [NT_BASE] = LAYOUT_65_ansi_blocker(
        KC_ESC,     KC_1,       KC_2,       KC_3,       KC_4,       KC_5,       KC_6,       KC_7,       KC_8,       KC_9,       KC_0,       KC_MINS,    KC_EQL,     KC_BSPC,    KC_MUTE,
        KC_TAB,     KC_Q,       KC_W,       KC_E,       KC_R,       KC_T,       KC_Y,       KC_U,       KC_I,       KC_O,       KC_P,       KC_LBRC,    KC_RBRC,    KC_BSLS,    KC_DELETE,
        KC_CAPS,    KC_A,       KC_S,       KC_D,       KC_F,       KC_G,       KC_H,       KC_J,       KC_K,       KC_L,       KC_SCLN,    KC_QUOT,                KC_ENT,     KC_PAGE_UP,
        KC_LSFT,    KC_Z,       KC_X,       KC_C,       KC_V,       KC_B,       KC_N,       KC_M,       KC_COMM,    KC_DOT,     KC_SLSH,    KC_RSFT,                KC_UP,      KC_PAGE_DOWN,
        KC_LCTL,    KC_LGUI,    KC_LALT,                            KC_SPC,                             NK_M5,      KC_RCTL,                            KC_LEFT,    KC_DOWN,    KC_RIGHT    
    ),

    [NT_FN] = LAYOUT_65_ansi_blocker(
        KC_GRV,     KC_F1,      KC_F2,      KC_F3,      KC_F4,      KC_F5,      KC_F6,      KC_F7,      KC_F8,      KC_F9,      KC_F10,     KC_F11,     KC_F12,     RGB_TOG,    _______,
        _______,    KC_USB,     KC_BLE1,    KC_BLE2,    KC_BLE3,    KC_24G,     _______,    _______,    _______,    _______,    _______,    _______,    _______,    RGB_MOD,    NK_M2,
        _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,                RGB_HUI,    NK_M1, 
        _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,                RGB_VAI,    _______,
        _______,    _______,     _______,                           _______,                           _______,    _______,                            RGB_SPD,    RGB_VAD,    RGB_SPI 
    ),

    [NT_NIKOTOUCH] = LAYOUT_65_ansi_blocker(
        NK_M1,      XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    NK_1,       NK_2,       NK_3,       NK_BS,      NK_BS,      XXXXXXX,    _______,    _______,
        XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    NK_4,       NK_5,       NK_6,       KC_ENT,     XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,
        XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    NK_7,       NK_8,       NK_9,       KC_ENT,     XXXXXXX,                _______,    _______,
        _______,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    NK_STAR,    NK_0,       XXXXXXX,    KC_ENT,     XXXXXXX,    _______,                KC_UP,      _______,
        XXXXXXX,    XXXXXXX,    XXXXXXX,                            KC_SPC,                             NK_M5,      XXXXXXX,                            KC_LEFT,    KC_DOWN,    KC_RIGHT
    ),

    [NT_NIKOSHIFT] = LAYOUT_65_ansi_blocker(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, KC_LEFT, KC_DOWN, KC_UP,   KC_RIGHT,_______, _______, _______, _______, _______, _______, _______,          _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          KC_UP,   _______,
        _______, _______, _______,                   _______,                            _______, _______,          KC_LEFT, KC_DOWN, KC_RIGHT
    )
};
// clang-format on

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [NT_BASE]      = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [NT_FN]        = {ENCODER_CCW_CW(RGB_VAD, RGB_VAI)},
    [NT_NIKOTOUCH] = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [NT_NIKOSHIFT] = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
};
#endif // ENCODER_MAP_ENABLE

// ============================================================
// タイムアウト処理
// ============================================================
void matrix_scan_user(void) {
    matrix_scan_nikotouch();
}

// ============================================================
// キー処理
// ============================================================
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // ニコタッチ共通処理
    return process_record_nikotouch(keycode, record);
}
