// Copyright 2026 Goronaya (@nemohashi)
// SPDX-License-Identifier: GPL-2.0-or-later
//
// BELL-56-mini キーマップ
// 56式ポケベル入力 + Artsey入力対応

#include QMK_KEYBOARD_H
#include "nikotouch.h"
#include "artsey.h"

// ============================================================
// コンボ定義
// ============================================================
enum combos {
    // 56式コンボ（NIKOTOUCHレイヤー）
    COMBO_56_STAR,      // 5+6 = 濁点（*キー相当）
    COMBO_67_ZERO,      // 6+7 = 0キー相当（mini版では不要だが互換性のため残す）
    COMBO_79_REPLAY,    // 7+9 = 確定前バッファから残りを再入力
    COMBO_1BS,          // 1+BS = 2文字削除
    COMBO_2BS,          // 2+BS = 3文字削除
    COMBO_3BS,          // 3+BS = 4文字削除
    
    // レイヤー切り替えコンボ（NIKOTOUCH用）
    COMBO_NK_LAYER_TOGGLE,  // Space+Ent = NIKOTOUCH ⇔ ARTSEY
    COMBO_NK_LAYER_BASE,    // 0+Ent = BASE切り替え
    
    // レイヤー切り替えコンボ（ARTSEY用）
    COMBO_AR_LAYER_TOGGLE,  // Space+Ent = NIKOTOUCH ⇔ ARTSEY
    COMBO_AR_LAYER_BASE,    // 0+Ent = BASE切り替え
    
    // レイヤー切り替えコンボ（BASE用）
    COMBO_BASE_LAYER_BACK,  // Space+0 = BASE → 前レイヤー
    
    // Artseyコンボ（ARTSEY_ALPHAレイヤー）2キー
    COMBO_AR_12,    // 1+2 = f
    COMBO_AR_23,    // 2+3 = g
    COMBO_AR_3M,    // 3+BS(-) = j
    COMBO_AR_45,    // 4+5 = c
    COMBO_AR_56,    // 5+6 = u
    COMBO_AR_6E,    // 6+Ent = n
    COMBO_AR_14,    // 1+4 = Enter
    COMBO_AR_25,    // 2+5 = '
    COMBO_AR_36,    // 3+6 = !
    COMBO_AR_ME,    // BS(-)+Ent = ?
    COMBO_AR_1M,    // 1+BS(-) = w
    COMBO_AR_4E,    // 4+Ent = b
    COMBO_AR_2M,    // 2+BS(-) = v
    COMBO_AR_46,    // 4+6 = h
    COMBO_AR_5E,    // 5+Ent = k
    COMBO_AR_15,    // 1+5 = ,
    COMBO_AR_26,    // 2+6 = Delete
    COMBO_AR_16,    // 1+6 = .
    COMBO_AR_1E,    // 1+Ent = /
    COMBO_AR_24,    // 2+4 = Backspace
    
    // Artseyコンボ 3キー
    COMBO_AR_123,   // 1+2+3 = d
    COMBO_AR_56E,   // 5+6+Ent = m
    COMBO_AR_13M,   // 1+3+BS(-) = q
    COMBO_AR_46E,   // 4+6+Ent = p
    COMBO_AR_23M,   // 2+3+BS(-) = x
    COMBO_AR_456,   // 4+5+6 = l
    COMBO_AR_12E,   // 1+2+Ent = Esc
    
    // Artseyコンボ 4キー
    COMBO_AR_456E,  // 4+5+6+Ent = Space
    COMBO_AR_123M,  // 1+2+3+BS(-) = z
    COMBO_AR_123E,  // 1+2+3+Ent = Tab
    COMBO_AR_423M,  // 4+2+3+BS(-) = Once Shift
    COMBO_AR_156E,  // 1+5+6+Ent = Toggle Shift
    
    COMBO_COUNT
};

