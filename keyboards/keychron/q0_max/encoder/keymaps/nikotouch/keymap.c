/* Copyright 2024 @ Keychron (https://www.keychron.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H
#include "keychron_common.h"

// ============================================================
// ニコタッチ設定
// ============================================================
#define NIKOTOUCH_TIMEOUT_MS 3000  // タイムアウト時間（ミリ秒）

// ============================================================
// カスタムキーコード
// ============================================================
enum custom_keycodes {
    NK_1 = SAFE_RANGE,
    NK_2,
    NK_3,
    NK_4,
    NK_5,
    NK_6,
    NK_7,
    NK_8,
    NK_9,
    NK_0,
    NK_STAR,     // * キー（濁音・半濁音変換）
    NK_BS,       // Backspace（バッファクリア機能付き）
    NK_M1,       // M1: BASEレイヤー切替 + 英数
    NK_M2,       // M2: NIKOTOUCHレイヤー切替 + かな
    NK_M3,       // M3: 未割り当て
    NK_M4,       // M4: 未割り当て
    NK_M5,       // M5: MO(FN)
};

// ============================================================
// レイヤー定義
// ============================================================
enum layers {
    BASE,
    FN,
    NIKOTOUCH,
    NIKOSHIFT,
};

// ============================================================
// ニコタッチ変換テーブル
// ============================================================
typedef struct {
    uint8_t key1;           // 1回目のキー (0-9)
    uint8_t key2;           // 2回目のキー (0-9)
    const char *output;     // 送信文字列
    const char *star1;      // *1回目変換（NULL = *入力待ちなし）
    const char *star2;      // *2回目変換（NULL = *2回目なし）
} nikotouch_entry_t;

static const nikotouch_entry_t nikotouch_table[] = {
    // 1 + X: 母音行
    {1, 1, "a", "la", NULL},
    {1, 2, "i", "li", NULL},
    {1, 3, "u", "lu", NULL},
    {1, 4, "e", "le", NULL},
    {1, 5, "o", "lo", NULL},
    {1, 6, ".", NULL, NULL},
    {1, 7, "-", NULL, NULL},
    {1, 8, "@", NULL, NULL},
    {1, 9, "_", NULL, NULL},
    {1, 0, "1", NULL, NULL},
    // 2 + X: か行
    {2, 1, "ka", "ga", "lka"},
    {2, 2, "ki", "gi", NULL},
    {2, 3, "ku", "gu", NULL},
    {2, 4, "ke", "ge", "lke"},
    {2, 5, "ko", "go", NULL},
    {2, 6, ",", NULL, NULL},
    {2, 7, "a", NULL, NULL},
    {2, 8, "b", NULL, NULL},
    {2, 9, "c", NULL, NULL},
    {2, 0, "2", NULL, NULL},
    // 3 + X: さ行
    {3, 1, "sa", "za", NULL},
    {3, 2, "si", "ji", NULL},
    {3, 3, "su", "zu", NULL},
    {3, 4, "se", "ze", NULL},
    {3, 5, "so", "zo", NULL},
    {3, 6, ",", NULL, NULL},
    {3, 7, "d", NULL, NULL},
    {3, 8, "e", NULL, NULL},
    {3, 9, "f", NULL, NULL},
    {3, 0, "3", NULL, NULL},
    // 4 + X: た行
    {4, 1, "ta", "da", NULL},
    {4, 2, "ti", "di", NULL},
    {4, 3, "tu", "ltu", "du"},
    {4, 4, "te", "de", NULL},
    {4, 5, "to", "do", NULL},
    {4, 6, "ltu", NULL, NULL},
    {4, 7, "g", NULL, NULL},
    {4, 8, "h", NULL, NULL},
    {4, 9, "i", NULL, NULL},
    {4, 0, "4", NULL, NULL},
    // 5 + X: な行
    {5, 1, "na", NULL, NULL},
    {5, 2, "ni", NULL, NULL},
    {5, 3, "nu", NULL, NULL},
    {5, 4, "ne", NULL, NULL},
    {5, 5, "no", NULL, NULL},
    {5, 6, ",", NULL, NULL},
    {5, 7, "j", NULL, NULL},
    {5, 8, "k", NULL, NULL},
    {5, 9, "l", NULL, NULL},
    {5, 0, "5", NULL, NULL},
    // 6 + X: は行
    {6, 1, "ha", "ba", "pa"},
    {6, 2, "hi", "bi", "pi"},
    {6, 3, "hu", "bu", "pu"},
    {6, 4, "he", "be", "pe"},
    {6, 5, "ho", "bo", "po"},
    {6, 6, ",", NULL, NULL},
    {6, 7, "m", NULL, NULL},
    {6, 8, "n", NULL, NULL},
    {6, 9, "o", NULL, NULL},
    {6, 0, "6", NULL, NULL},
    // 7 + X: ま行
    {7, 1, "ma", NULL, NULL},
    {7, 2, "mi", NULL, NULL},
    {7, 3, "mu", NULL, NULL},
    {7, 4, "me", NULL, NULL},
    {7, 5, "mo", NULL, NULL},
    {7, 6, "p", NULL, NULL},
    {7, 7, "q", NULL, NULL},
    {7, 8, "r", NULL, NULL},
    {7, 9, "s", NULL, NULL},
    {7, 0, "7", NULL, NULL},
    // 8 + X: や行
    {8, 1, "ya", NULL, NULL},
    {8, 2, "yu", NULL, NULL},
    {8, 3, "yo", NULL, NULL},
    {8, 4, "lya", NULL, NULL},
    {8, 5, "lyu", NULL, NULL},
    {8, 6, "lyo", NULL, NULL},
    {8, 7, "t", NULL, NULL},
    {8, 8, "u", NULL, NULL},
    {8, 9, "v", NULL, NULL},
    {8, 0, "8", NULL, NULL},
    // 9 + X: ら行
    {9, 1, "ra", NULL, NULL},
    {9, 2, "ri", NULL, NULL},
    {9, 3, "ru", NULL, NULL},
    {9, 4, "re", NULL, NULL},
    {9, 5, "ro", NULL, NULL},
    {9, 6, "w", NULL, NULL},
    {9, 7, "x", NULL, NULL},
    {9, 8, "y", NULL, NULL},
    {9, 9, "z", NULL, NULL},
    {9, 0, "9", NULL, NULL},
    // 0 + X: わ行・記号
    {0, 1, "wa", NULL, NULL},
    {0, 2, "wo", NULL, NULL},
    {0, 3, "nn", NULL, NULL},
    {0, 4, ",", NULL, NULL},
    {0, 5, ".", NULL, NULL},
    {0, 6, "-", NULL, NULL},
    {0, 7, ":", NULL, NULL},
    {0, 8, "!", NULL, NULL},
    {0, 9, "?", NULL, NULL},
    {0, 0, "0", NULL, NULL},
};

#define NIKOTOUCH_TABLE_SIZE (sizeof(nikotouch_table) / sizeof(nikotouch_table[0]))

// ============================================================
// ニコタッチ状態管理
// ============================================================
static uint8_t niko_buffer = 0xFF;           // 1回目のキー (0xFF = 未入力)
static uint16_t niko_buffer_time = 0;        // 1回目キー押下時刻
static bool star_mode = false;               // *入力待ちモード
static uint8_t star_count = 0;               // *を押した回数 (0, 1, 2)
static const char *star_buffer = NULL;       // *バッファ（現在の出力文字列）
static const char *star1_next = NULL;        // 次の*1回目変換
static const char *star2_next = NULL;        // 次の*2回目変換

// ニコタッチ状態をクリア
static void niko_clear(void) {
    niko_buffer = 0xFF;
    niko_buffer_time = 0;
}

// *モードをクリア
static void star_clear(void) {
    star_mode = false;
    star_count = 0;
    star_buffer = NULL;
    star1_next = NULL;
    star2_next = NULL;
}

// 文字列の長さを取得（Backspace送信用）
static uint8_t get_output_length(const char *str) {
    if (str == NULL) return 0;
    uint8_t len = 0;
    while (*str) {
        len++;
        str++;
    }
    return len;
}

// Backspaceを指定回数送信
static void send_backspaces(uint8_t count) {
    for (uint8_t i = 0; i < count; i++) {
        tap_code(KC_BSPC);
    }
}

// ニコタッチ変換テーブルを検索
static const nikotouch_entry_t* find_nikotouch_entry(uint8_t key1, uint8_t key2) {
    for (uint8_t i = 0; i < NIKOTOUCH_TABLE_SIZE; i++) {
        if (nikotouch_table[i].key1 == key1 && nikotouch_table[i].key2 == key2) {
            return &nikotouch_table[i];
        }
    }
    return NULL;
}

// NK_1〜NK_0 を数値 0〜9 に変換
static uint8_t keycode_to_niko_num(uint16_t keycode) {
    switch (keycode) {
        case NK_0: return 0;
        case NK_1: return 1;
        case NK_2: return 2;
        case NK_3: return 3;
        case NK_4: return 4;
        case NK_5: return 5;
        case NK_6: return 6;
        case NK_7: return 7;
        case NK_8: return 8;
        case NK_9: return 9;
        default: return 0xFF;
    }
}

// ニコタッチキーかどうか
static bool is_nikotouch_key(uint16_t keycode) {
    return keycode >= NK_1 && keycode <= NK_0;
}

// ============================================================
// キーマップ定義
// ============================================================
// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [BASE] = LAYOUT_tenkey_27(
        KC_MUTE, KC_ESC,  KC_DEL,  KC_TAB,  KC_BSPC,
        NK_M1,   KC_NUM,  KC_PSLS, KC_PAST, KC_PMNS,
        NK_M2,   KC_P7,   KC_P8,   KC_P9,   KC_PPLS,
        NK_M3,   KC_P4,   KC_P5,   KC_P6,
        NK_M4,   KC_P1,   KC_P2,   KC_P3,   KC_PENT,
        NK_M5,   KC_P0,            KC_PDOT          ),

    [FN] = LAYOUT_tenkey_27(
        RGB_TOG, BT_HST1, BT_HST2, BT_HST3, P2P4G,
        NK_M1,   RGB_MOD, RGB_VAI, RGB_HUI, QK_BOOT,
        NK_M2,   RGB_RMOD,RGB_VAD, RGB_HUD, _______,
        NK_M3,   RGB_SAI, RGB_SPI, KC_MPRV,
        NK_M4,   RGB_SAD, RGB_SPD, KC_MPLY, _______,
        _______, RGB_TOG,          KC_MNXT          ),

    [NIKOTOUCH] = LAYOUT_tenkey_27(
        KC_MUTE,    KC_COMM, KC_DOT,  KC_LBRC, KC_RBRC,
        NK_M1,      NK_1,    NK_2,    NK_3,    NK_BS,
        NK_M2,      NK_4,    NK_5,    NK_6,    KC_ENT,
        NK_M3,      NK_7,    NK_8,    NK_9,
        NK_M4,      NK_STAR, NK_0,    KC_HASH, KC_ENT,
        NK_M5,      KC_SPC,       MO(NIKOSHIFT)     ),

    [NIKOSHIFT] = LAYOUT_tenkey_27(
        _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______,
        _______, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT,
        _______, _______, _______, _______,
        _______, _______, _______, _______, _______,
        _______, _______,          _______          )
};
// clang-format on

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [BASE]      = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [FN]        = {ENCODER_CCW_CW(RGB_VAD, RGB_VAI)},
    [NIKOTOUCH] = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [NIKOSHIFT] = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
};
#endif // ENCODER_MAP_ENABLE

// ============================================================
// タイムアウト処理
// ============================================================
void matrix_scan_user(void) {
    // ニコバッファのタイムアウトチェック
    if (niko_buffer != 0xFF) {
        if (timer_elapsed(niko_buffer_time) > NIKOTOUCH_TIMEOUT_MS) {
            niko_clear();
        }
    }
}

// ============================================================
// キー処理
// ============================================================
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // Keychron共通処理
    if (!process_record_keychron_common(keycode, record)) {
        return false;
    }

    // キーリリース時の処理
    if (!record->event.pressed) {
        // NK_M5（FNレイヤー切替）のリリース処理
        if (keycode == NK_M5) {
            layer_off(FN);
            return false;
        }
        // NK_BS（Backspace）のリリース処理
        if (keycode == NK_BS) {
            unregister_code(KC_BSPC);
            return false;
        }
        return true;
    }

    // カスタムキー処理
    switch (keycode) {
        // M1: BASEレイヤー + 英数（日本語OFF）
        case NK_M1:
            layer_move(BASE);
            tap_code(KC_LNG2);  // 英数キー
            return false;

        // M2: NIKOTOUCHレイヤー + かな（日本語ON）
        case NK_M2:
            layer_move(NIKOTOUCH);
            tap_code(KC_LNG1);  // かなキー
            niko_clear();
            star_clear();
            return false;

        // M3, M4: 未割り当て
        case NK_M3:
        case NK_M4:
            return false;

        // M5: MO(FN) と同等
        case NK_M5:
            // MO(FN)はキーマップで直接扱えないので、ここで処理
            layer_on(FN);
            return false;

        // Backspace（ニコタッチバッファクリア機能付き）
        case NK_BS:
            if (niko_buffer != 0xFF) {
                // ニコバッファがある場合はクリアのみ
                niko_clear();
                return false;
            } else if (star_mode) {
                // *バッファがある場合はクリア + Backspace送信（1回のみ）
                star_clear();
                tap_code(KC_BSPC);
                return false;
            } else {
                // 通常のBackspace（キーリピート対応）
                register_code(KC_BSPC);
                return false;
            }

        // * キー（濁音・半濁音変換）
        case NK_STAR:
            if (!star_mode) {
                // *入力待ちモードでない場合は無視
                return false;
            }

            star_count++;

            if (star_count == 1 && star1_next != NULL) {
                // *1回目変換
                uint8_t bs_count = get_output_length(star_buffer);
                send_backspaces(bs_count);
                send_string(star1_next);

                if (star2_next != NULL) {
                    // *2回目変換がある場合は継続
                    star_buffer = star1_next;
                } else {
                    // *2回目変換がない場合は終了
                    star_clear();
                }
            } else if (star_count == 2 && star2_next != NULL) {
                // *2回目変換
                uint8_t bs_count = get_output_length(star_buffer);
                send_backspaces(bs_count);
                send_string(star2_next);
                star_clear();
            } else {
                // これ以上変換がない
                star_clear();
            }
            return false;

        // ニコタッチ数字キー (NK_0 〜 NK_9)
        default:
            if (is_nikotouch_key(keycode)) {
                uint8_t num = keycode_to_niko_num(keycode);

                // *入力待ちモード中に*以外が押された場合
                if (star_mode) {
                    star_clear();
                }

                if (niko_buffer == 0xFF) {
                    // 1回目のキー入力
                    niko_buffer = num;
                    niko_buffer_time = timer_read();
                } else {
                    // 2回目のキー入力
                    const nikotouch_entry_t *entry = find_nikotouch_entry(niko_buffer, num);

                    if (entry != NULL) {
                        // 変換テーブルに該当
                        send_string(entry->output);

                        // *入力待ちモードの設定
                        if (entry->star1 != NULL) {
                            star_mode = true;
                            star_count = 0;
                            star_buffer = entry->output;
                            star1_next = entry->star1;
                            star2_next = entry->star2;
                        }
                    }
                    // 該当なしの場合は何も送信しない

                    niko_clear();
                }
                return false;
            }
            break;
    }

    // *入力待ちモード中に他のキーが押された場合
    if (star_mode && keycode != NK_STAR) {
        star_clear();
    }

    return true;
}

// M5キーリリース時の処理
void post_process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (keycode == NK_M5 && !record->event.pressed) {
        layer_off(FN);
    }
}
