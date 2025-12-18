/* Copyright 2024 @ Keychron (https://www.keychron.com)
 *
 * 56式ポケベル入力 - Keychron Q0 Max (encoder) キーマップ
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 */

#include QMK_KEYBOARD_H
#include "keychron_common.h"
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
    [NT_BASE] = LAYOUT_tenkey_27(
        KC_MUTE, KC_ESC,  KC_DEL,  KC_TAB,  KC_BSPC,
        NK_M1,   KC_NUM,  KC_PSLS, KC_PAST, KC_PMNS,
        NK_M2,   KC_P7,   KC_P8,   KC_P9,   KC_PPLS,
        NK_M3,   KC_P4,   KC_P5,   KC_P6,
        NK_M4,   KC_P1,   KC_P2,   KC_P3,   KC_PENT,
        NK_M5,   KC_P0,            KC_PDOT          ),

    [NT_FN] = LAYOUT_tenkey_27(
        RGB_TOG, BT_HST1, BT_HST2, BT_HST3, P2P4G,
        NK_M1,   RGB_MOD, RGB_VAI, RGB_HUI, QK_BOOT,
        NK_M2,   RGB_RMOD,RGB_VAD, RGB_HUD, _______,
        NK_M3,   RGB_SAI, RGB_SPI, KC_MPRV,
        NK_M4,   RGB_SAD, RGB_SPD, KC_MPLY, _______,
        _______, RGB_TOG,          KC_MNXT          ),

    [NT_NIKOTOUCH] = LAYOUT_tenkey_27(
        KC_MUTE,    KC_COMM, KC_DOT,  KC_LBRC, KC_RBRC,
        NK_M1,      NK_1,    NK_2,    NK_3,    NK_BS,
        NK_M2,      NK_4,    NK_5,    NK_6,    KC_ENT,
        NK_M3,      NK_7,    NK_8,    NK_9,
        NK_M4,      NK_STAR, NK_0,    KC_HASH, KC_ENT,
        NK_M5,      KC_SPC,       MO(NT_NIKOSHIFT)  ),

    [NT_NIKOSHIFT] = LAYOUT_tenkey_27(
        _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______,
        _______, KC_LEFT, KC_DOWN, KC_UP,   KC_RIGHT,
        _______, KC_LEFT, KC_DOWN, KC_UP,
        _______, _______, _______, _______, _______,
        _______, _______,          _______          )
};
// clang-format on

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
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
    // Keychron共通処理
    if (!process_record_keychron_common(keycode, record)) {
        return false;
    }

    // ニコタッチ共通処理
    return process_record_nikotouch(keycode, record);
}