// ============================================================
// コンボキー定義
// ============================================================
// 56式コンボ（NIKOTOUCHレイヤー）
const uint16_t PROGMEM combo_56[] = {NK_5, NK_6, COMBO_END};
const uint16_t PROGMEM combo_67[] = {NK_6, NK_7, COMBO_END};
const uint16_t PROGMEM combo_79[] = {NK_7, NK_9, COMBO_END};
const uint16_t PROGMEM combo_1bs[] = {NK_1, NK_BS, COMBO_END};
const uint16_t PROGMEM combo_2bs[] = {NK_2, NK_BS, COMBO_END};
const uint16_t PROGMEM combo_3bs[] = {NK_3, NK_BS, COMBO_END};

// レイヤー切り替えコンボ（NIKOTOUCH用: Space + Ent/0）
const uint16_t PROGMEM combo_nk_layer_toggle[] = {KC_SPC, NK_ENT, COMBO_END};
const uint16_t PROGMEM combo_nk_layer_base[] = {NK_0, NK_ENT, COMBO_END};  // 0キー

// レイヤー切り替えコンボ（ARTSEY用: Space + Ent/0）
const uint16_t PROGMEM combo_ar_layer_toggle[] = {AR_SPC, AR_ENT, COMBO_END};
const uint16_t PROGMEM combo_ar_layer_base[] = {AR_DOT, AR_ENT, COMBO_END};  // 0キー=AR_DOT

// レイヤー切り替えコンボ（BASE用: Space + 0）
const uint16_t PROGMEM combo_base_layer_back[] = {KC_SPC, KC_P0, COMBO_END};

// Artseyコンボキー定義（2キー）
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
const uint16_t PROGMEM combo_ar_24[] = {AR_2, AR_4, COMBO_END};

// Artseyコンボキー定義（3キー）
const uint16_t PROGMEM combo_ar_123[] = {AR_1, AR_2, AR_3, COMBO_END};
const uint16_t PROGMEM combo_ar_56e[] = {AR_5, AR_6, AR_ENT, COMBO_END};
const uint16_t PROGMEM combo_ar_13m[] = {AR_1, AR_3, AR_MINUS, COMBO_END};
const uint16_t PROGMEM combo_ar_46e[] = {AR_4, AR_6, AR_ENT, COMBO_END};
const uint16_t PROGMEM combo_ar_23m[] = {AR_2, AR_3, AR_MINUS, COMBO_END};
const uint16_t PROGMEM combo_ar_456[] = {AR_4, AR_5, AR_6, COMBO_END};
const uint16_t PROGMEM combo_ar_12e[] = {AR_1, AR_2, AR_ENT, COMBO_END};

// Artseyコンボキー定義（4キー）
const uint16_t PROGMEM combo_ar_456e[] = {AR_4, AR_5, AR_6, AR_ENT, COMBO_END};
const uint16_t PROGMEM combo_ar_123m[] = {AR_1, AR_2, AR_3, AR_MINUS, COMBO_END};
const uint16_t PROGMEM combo_ar_123e[] = {AR_1, AR_2, AR_3, AR_ENT, COMBO_END};
const uint16_t PROGMEM combo_ar_423m[] = {AR_4, AR_2, AR_3, AR_MINUS, COMBO_END};
const uint16_t PROGMEM combo_ar_156e[] = {AR_1, AR_5, AR_6, AR_ENT, COMBO_END};

