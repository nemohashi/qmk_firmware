/* Copyright 2024 nemohashi
 *
 * 56式ポケベル入力 (56-style Pager Input)
 * 共通ロジック実装
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 */

#include "nikotouch.h"

// ============================================================
// ニコタッチ変換テーブル
// ============================================================
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
    {3, 6, "-", NULL, NULL},
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
    {5, 6, "[", NULL, NULL},
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
    {6, 6, "]", NULL, NULL},
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
    // 8 + X: や行（母音位置に合わせた配置）
    {8, 1, "ya", "lya", NULL},
    {8, 2, "lya", NULL, NULL},
    {8, 3, "yu", "lyu", NULL},
    {8, 4, "lyu", NULL, NULL},
    {8, 5, "yo", "lyo", NULL},
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
static bool niko_consonant_displayed = false; // 子音文字を表示したかどうか
static bool star_mode = false;               // *入力待ちモード
static uint8_t star_state = 0;               // 現在の*状態 (0=無変換, 1=*1回目, 2=*2回目)
static const char *star_output[3];           // 各状態の出力文字列 [0]=無変換, [1]=*1回目, [2]=*2回目
static uint8_t star_max_state = 0;           // 最大状態数（1 or 2 or 3）

// M5のLT機能用
static bool m5_pressed = false;              // M5が押されているか
static uint16_t m5_timer = 0;                // M5押下時刻
bool m5_is_hold = false;                     // M5がホールドとして扱われたか（外部参照用）

// 長押し入力用（1-5キー）
static uint8_t hold_key = 0xFF;              // 長押し中のキー (0xFF = なし)
static uint16_t hold_timer = 0;              // 長押し開始時刻
static bool hold_triggered = false;          // 長押しが発動したか

// ============================================================
// 確定前バッファ管理
// ============================================================
// バッファエントリ: 各キー入力を記録
// key_num: 0-9 = NK_0-NK_9, 10 = NK_STAR
#define PRECONFIRM_KEY_STAR 10
typedef struct {
    uint8_t key_num;  // 入力キー (0-9, 10=*)
} preconfirm_entry_t;

static preconfirm_entry_t preconfirm_buffer[PRECONFIRM_BUFFER_SIZE];
static uint8_t preconfirm_write_ptr = 0;    // 書き込み位置
static uint8_t preconfirm_cursor = 0;       // 現在のカーソル位置（Backspaceで戻った位置）
static bool preconfirm_space_flag = false;  // Spaceが押されたかどうか

// ============================================================
// 前方宣言
// ============================================================
static const nikotouch_entry_t* find_nikotouch_entry(uint8_t key1, uint8_t key2);
static void send_consonant(uint8_t niko_num);

// ============================================================
// 状態クリア関数
// ============================================================

void niko_clear(void) {
    niko_buffer = 0xFF;
    niko_buffer_time = 0;
    niko_consonant_displayed = false;
}

void star_clear(void) {
    star_mode = false;
    star_state = 0;
    star_output[0] = NULL;
    star_output[1] = NULL;
    star_output[2] = NULL;
    star_max_state = 0;
}

// 確定前バッファをクリア
void preconfirm_clear(void) {
    preconfirm_write_ptr = 0;
    preconfirm_cursor = 0;
    preconfirm_space_flag = false;
}

// 確定前バッファにキーを追加
static void preconfirm_add_key(uint8_t key_num) {
    // カーソルが書き込み位置より前にある場合、カーソル位置から書き込み
    if (preconfirm_cursor < preconfirm_write_ptr) {
        preconfirm_write_ptr = preconfirm_cursor;
    }
    
    // バッファが満杯の場合、古いエントリを捨てて詰める
    if (preconfirm_write_ptr >= PRECONFIRM_BUFFER_SIZE) {
        // 2キー分（1文字分）ずらす
        for (uint8_t i = 0; i < PRECONFIRM_BUFFER_SIZE - 2; i++) {
            preconfirm_buffer[i] = preconfirm_buffer[i + 2];
        }
        preconfirm_write_ptr = PRECONFIRM_BUFFER_SIZE - 2;
    }
    
    preconfirm_buffer[preconfirm_write_ptr].key_num = key_num;
    preconfirm_write_ptr++;
    preconfirm_cursor = preconfirm_write_ptr;
}

