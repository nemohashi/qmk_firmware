/* Copyright 2024 @ Keychron (https://www.keychron.com)
 *
 * 56式ポケベル入力 + Artsey入力 - Keychron Q0 Max (encoder) キーマップ
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 */

#include QMK_KEYBOARD_H
#include "keychron_common.h"
#include "nikotouch.h"
#include "artsey.h"

// ============================================================
// コンボ定義
// ============================================================
enum combos {
    // 56式コンボ（NIKOTOUCHレイヤー）
    COMBO_56_STAR,  // 5+6 = 濁点（*キー相当）
    COMBO_67_ZERO,  // 6+7 = 0キー相当
    COMBO_79_REPLAY, // 7+9 = 確定前バッファから残りを再入力
    COMBO_1BS,      // 1+BS = 2文字削除
    COMBO_2BS,      // 2+BS = 3文字削除
    COMBO_3BS,      // 3+BS = 4文字削除
    
    // レイヤー切り替えコンボ（NIKOTOUCH用）
    COMBO_NK_LAYER_TOGGLE,  // Space+Ent = NIKOTOUCH ⇔ ARTSEY
    COMBO_NK_LAYER_BASE,    // Space+Ent2 = BASE ⇔ 前レイヤー
    
    // レイヤー切り替えコンボ（ARTSEY用）
    COMBO_AR_LAYER_TOGGLE,  // Space+Ent = NIKOTOUCH ⇔ ARTSEY
    COMBO_AR_LAYER_BASE,    // Space+Ent2 = BASE ⇔ 前レイヤー
    
    // レイヤー切り替えコンボ（BASE用）
    COMBO_BASE_LAYER_BACK,  // P0+PENT = BASE → 前レイヤー
    
    // Artseyコンボ（ARTSEY_ALPHAレイヤー）2キー
    COMBO_AR_12,    // 1+2 = f
    COMBO_AR_23,    // 2+3 = g
    COMBO_AR_3M,    // 3+- = j
    COMBO_AR_45,    // 4+5 = c
    COMBO_AR_56,    // 5+6 = u
    COMBO_AR_6E,    // 6+Ent = n
    COMBO_AR_14,    // 1+4 = Enter
    COMBO_AR_25,    // 2+5 = '
    COMBO_AR_36,    // 3+6 = !
    COMBO_AR_ME,    // -+Ent = ?
    COMBO_AR_1M,    // 1+- = w
    COMBO_AR_4E,    // 4+Ent = b
    COMBO_AR_2M,    // 2+- = v
    COMBO_AR_46,    // 4+6 = h
    COMBO_AR_5E,    // 5+Ent = k
    COMBO_AR_15,    // 1+5 = ,
    COMBO_AR_26,    // 2+6 = Delete
    COMBO_AR_16,    // 1+6 = .
    COMBO_AR_1E,    // 1+Ent = /
    COMBO_AR_24,    // 2+4 = Backspace（新規）
    
    // Artseyコンボ 3キー
    COMBO_AR_123,   // 1+2+3 = d
    COMBO_AR_56E,   // 5+6+Ent = m
    COMBO_AR_13M,   // 1+3+- = q
    COMBO_AR_46E,   // 4+6+Ent = p
    COMBO_AR_23M,   // 2+3+- = x
    COMBO_AR_456,   // 4+5+6 = l
    COMBO_AR_12E,   // 1+2+Ent = Esc（新規）
    
    // Artseyコンボ 4キー
    COMBO_AR_456E,  // 4+5+6+Ent = Space
    COMBO_AR_123M,  // 1+2+3+- = z
    COMBO_AR_123E,  // 1+2+3+Ent = Tab（新規）
    COMBO_AR_423M,  // 4+2+3+- = Once Shift（新規）
    COMBO_AR_156E,  // 1+5+6+Ent = Toggle Shift（新規）
    
    COMBO_COUNT
};

// コンボキー定義（NIKOTOUCHレイヤーのキーを使用）
const uint16_t PROGMEM combo_56[] = {NK_5, NK_6, COMBO_END};
const uint16_t PROGMEM combo_67[] = {NK_6, NK_7, COMBO_END};
const uint16_t PROGMEM combo_79[] = {NK_7, NK_9, COMBO_END};
const uint16_t PROGMEM combo_1bs[] = {NK_1, NK_BS, COMBO_END};
const uint16_t PROGMEM combo_2bs[] = {NK_2, NK_BS, COMBO_END};
const uint16_t PROGMEM combo_3bs[] = {NK_3, NK_BS, COMBO_END};