combo_t key_combos[COMBO_COUNT] = {
    // 56式コンボ
    [COMBO_56_STAR] = COMBO(combo_56, CMB_56),
    [COMBO_67_ZERO] = COMBO(combo_67, CMB_67),
    [COMBO_79_REPLAY] = COMBO(combo_79, CMB_79),
    [COMBO_1BS] = COMBO(combo_1bs, CMB_1BS),
    [COMBO_2BS] = COMBO(combo_2bs, CMB_2BS),
    [COMBO_3BS] = COMBO(combo_3bs, CMB_3BS),
    
    // レイヤー切り替えコンボ
    [COMBO_NK_LAYER_TOGGLE] = COMBO(combo_nk_layer_toggle, CMB_LAYER_TOGGLE),
    [COMBO_NK_LAYER_BASE] = COMBO(combo_nk_layer_base, CMB_LAYER_BASE),
    [COMBO_AR_LAYER_TOGGLE] = COMBO(combo_ar_layer_toggle, CMB_LAYER_TOGGLE),
    [COMBO_AR_LAYER_BASE] = COMBO(combo_ar_layer_base, CMB_LAYER_BASE),
    [COMBO_BASE_LAYER_BACK] = COMBO(combo_base_layer_back, CMB_LAYER_BASE),
    
    // Artseyコンボ 2キー
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
    [COMBO_AR_24] = COMBO(combo_ar_24, CMB_AR_24),
    
    // Artseyコンボ 3キー
    [COMBO_AR_123] = COMBO(combo_ar_123, CMB_AR_123),
    [COMBO_AR_56E] = COMBO(combo_ar_56e, CMB_AR_56E),
    [COMBO_AR_13M] = COMBO(combo_ar_13m, CMB_AR_13M),
    [COMBO_AR_46E] = COMBO(combo_ar_46e, CMB_AR_46E),
    [COMBO_AR_23M] = COMBO(combo_ar_23m, CMB_AR_23M),
    [COMBO_AR_456] = COMBO(combo_ar_456, CMB_AR_456),
    [COMBO_AR_12E] = COMBO(combo_ar_12e, CMB_AR_12E),
    
    // Artseyコンボ 4キー
    [COMBO_AR_456E] = COMBO(combo_ar_456e, CMB_AR_456E),
    [COMBO_AR_123M] = COMBO(combo_ar_123m, CMB_AR_123M),
    [COMBO_AR_123E] = COMBO(combo_ar_123e, CMB_AR_123E),
    [COMBO_AR_423M] = COMBO(combo_ar_423m, CMB_AR_423M),
    [COMBO_AR_156E] = COMBO(combo_ar_156e, CMB_AR_156E),
};

// コンボのタイミング設定
uint16_t get_combo_term(uint16_t index, combo_t *combo) {
    return COMBO_TERM_56;
}