// 確定前バッファのカーソルを戻す（1文字分=2キー入力分、濁音は3キー分）
static void preconfirm_backtrack(uint8_t char_count) {
    for (uint8_t i = 0; i < char_count; i++) {
        if (preconfirm_cursor == 0) break;
        
        // 直前のキーが*かどうかチェック
        if (preconfirm_cursor >= 1 && 
            preconfirm_buffer[preconfirm_cursor - 1].key_num == PRECONFIRM_KEY_STAR) {
            // 濁音：3キー分戻る（数字、数字、*）
            if (preconfirm_cursor >= 3) {
                preconfirm_cursor -= 3;
            } else {
                preconfirm_cursor = 0;
            }
        } else {
            // 通常の文字：2キー分戻る
            if (preconfirm_cursor >= 2) {
                preconfirm_cursor -= 2;
            } else {
                preconfirm_cursor = 0;
            }
        }
    }
}

// カーソル位置から直前の2キーを取得（濁音変換用）
// 成功時はtrueを返し、key1, key2に値を設定
static bool preconfirm_get_last_pair(uint8_t *key1, uint8_t *key2) {
    // カーソル位置から2キー戻った場所を見る
    if (preconfirm_cursor < 2) {
        return false;
    }
    
    uint8_t idx = preconfirm_cursor - 2;
    
    // *キーが含まれている場合は対象外
    if (preconfirm_buffer[idx].key_num == PRECONFIRM_KEY_STAR ||
        preconfirm_buffer[idx + 1].key_num == PRECONFIRM_KEY_STAR) {
        return false;
    }
    
    *key1 = preconfirm_buffer[idx].key_num;
    *key2 = preconfirm_buffer[idx + 1].key_num;
    return true;
}

// カーソル位置から*を挿入（濁音変換成功時）
static void preconfirm_insert_star_at_cursor(void) {
    // カーソル位置に*を追加
    if (preconfirm_cursor >= PRECONFIRM_BUFFER_SIZE) {
        return;
    }
    
    // カーソル以降のデータを1つ後ろにずらす
    if (preconfirm_write_ptr < PRECONFIRM_BUFFER_SIZE) {
        for (int i = preconfirm_write_ptr; i > preconfirm_cursor; i--) {
            preconfirm_buffer[i] = preconfirm_buffer[i - 1];
        }
        preconfirm_buffer[preconfirm_cursor].key_num = PRECONFIRM_KEY_STAR;
        preconfirm_cursor++;
        preconfirm_write_ptr++;
    }
}

// カーソル位置から残りのシーケンスを再生（7+9コンボ用）
static void preconfirm_replay_remaining(void) {
    while (preconfirm_cursor < preconfirm_write_ptr) {
        uint8_t key_num = preconfirm_buffer[preconfirm_cursor].key_num;
        
        if (key_num == PRECONFIRM_KEY_STAR) {
            // *キーの処理：直前の文字に対して濁音変換
            if (star_mode) {
                // *入力待ちモード中：変換を実行
                uint8_t next_state = (star_state + 1) % star_max_state;
                // 1文字削除して次の文字を送信
                tap_code(KC_BSPC);
                send_string(star_output[next_state]);
                star_state = next_state;
            }
            preconfirm_cursor++;
        } else if (key_num <= 9) {
            // 数字キーの処理
            if (niko_buffer == 0xFF) {
                // 1回目のキー
                niko_buffer = key_num;
                send_consonant(key_num);
                niko_consonant_displayed = true;
                preconfirm_cursor++;
            } else {
                // 2回目のキー：変換実行
                if (niko_consonant_displayed) {
                    tap_code(KC_BSPC);
                }
                
                const nikotouch_entry_t *entry = find_nikotouch_entry(niko_buffer, key_num);
                if (entry != NULL && entry->output != NULL) {
                    send_string(entry->output);
                    
                    // *入力待ちモードの設定
                    if (entry->star1 != NULL) {
                        star_mode = true;
                        star_state = 0;
                        star_output[0] = entry->output;
                        star_output[1] = entry->star1;
                        star_output[2] = entry->star2;
                        star_max_state = (entry->star2 != NULL) ? 3 : 2;
                    } else {
                        star_clear();
                    }
                }
                niko_clear();
                preconfirm_cursor++;
            }
        } else {
            // 不明なキー
            preconfirm_cursor++;
        }
    }
}