// レイヤー切り替えコンボキー定義（NIKOTOUCH用: KC_SPC + NK_ENT/NK_ENT2）
const uint16_t PROGMEM combo_nk_layer_toggle[] = {KC_SPC, NK_ENT, COMBO_END};
const uint16_t PROGMEM combo_nk_layer_base[] = {KC_SPC, NK_ENT2, COMBO_END};

// レイヤー切り替えコンボキー定義（ARTSEY用: AR_SPC + AR_ENT/AR_ENT2）
const uint16_t PROGMEM combo_ar_layer_toggle[] = {AR_SPC, AR_ENT, COMBO_END};
const uint16_t PROGMEM combo_ar_layer_base[] = {AR_SPC, AR_ENT2, COMBO_END};

// レイヤー切り替えコンボキー定義（BASE用: KC_P0 + KC_PENT）
const uint16_t PROGMEM combo_base_layer_back[] = {KC_P0, KC_PENT, COMBO_END};

// Artseyコンボキー定義（ARTSEY_ALPHAレイヤー）
const uint16_t PROGMEM combo_ar_12[] = {AR_1, AR_2, COMBO_END};
const uint16_t PROGMEM combo_ar_23[] = {AR_2, AR_3, COMBO_END};
const uint16_t PROGMEM combo_ar_3m[] = {AR_3, AR_MINUS, COMBO_END};
const uint16_t PROGMEM combo_ar_45[] = {AR_4, AR_5, COMBO_END};
const uint16_t PROGMEM combo_ar_56[] = {AR_5, AR_6, COMBO_END};
const uint16_t PROGMEM combo_ar_6e[] = {AR_6, AR_ENT, COMBO_END};
const uint16_t PROGMEM combo_ar_14[] = {AR_1, AR_4, COMBO_END};
const uint16_t PROGMEM combo_ar_25[] = {AR_2, AR_5, COMBO_END};
const uint16_t PROGMEM combo_ar_36[] = {AR_3, AR_6, COMBO_END};
const uint16_t PROGMEM combo_ar_me[] = {AR_MINUS, AR_ENT, COMBO_END};
const uint16_t PROGMEM combo_ar_1m[] = {AR_1, AR_MINUS, COMBO_END};
const uint16_t PROGMEM combo_ar_4e[] = {AR_4, AR_ENT, COMBO_END};
const uint16_t PROGMEM combo_ar_2m[] = {AR_2, AR_MINUS, COMBO_END};
const uint16_t PROGMEM combo_ar_46[] = {AR_4, AR_6, COMBO_END};
const uint16_t PROGMEM combo_ar_5e[] = {AR_5, AR_ENT, COMBO_END};
const uint16_t PROGMEM combo_ar_15[] = {AR_1, AR_5, COMBO_END};
const uint16_t PROGMEM combo_ar_26[] = {AR_2, AR_6, COMBO_END};
const uint16_t PROGMEM combo_ar_16[] = {AR_1, AR_6, COMBO_END};
const uint16_t PROGMEM combo_ar_1e[] = {AR_1, AR_ENT, COMBO_END};
const uint16_t PROGMEM combo_ar_24[] = {AR_2, AR_4, COMBO_END};  // 新規

// Artseyコンボキー定義（3キー）
const uint16_t PROGMEM combo_ar_123[] = {AR_1, AR_2, AR_3, COMBO_END};
const uint16_t PROGMEM combo_ar_56e[] = {AR_5, AR_6, AR_ENT, COMBO_END};
const uint16_t PROGMEM combo_ar_13m[] = {AR_1, AR_3, AR_MINUS, COMBO_END};
const uint16_t PROGMEM combo_ar_46e[] = {AR_4, AR_6, AR_ENT, COMBO_END};
const uint16_t PROGMEM combo_ar_23m[] = {AR_2, AR_3, AR_MINUS, COMBO_END};
const uint16_t PROGMEM combo_ar_456[] = {AR_4, AR_5, AR_6, COMBO_END};
const uint16_t PROGMEM combo_ar_12e[] = {AR_1, AR_2, AR_ENT, COMBO_END};  // 新規