// コンボのレイヤー別有効化
bool combo_should_trigger(uint16_t combo_index, combo_t *combo, uint16_t keycode, keyrecord_t *record) {
    // 56式コンボ（COMBO_56_STAR～COMBO_3BS）
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
    
    // Artseyコンボ
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
    /*
     * BASEレイヤー（テンキー）
     * ┌──────┬──────┬──────┬──────┐
     * │      │      │      │MO(FN)│
     * ├──────┼──────┼──────┼──────┤
     * │  7   │  8   │  9   │ BSPC │
     * ├──────┼──────┼──────┼──────┤
     * │  4   │  5   │  6   │      │
     * ├──────┼──────┼──────┤ ENT  │
     * │  1   │  2   │  3   │      │
     * ├──────┴──────┼──────┴──────┤
     * │    SPC      │      0      │
     * └─────────────┴─────────────┘
     */
    [NT_BASE] = LAYOUT(
        KC_P7,   KC_P8,   KC_P9,   KC_BSPC, MO(NT_FN),
        KC_P4,   KC_P5,   KC_P6,   KC_P1,   KC_P2,
        KC_P3,   KC_SPC,  KC_P0,   KC_PENT
    ),

    /*
     * FNレイヤー
     */
    [NT_FN] = LAYOUT(
        KC_HOME, KC_UP,   KC_PGUP, QK_BOOT, _______,
        KC_LEFT, KC_DOWN, KC_RGHT, KC_END,  KC_DOWN,
        KC_PGDN, _______, _______, _______
    ),

    /*
     * NIKOTOUCHレイヤー（56式ポケベル入力）
     * ┌──────┬──────┬──────┬──────┐
     * │      │      │      │MO(SH)│
     * ├──────┼──────┼──────┼──────┤
     * │ NK_1 │ NK_2 │ NK_3 │ NK_BS│
     * ├──────┼──────┼──────┼──────┤
     * │ NK_4 │ NK_5 │ NK_6 │      │
     * ├──────┼──────┼──────┤NK_ENT│
     * │ NK_7 │ NK_8 │ NK_9 │      │
     * ├──────┴──────┼──────┴──────┤
     * │    SPC      │    NK_0     │  ← 0キーが.キーの役割も担う
     * └─────────────┴─────────────┘
     */
    [NT_NIKOTOUCH] = LAYOUT(
        NK_1,    NK_2,    NK_3,    NK_BS,   MO(NT_NIKOSHIFT),
        NK_4,    NK_5,    NK_6,    NK_7,    NK_8,
        NK_9,    KC_SPC,  NK_0,    NK_ENT
    ),

    /*
     * NIKOSHIFTレイヤー（矢印キー等）
     */
    [NT_NIKOSHIFT] = LAYOUT(
        _______, _______, _______, _______, _______,
        KC_LEFT, KC_DOWN, KC_UP,   _______, _______,
        _______, _______, _______, KC_RGHT
    ),

    /*
     * ARTSEY_ALPHAレイヤー
     * ┌──────┬──────┬──────┬──────┐
     * │      │      │      │      │
     * ├──────┼──────┼──────┼──────┤
     * │ AR_1 │ AR_2 │ AR_3 │AR_MNS│ ← a, r, t, s(hold=KAKO)
     * ├──────┼──────┼──────┼──────┤
     * │ AR_4 │ AR_5 │ AR_6 │      │ ← e, y, i (hold=SYM1)
     * ├──────┼──────┼──────┤AR_ENT│ ← o (hold=SYM3)
     * │ AR_7 │ AR_8 │ AR_9 │      │ ← BS, N/A, N/A
     * ├──────┴──────┼──────┴──────┤
     * │   AR_SPC    │   AR_DOT    │
     * └─────────────┴─────────────┘
     */
    [NT_ARTSEY_ALPHA] = LAYOUT(
        AR_1,    AR_2,    AR_3,    AR_MINUS, MO(NT_NIKOSHIFT),
        AR_4,    AR_5,    AR_6,    AR_7,     AR_8,
        AR_9,    AR_SPC,  AR_DOT,  AR_ENT
    ),

    /*
     * ARTSEY_KAKOレイヤー（括弧入力）
     */
    [NT_ARTSEY_KAKO] = LAYOUT(
        AR_K_1,  AR_K_2,  AR_K_3,  AR_K_MINUS, _______,
        AR_K_4,  AR_K_5,  AR_K_6,  KC_P7,      KC_P8,
        KC_P9,   KC_SPC,  AR_K_DOT, AR_K_ENT
    ),

    /*
     * ARTSEY_SYMBOL1レイヤー
     */
    [NT_ARTSEY_SYMBOL1] = LAYOUT(
        AR_S1_1, AR_S1_2, AR_S1_3, AR_S1_MINUS, _______,
        AR_S1_4, AR_S1_5, AR_S1_6, KC_P7,       KC_P8,
        KC_P9,   KC_SPC,  AR_S1_DOT, AR_S1_ENT
    ),

    /*
     * ARTSEY_SYMBOL2レイヤー
     */
    [NT_ARTSEY_SYMBOL2] = LAYOUT(
        AR_S2_1, AR_S2_2, AR_S2_3, AR_S2_MINUS, _______,
        AR_S2_4, AR_S2_5, AR_S2_6, KC_P7,       KC_P8,
        KC_P9,   KC_SPC,  AR_S2_DOT, AR_S2_ENT
    ),

    /*
     * ARTSEY_SYMBOL3レイヤー
     */
    [NT_ARTSEY_SYMBOL3] = LAYOUT(
        AR_S3_1, AR_S3_2, AR_S3_3, AR_S3_MINUS, _______,
        AR_S3_4, AR_S3_5, AR_S3_6, KC_P7,       KC_P8,
        KC_P9,   KC_SPC,  AR_S3_DOT, AR_S3_ENT
    ),
};
// clang-format on

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
    // Artsey処理
    if (!process_record_artsey(keycode, record)) {
        return false;
    }

    // ニコタッチ共通処理
    return process_record_nikotouch(keycode, record);
}