// ============================================================
// 内部関数
// ============================================================

// ニコタッチ変換テーブルを検索
static const nikotouch_entry_t* find_nikotouch_entry(uint8_t key1, uint8_t key2) {
    for (uint8_t i = 0; i < NIKOTOUCH_TABLE_SIZE; i++) {
        if (nikotouch_table[i].key1 == key1 && nikotouch_table[i].key2 == key2) {
            return &nikotouch_table[i];
        }
    }
    return NULL;
}

// 子音文字を送信（Shift+アルファベット）
static void send_consonant(uint8_t niko_num) {
    uint16_t keycode = KC_NO;
    
    switch (niko_num) {
        case 1: keycode = KC_A; break;  // A
        case 2: keycode = KC_K; break;  // K
        case 3: keycode = KC_S; break;  // S
        case 4: keycode = KC_T; break;  // T
        case 5: keycode = KC_N; break;  // N
        case 6: keycode = KC_H; break;  // H
        case 7: keycode = KC_M; break;  // M
        case 8: keycode = KC_Y; break;  // Y
        case 9: keycode = KC_R; break;  // R
        case 0: keycode = KC_W; break;  // W
        default: return;
    }
    
    // Shift + アルファベットで大文字を送信
    register_code(KC_LSFT);
    tap_code(keycode);
    unregister_code(KC_LSFT);
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

// *キー処理（NK_STAR, CMB_56, M5タップで共通）
static void process_star_action(void) {
    if (!star_mode) {
        return;
    }
    // 次の状態に進む（ループ）
    star_state = (star_state + 1) % star_max_state;
    // 1文字削除して次の文字を送信
    tap_code(KC_BSPC);
    send_string(star_output[star_state]);
}

// 0キー処理（NK_0, CMB_45で共通）
static void process_zero_key(void) {
    // *入力待ちモード中に他のキーが押された場合
    if (star_mode) {
        star_clear();
    }

    if (niko_buffer == 0xFF) {
        // 1回目のキー入力：0キーと同じ
        niko_buffer = 0;
        niko_buffer_time = timer_read();
        send_consonant(0);
        niko_consonant_displayed = true;
    } else {
        // 2回目のキー入力：0キーと同じ
        if (niko_consonant_displayed) {
            tap_code(KC_BSPC);
        }
        
        const nikotouch_entry_t *entry = find_nikotouch_entry(niko_buffer, 0);

        if (entry != NULL && entry->output != NULL) {
            send_string(entry->output);

            if (entry->star1 != NULL) {
                star_mode = true;
                star_state = 0;
                star_output[0] = entry->output;
                star_output[1] = entry->star1;
                star_output[2] = entry->star2;
                
                if (entry->star2 != NULL) {
                    star_max_state = 3;
                } else {
                    star_max_state = 2;
                }
            }
        }

        niko_clear();
    }
}

// ============================================================
// 公開関数
// ============================================================

// タイムアウト処理（キーマップの matrix_scan_user から呼び出す）
void matrix_scan_nikotouch(void) {
    // ニコバッファのタイムアウトチェック
    if (niko_buffer != 0xFF) {
        if (timer_elapsed(niko_buffer_time) > NIKOTOUCH_TIMEOUT_MS) {
            // タイムアウト時、子音文字が表示されていたらBackspaceで削除
            if (niko_consonant_displayed) {
                tap_code(KC_BSPC);
            }
            niko_clear();
        }
    }

    // 1-5キーの長押し判定
    if (hold_key != 0xFF && !hold_triggered) {
        if (timer_elapsed(hold_timer) > NIKOTOUCH_HOLD_TERM) {
            // 長押し発動：対応する文字を出力
            hold_triggered = true;
            
            // *入力待ちモードをクリア
            if (star_mode) {
                star_clear();
            }
            
            // 長押し文字を出力（11, 22, 33, 44, 55相当）
            const nikotouch_entry_t *entry = find_nikotouch_entry(hold_key, hold_key);
            if (entry != NULL && entry->output != NULL) {
                send_string(entry->output);
                
                // 確定前バッファに記録（1キー目と2キー目）
                preconfirm_add_key(hold_key);
                preconfirm_add_key(hold_key);
                
                // *入力待ちモードの設定
                if (entry->star1 != NULL) {
                    star_mode = true;
                    star_state = 0;
                    star_output[0] = entry->output;
                    star_output[1] = entry->star1;
                    star_output[2] = entry->star2;
                    star_max_state = (entry->star2 != NULL) ? 3 : 2;
                }
            }
        }
    }

    // M5のホールド判定
    if (m5_pressed && !m5_is_hold) {
        if (timer_elapsed(m5_timer) > TAPPING_TERM_M5) {
            // タップ時間を超えた = ホールドとして扱う
            m5_is_hold = true;
            layer_on(NT_FN);
        }
    }
}

// キー処理（キーマップの process_record_user から呼び出す）
bool process_record_nikotouch(uint16_t keycode, keyrecord_t *record) {
    // カスタムキー処理（press/releaseどちらも処理が必要なもの）
    switch (keycode) {
        // M4: MO(FN) - 押している間だけFNレイヤー
        case NK_M4:
            if (record->event.pressed) {
                layer_on(NT_FN);
            } else {
                layer_off(NT_FN);
            }
            return false;

        // M5: LT(FN, NK_STAR) - タップで*、ホールドでFN
        case NK_M5:
            if (record->event.pressed) {
                // 押された
                m5_pressed = true;
                m5_timer = timer_read();
                m5_is_hold = false;
            } else {
                // 離された
                if (!m5_is_hold) {
                    // タップとして扱う = *キーの処理
                    process_star_action();
                } else {
                    // ホールドとして扱われた = FNレイヤーをオフ
                    layer_off(NT_FN);
                }
                m5_pressed = false;
            }
            return false;
    }

    // キーリリース時の処理
    if (!record->event.pressed) {
        // NK_BS（Backspace）のリリース処理
        if (keycode == NK_BS) {
            unregister_code(KC_BSPC);
            return false;
        }
        
        // 1-5キーのリリース処理（長押し対応）
        if (keycode >= NK_1 && keycode <= NK_5) {
            uint8_t num = keycode_to_niko_num(keycode);
            if (hold_key == num) {
                if (!hold_triggered) {
                    // 長押し未発動：従来の1キー目処理を実行
                    // *入力待ちモード中に*以外が押された場合
                    if (star_mode) {
                        star_clear();
                    }
                    
                    // Spaceフラグが立っている状態で数字キーが押されたらバッファをクリア
                    if (preconfirm_space_flag) {
                        preconfirm_clear();
                    }
                    
                    // 1回目のキー入力：子音文字を表示
                    niko_buffer = num;
                    niko_buffer_time = timer_read();
                    send_consonant(num);
                    niko_consonant_displayed = true;
                    // 確定前バッファに記録
                    preconfirm_add_key(num);
                }
                // 長押し状態をクリア
                hold_key = 0xFF;
                hold_triggered = false;
            }
            return false;
        }
        
        return true;
    }

    // 以降はキープレス時のみの処理
    switch (keycode) {
        // M1: BASEレイヤー + 英数（日本語OFF）
        case NK_M1:
            layer_move(NT_BASE);
            tap_code(KC_LNG2);  // 英数キー
            preconfirm_clear();  // レイヤー切替時は確定前バッファをクリア
            return false;

        // M2: NIKOTOUCHレイヤー + かな（日本語ON）
        case NK_M2:
            layer_move(NT_NIKOTOUCH);
            tap_code(KC_LNG1);  // かなキー
            niko_clear();
            star_clear();
            preconfirm_clear();  // レイヤー切替時は確定前バッファをクリア
            return false;

        // M3: ARTSEY_ALPHAレイヤーに切り替え
        case NK_M3:
            layer_move(NT_ARTSEY_ALPHA);
            niko_clear();
            star_clear();
            preconfirm_clear();
            return false;

        // Backspace（ニコタッチバッファクリア機能付き）
        case NK_BS:
            if (niko_buffer != 0xFF) {
                // ニコバッファがある場合：子音文字が表示されていたら削除してからクリア
                if (niko_consonant_displayed) {
                    tap_code(KC_BSPC);
                }
                niko_clear();
                return false;
            } else if (star_mode) {
                // *バッファがある場合はクリア + Backspace送信（1回のみ）
                star_clear();
                tap_code(KC_BSPC);
                // 確定前バッファのカーソルを1文字戻す
                preconfirm_backtrack(1);
                return false;
            } else {
                // 通常のBackspace（キーリピート対応）
                register_code(KC_BSPC);
                // 確定前バッファのカーソルを1文字戻す
                preconfirm_backtrack(1);
                return false;
            }

        // Shift + Enter（改行）
        case NK_SENT:
            register_code(KC_LSFT);
            tap_code(KC_ENT);
            unregister_code(KC_LSFT);
            return false;

        // * キー（濁音・半濁音変換）
        case NK_STAR:
            if (star_mode) {
                // 通常の*入力待ちモード
                process_star_action();
                // 確定前バッファに*を記録
                preconfirm_add_key(PRECONFIRM_KEY_STAR);
            } else {
                // *入力待ちモードでない場合、確定前バッファから濁音変換を試みる
                uint8_t key1, key2;
                if (preconfirm_get_last_pair(&key1, &key2)) {
                    // 確定前バッファから直前のキーペアを取得
                    const nikotouch_entry_t *entry = find_nikotouch_entry(key1, key2);
                    if (entry != NULL && entry->star1 != NULL) {
                        // 濁音変換可能：1文字削除して変換後の文字を送信
                        tap_code(KC_BSPC);
                        send_string(entry->star1);
                        // 確定前バッファに*を挿入
                        preconfirm_insert_star_at_cursor();
                    }
                }
            }
            return false;

        // 5+6 コンボ（濁点変換 = *キー相当）
        case CMB_56:
            if (star_mode) {
                process_star_action();
                preconfirm_add_key(PRECONFIRM_KEY_STAR);
            } else {
                uint8_t key1, key2;
                if (preconfirm_get_last_pair(&key1, &key2)) {
                    const nikotouch_entry_t *entry = find_nikotouch_entry(key1, key2);
                    if (entry != NULL && entry->star1 != NULL) {
                        tap_code(KC_BSPC);
                        send_string(entry->star1);
                        preconfirm_insert_star_at_cursor();
                    }
                }
            }
            return false;

        // 6+7 コンボ（0キー相当）
        case CMB_67:
            process_zero_key();
            // 確定前バッファに0を記録
            preconfirm_add_key(0);
            return false;

        // 7+9 コンボ（確定前バッファから残りを再入力）
        case CMB_79:
            niko_clear();  // 再生前に状態をリセット
            star_clear();
            preconfirm_replay_remaining();
            return false;

        // 1+BS コンボ（2文字削除）
        case CMB_1BS:
            for (uint8_t i = 0; i < 2; i++) tap_code(KC_BSPC);
            preconfirm_backtrack(2);
            return false;

        // 2+BS コンボ（3文字削除）
        case CMB_2BS:
            for (uint8_t i = 0; i < 3; i++) tap_code(KC_BSPC);
            preconfirm_backtrack(3);
            return false;

        // 3+BS コンボ（4文字削除）
        case CMB_3BS:
            for (uint8_t i = 0; i < 4; i++) tap_code(KC_BSPC);
            preconfirm_backtrack(4);
            return false;

        // ニコタッチ数字キー (NK_0 〜 NK_9)
        default:
            if (is_nikotouch_key(keycode)) {
                uint8_t num = keycode_to_niko_num(keycode);

                // Spaceフラグが立っている状態で数字キーが押されたらバッファをクリア
                if (preconfirm_space_flag) {
                    preconfirm_clear();
                }

                // *入力待ちモード中に*以外が押された場合
                if (star_mode) {
                    star_clear();
                }

                // ロールオーバー対応: 1-5キーが押されている間に別のキーが押された場合
                if (hold_key != 0xFF && !hold_triggered) {
                    // hold_keyを1キー目、numを2キー目として変換
                    const nikotouch_entry_t *entry = find_nikotouch_entry(hold_key, num);
                    if (entry != NULL && entry->output != NULL) {
                        send_string(entry->output);
                        
                        // 確定前バッファに記録（1キー目と2キー目）
                        preconfirm_add_key(hold_key);
                        preconfirm_add_key(num);
                        
                        // *入力待ちモードの設定
                        if (entry->star1 != NULL) {
                            star_mode = true;
                            star_state = 0;
                            star_output[0] = entry->output;
                            star_output[1] = entry->star1;
                            star_output[2] = entry->star2;
                            star_max_state = (entry->star2 != NULL) ? 3 : 2;
                        }
                    }
                    // hold_keyをクリア
                    hold_key = 0xFF;
                    hold_triggered = false;
                    niko_clear();
                    return false;
                }

                if (niko_buffer == 0xFF) {
                    // 1回目のキー入力
                    if (num >= 1 && num <= 5) {
                        // 1-5キー：長押しタイマー開始（子音表示しない）
                        hold_key = num;
                        hold_timer = timer_read();
                        hold_triggered = false;
                    } else {
                        // 0, 6-9キー：従来通り子音文字を表示
                        niko_buffer = num;
                        niko_buffer_time = timer_read();
                        send_consonant(num);
                        niko_consonant_displayed = true;
                        // 確定前バッファに記録
                        preconfirm_add_key(num);
                    }
                } else {
                    // 2回目のキー入力：子音文字を削除してから変換後の文字列を送信
                    if (niko_consonant_displayed) {
                        tap_code(KC_BSPC);
                    }
                    
                    const nikotouch_entry_t *entry = find_nikotouch_entry(niko_buffer, num);

                    if (entry != NULL && entry->output != NULL) {
                        // 変換テーブルに該当
                        send_string(entry->output);

                        // *入力待ちモードの設定
                        if (entry->star1 != NULL) {
                            star_mode = true;
                            star_state = 0;
                            star_output[0] = entry->output;
                            star_output[1] = entry->star1;
                            star_output[2] = entry->star2;
                            
                            // 最大状態数を計算
                            if (entry->star2 != NULL) {
                                star_max_state = 3;  // 無変換、*1回目、*2回目
                            } else {
                                star_max_state = 2;  // 無変換、*1回目
                            }
                        }
                    }
                    // 該当なしの場合は何も送信しない

                    // 確定前バッファに記録
                    preconfirm_add_key(num);
                    niko_clear();
                }
                return false;
            }
            break;
    }

    // Enter キー：確定前バッファをクリア
    if (keycode == KC_ENT || keycode == KC_PENT) {
        preconfirm_clear();
    }

    // Space キー：Spaceフラグを立てる（次の数字キーでクリア）
    if (keycode == KC_SPC) {
        preconfirm_space_flag = true;
    }

    // *入力待ちモード中に他のキーが押された場合（M5とNK_STARは除外）
    // ただし、M5がホールド判定された場合（FNレイヤーが有効）は除外
    if (star_mode && keycode != NK_STAR && keycode != NK_M5 && !m5_is_hold) {
        star_clear();
    }

    return true;
}