// Artseyコンボキー定義（4キー）
const uint16_t PROGMEM combo_ar_456e[] = {AR_4, AR_5, AR_6, AR_ENT, COMBO_END};
const uint16_t PROGMEM combo_ar_123m[] = {AR_1, AR_2, AR_3, AR_MINUS, COMBO_END};
const uint16_t PROGMEM combo_ar_123e[] = {AR_1, AR_2, AR_3, AR_ENT, COMBO_END};  // 新規
const uint16_t PROGMEM combo_ar_423m[] = {AR_4, AR_2, AR_3, AR_MINUS, COMBO_END};  // 新規
const uint16_t PROGMEM combo_ar_156e[] = {AR_1, AR_5, AR_6, AR_ENT, COMBO_END};  // 新規

combo_t key_combos[COMBO_COUNT] = {
    // NIKOTOUCHコンボ
    [COMBO_56_STAR] = COMBO(combo_56, CMB_56),
    [COMBO_67_ZERO] = COMBO(combo_67, CMB_67),
    [COMBO_79_REPLAY] = COMBO(combo_79, CMB_79),
    [COMBO_1BS] = COMBO(combo_1bs, CMB_1BS),
    [COMBO_2BS] = COMBO(combo_2bs, CMB_2BS),
    [COMBO_3BS] = COMBO(combo_3bs, CMB_3BS),
    
    // レイヤー切り替えコンボ（NIKOTOUCH用）
    [COMBO_NK_LAYER_TOGGLE] = COMBO(combo_nk_layer_toggle, CMB_LAYER_TOGGLE),
    [COMBO_NK_LAYER_BASE] = COMBO(combo_nk_layer_base, CMB_LAYER_BASE),
    
    // レイヤー切り替えコンボ（ARTSEY用）
    [COMBO_AR_LAYER_TOGGLE] = COMBO(combo_ar_layer_toggle, CMB_LAYER_TOGGLE),
    [COMBO_AR_LAYER_BASE] = COMBO(combo_ar_layer_base, CMB_LAYER_BASE),
    
    // レイヤー切り替えコンボ（BASE用）
    [COMBO_BASE_LAYER_BACK] = COMBO(combo_base_layer_back, CMB_LAYER_BASE),
    
    // Artseyコンボ（ARTSEY_ALPHAレイヤー）2キー
    [COMBO_AR_12] = COMBO(combo_ar_12, CMB_AR_12),
    [COMBO_AR_23] = COMBO(combo_ar_23, CMB_AR_23),
    [COMBO_AR_3M] = COMBO(combo_ar_3m, CMB_AR_3M),
    [COMBO_AR_45] = COMBO(combo_ar_45, CMB_AR_45),
    [COMBO_AR_56] = COMBO(combo_ar_56, CMB_AR_56),
    [COMBO_AR_6E] = COMBO(combo_ar_6e, CMB_AR_6E),
    [COMBO_AR_14] = COMBO(combo_ar_14, CMB_AR_14),
    [COMBO_AR_25] = COMBO(combo_ar_25, CMB_AR_25),
    [COMBO_AR_36] = COMBO(combo_ar_36, CMB_AR_36),
    [COMBO_AR_ME] = COMBO(combo_ar_me, CMB_AR_ME),
    [COMBO_AR_1M] = COMBO(combo_ar_1m, CMB_AR_1M),
    [COMBO_AR_4E] = COMBO(combo_ar_4e, CMB_AR_4E),
    [COMBO_AR_2M] = COMBO(combo_ar_2m, CMB_AR_2M),
    [COMBO_AR_46] = COMBO(combo_ar_46, CMB_AR_46),
    [COMBO_AR_5E] = COMBO(combo_ar_5e, CMB_AR_5E),
    [COMBO_AR_15] = COMBO(combo_ar_15, CMB_AR_15),
    [COMBO_AR_26] = COMBO(combo_ar_26, CMB_AR_26),
    [COMBO_AR_16] = COMBO(combo_ar_16, CMB_AR_16),
    [COMBO_AR_1E] = COMBO(combo_ar_1e, CMB_AR_1E),
    [COMBO_AR_24] = COMBO(combo_ar_24, CMB_AR_24),  // 新規
    
    // Artseyコンボ 3キー
    [COMBO_AR_123] = COMBO(combo_ar_123, CMB_AR_123),
    [COMBO_AR_56E] = COMBO(combo_ar_56e, CMB_AR_56E),
    [COMBO_AR_13M] = COMBO(combo_ar_13m, CMB_AR_13M),
    [COMBO_AR_46E] = COMBO(combo_ar_46e, CMB_AR_46E),
    [COMBO_AR_23M] = COMBO(combo_ar_23m, CMB_AR_23M),
    [COMBO_AR_456] = COMBO(combo_ar_456, CMB_AR_456),
    [COMBO_AR_12E] = COMBO(combo_ar_12e, CMB_AR_12E),  // 新規
    
    // Artseyコンボ 4キー
    [COMBO_AR_456E] = COMBO(combo_ar_456e, CMB_AR_456E),
    [COMBO_AR_123M] = COMBO(combo_ar_123m, CMB_AR_123M),
    [COMBO_AR_123E] = COMBO(combo_ar_123e, CMB_AR_123E),  // 新規
    [COMBO_AR_423M] = COMBO(combo_ar_423m, CMB_AR_423M),  // 新規
    [COMBO_AR_156E] = COMBO(combo_ar_156e, CMB_AR_156E),  // 新規
};

// コンボのタイミング設定
uint16_t get_combo_term(uint16_t index, combo_t *combo) {
    return COMBO_TERM_56;
}

// コンボのレイヤー別有効化
bool combo_should_trigger(uint16_t combo_index, combo_t *combo, uint16_t keycode, keyrecord_t *record) {
    // NIKOTOUCHコンボ（COMBO_56_STAR～COMBO_3BS）
    if (combo_index >= COMBO_56_STAR && combo_index <= COMBO_3BS) {
        return layer_state_is(NT_NIKOTOUCH);
    }
    
    // NIKOTOUCHレイヤー切り替えコンボ
    if (combo_index == COMBO_NK_LAYER_TOGGLE || combo_index == COMBO_NK_LAYER_BASE) {
        return layer_state_is(NT_NIKOTOUCH) || layer_state_is(NT_NIKOSHIFT);
    }
    
    // ARTSEYレイヤー切り替えコンボ
    if (combo_index == COMBO_AR_LAYER_TOGGLE || combo_index == COMBO_AR_LAYER_BASE) {
        return layer_state_is(NT_ARTSEY_ALPHA) || 
               layer_state_is(NT_ARTSEY_KAKO) || 
               layer_state_is(NT_ARTSEY_SYMBOL1) || 
               layer_state_is(NT_ARTSEY_SYMBOL2) || 
               layer_state_is(NT_ARTSEY_SYMBOL3);
    }
    
    // BASEレイヤー切り替えコンボ
    if (combo_index == COMBO_BASE_LAYER_BACK) {
        return layer_state_is(NT_BASE) || layer_state_is(NT_FN);
    }
    
    // Artseyコンボ（COMBO_AR_12～COMBO_AR_156E）
    if (combo_index >= COMBO_AR_12 && combo_index <= COMBO_AR_156E) {
        return layer_state_is(NT_ARTSEY_ALPHA) || 
               layer_state_is(NT_ARTSEY_KAKO);
    }
    
    return false;
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
        NK_M2,      NK_4,    NK_5,    NK_6,    NK_ENT,
        NK_M3,      NK_7,    NK_8,    NK_9,
        NK_M4,      NK_STAR, NK_0,    KC_HASH, NK_ENT2,
        NK_M5,      KC_SPC,       MO(NT_NIKOSHIFT)  ),

    [NT_NIKOSHIFT] = LAYOUT_tenkey_27(
        _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______,
        _______, KC_LEFT, KC_DOWN, KC_UP,   KC_RIGHT,
        _______, KC_LEFT, KC_DOWN, KC_UP,
        _______, _______, _______, _______, NK_SENT,
        _______, _______,          _______          ),

    // Artseyレイヤー - ALPHA (基本英字入力)
    [NT_ARTSEY_ALPHA] = LAYOUT_tenkey_27(
        KC_MUTE,    KC_ESC,  KC_DEL,  KC_TAB,  KC_BSPC,
        NK_M1,      AR_1,    AR_2,    AR_3,    AR_MINUS,
        NK_M2,      AR_4,    AR_5,    AR_6,    AR_ENT,
        NK_M3,      AR_7,    AR_8,    AR_9,
        NK_M4,      AR_STAR, AR_0,    AR_HASH, AR_ENT2,
        NK_M5,      AR_SPC,          AR_DOT            ),

    // Artseyレイヤー - KAKO (括弧・記号)
    [NT_ARTSEY_KAKO] = LAYOUT_tenkey_27(
        KC_MUTE,    KC_ESC,  KC_DEL,  KC_TAB,     KC_BSPC,
        NK_M1,      AR_K_1,  AR_K_2,  AR_K_3,   AR_K_MINUS,
        NK_M2,      AR_K_4,  AR_K_5,  AR_K_6,   AR_K_ENT,
        NK_M3,      KC_P7,   KC_P8,   KC_P9,
        NK_M4,      KC_P1,   KC_P2,   KC_P3,    KC_PENT,
        NK_M5,      KC_P0,           AR_K_DOT          ),

    // Artseyレイヤー - SYMBOL1 (記号1: # ` ; \ N/A @ - =)
    [NT_ARTSEY_SYMBOL1] = LAYOUT_tenkey_27(
        KC_MUTE,    KC_ESC,   KC_DEL,   KC_TAB,        KC_BSPC,
        NK_M1,      AR_S1_1,  AR_S1_2,  AR_S1_3,   AR_S1_MINUS,
        NK_M2,      AR_S1_4,  AR_S1_5,  AR_S1_6,   AR_S1_ENT,
        NK_M3,      KC_P7,    KC_P8,    KC_P9,
        NK_M4,      KC_P1,    KC_P2,    KC_P3,     KC_PENT,
        NK_M5,      KC_P0,             AR_S1_DOT          ),

    // Artseyレイヤー - SYMBOL2 (記号2: $ % ^ N/A & * _ N/A)
    [NT_ARTSEY_SYMBOL2] = LAYOUT_tenkey_27(
        KC_MUTE,    KC_ESC,   KC_DEL,   KC_TAB,        KC_BSPC,
        NK_M1,      AR_S2_1,  AR_S2_2,  AR_S2_3,   AR_S2_MINUS,
        NK_M2,      AR_S2_4,  AR_S2_5,  AR_S2_6,   AR_S2_ENT,
        NK_M3,      KC_P7,    KC_P8,    KC_P9,
        NK_M4,      KC_P1,    KC_P2,    KC_P3,     KC_PENT,
        NK_M5,      KC_P0,             AR_S2_DOT          ),

    // Artseyレイヤー - SYMBOL3 (記号3: + | : N/A < > ~ N/A)
    [NT_ARTSEY_SYMBOL3] = LAYOUT_tenkey_27(
        KC_MUTE,    KC_ESC,   KC_DEL,   KC_TAB,        KC_BSPC,
        NK_M1,      AR_S3_1,  AR_S3_2,  AR_S3_3,   AR_S3_MINUS,
        NK_M2,      AR_S3_4,  AR_S3_5,  AR_S3_6,   AR_S3_ENT,
        NK_M3,      KC_P7,    KC_P8,    KC_P9,
        NK_M4,      KC_P1,    KC_P2,    KC_P3,     KC_PENT,
        NK_M5,      KC_P0,             AR_S3_DOT          )
};
// clang-format on

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [NT_BASE]            = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [NT_FN]              = {ENCODER_CCW_CW(RGB_VAD, RGB_VAI)},
    [NT_NIKOTOUCH]       = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [NT_NIKOSHIFT]       = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [NT_ARTSEY_ALPHA]    = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [NT_ARTSEY_KAKO]     = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [NT_ARTSEY_SYMBOL1]  = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [NT_ARTSEY_SYMBOL2]  = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [NT_ARTSEY_SYMBOL3]  = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
};
#endif // ENCODER_MAP_ENABLE

// ============================================================
// タイムアウト処理
// ============================================================
void matrix_scan_user(void) {
    matrix_scan_nikotouch();
    matrix_scan_artsey();
}

// ============================================================
// キー処理
// ============================================================
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // Keychron共通処理
    if (!process_record_keychron_common(keycode, record)) {
        return false;
    }

    // Artsey処理
    if (!process_record_artsey(keycode, record)) {
        return false;
    }

    // ニコタッチ共通処理
    return process_record_nikotouch(keycode, record);
}
