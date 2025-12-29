/* Copyright 2024 nemohashi
 *
 * Artsey入力方式 (Artsey Input Method)
 * ロジック実装
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 */

#include "artsey.h"

// ============================================================
// 状態管理
// ============================================================
static bool ar_1_pressed = false;     // 1キーが押されているか（KAKOレイヤー）
static bool ar_4_pressed = false;     // 4キーが押されているか（SYMBOL1レイヤー）
static bool ar_minus_pressed = false; // -キーが押されているか（SYMBOL2レイヤー）
static bool ar_ent_pressed = false;   // Entキーが押されているか（SYMBOL3レイヤー）
static bool ar_dot_pressed = false;   // .キーが押されているか（NIKOSHIFTレイヤー）

static uint16_t ar_1_timer = 0;       // 1キーのタイマー
static uint16_t ar_4_timer = 0;       // 4キーのタイマー
static uint16_t ar_minus_timer = 0;   // -キーのタイマー
static uint16_t ar_ent_timer = 0;     // Entキーのタイマー
static uint16_t ar_dot_timer = 0;     // .キーのタイマー

static bool ar_1_is_hold = false;     // 1キーがホールドとして扱われたか
static bool ar_4_is_hold = false;     // 4キーがホールドとして扱われたか
static bool ar_minus_is_hold = false; // -キーがホールドとして扱われたか
static bool ar_ent_is_hold = false;   // Entキーがホールドとして扱われたか
static bool ar_dot_is_hold = false;   // .キーがホールドとして扱われたか

#define ARTSEY_TAPPING_TERM 200       // タップ/ホールド判定時間（ms）

// Shift状態管理
static bool artsey_once_shift = false;    // Once Shift（次の1文字のみ大文字）
static bool artsey_toggle_shift = false;  // Toggle Shift（ON/OFFトグル）

// ============================================================
// 状態クリア関数
// ============================================================
void artsey_clear(void) {
    ar_1_pressed = false;
    ar_4_pressed = false;
    ar_minus_pressed = false;
    ar_ent_pressed = false;
    ar_dot_pressed = false;
    ar_1_is_hold = false;
    ar_4_is_hold = false;
    ar_minus_is_hold = false;
    ar_ent_is_hold = false;
    ar_dot_is_hold = false;
    artsey_once_shift = false;
    // Toggle Shiftはクリアしない（意図的に維持）
}

// ============================================================
// 内部関数
// ============================================================

// Artseyレイヤーがアクティブかどうか
static bool is_artsey_layer_active(void) {
    return layer_state_is(NT_ARTSEY_ALPHA) || 
           layer_state_is(NT_ARTSEY_KAKO) || 
           layer_state_is(NT_ARTSEY_SYMBOL1) ||
           layer_state_is(NT_ARTSEY_SYMBOL2) ||
           layer_state_is(NT_ARTSEY_SYMBOL3);
}

// アルファベットキーをShift状態を考慮して送信
static void send_alpha_key(uint16_t keycode) {
    // Toggle ShiftがONの場合、またはOnce ShiftがONの場合は大文字
    if (artsey_toggle_shift || artsey_once_shift) {
        register_code(KC_LSFT);
        tap_code(keycode);
        unregister_code(KC_LSFT);
        
        // Once Shiftは1文字送信後に解除
        if (artsey_once_shift) {
            artsey_once_shift = false;
        }
    } else {
        tap_code(keycode);
    }
}

// ============================================================
// 公開関数
// ============================================================

void matrix_scan_artsey(void) {
    // AR_1のホールド判定（KAKO）
    if (ar_1_pressed && !ar_1_is_hold) {
        if (timer_elapsed(ar_1_timer) > ARTSEY_TAPPING_TERM) {
            ar_1_is_hold = true;
            layer_on(NT_ARTSEY_KAKO);
        }
    }
    
    // AR_4のホールド判定（SYMBOL1）
    if (ar_4_pressed && !ar_4_is_hold) {
        if (timer_elapsed(ar_4_timer) > ARTSEY_TAPPING_TERM) {
            ar_4_is_hold = true;
            layer_on(NT_ARTSEY_SYMBOL1);
        }
    }
    
    // AR_MINUSのホールド判定（SYMBOL2）
    if (ar_minus_pressed && !ar_minus_is_hold) {
        if (timer_elapsed(ar_minus_timer) > ARTSEY_TAPPING_TERM) {
            ar_minus_is_hold = true;
            layer_on(NT_ARTSEY_SYMBOL2);
        }
    }
    
    // AR_ENTのホールド判定（SYMBOL3）
    if (ar_ent_pressed && !ar_ent_is_hold) {
        if (timer_elapsed(ar_ent_timer) > ARTSEY_TAPPING_TERM) {
            ar_ent_is_hold = true;
            layer_on(NT_ARTSEY_SYMBOL3);
        }
    }
    
    // AR_DOTのホールド判定（NIKOSHIFT）
    if (ar_dot_pressed && !ar_dot_is_hold) {
        if (timer_elapsed(ar_dot_timer) > ARTSEY_TAPPING_TERM) {
            ar_dot_is_hold = true;
            layer_on(NT_NIKOSHIFT);
        }
    }
}

// キー処理
bool process_record_artsey(uint16_t keycode, keyrecord_t *record) {
    // Artseyレイヤーでない場合は処理しない
    if (!is_artsey_layer_active()) {
        return true;
    }
    
    // レイヤーホールドキーの処理
    switch (keycode) {
        case AR_1:
            if (record->event.pressed) {
                ar_1_pressed = true;
                ar_1_timer = timer_read();
                ar_1_is_hold = false;
            } else {
                // リリース時
                if (!ar_1_is_hold) {
                    // タップとして扱う = "a"を出力
                    send_alpha_key(KC_A);
                } else {
                    // ホールドとして扱われた = KAKOレイヤーをオフ
                    layer_off(NT_ARTSEY_KAKO);
                }
                ar_1_pressed = false;
            }
            return false;
            
        case AR_4:
            if (record->event.pressed) {
                // .キーシフト中は左矢印
                if (ar_dot_is_hold) {
                    tap_code(KC_LEFT);
                    ar_4_pressed = false;  // フラグをクリアしておく
                    ar_4_is_hold = true;   // ホールド扱いにしてリリース時の出力をスキップ
                    return false;
                }
                ar_4_pressed = true;
                ar_4_timer = timer_read();
                ar_4_is_hold = false;
            } else {
                // リリース時
                if (!ar_4_is_hold) {
                    // タップとして扱う = "e"を出力
                    send_alpha_key(KC_E);
                } else {
                    // ホールドとして扱われた = SYMBOL1レイヤーをオフ
                    layer_off(NT_ARTSEY_SYMBOL1);
                }
                ar_4_pressed = false;
                ar_4_is_hold = false;  // 次回のためにリセット
            }
            return false;
        
        case AR_MINUS:
            if (record->event.pressed) {
                ar_minus_pressed = true;
                ar_minus_timer = timer_read();
                ar_minus_is_hold = false;
            } else {
                // リリース時
                if (!ar_minus_is_hold) {
                    // タップとして扱う = "s"を出力
                    send_alpha_key(KC_S);
                } else {
                    // ホールドとして扱われた = SYMBOL2レイヤーをオフ
                    layer_off(NT_ARTSEY_SYMBOL2);
                }
                ar_minus_pressed = false;
            }
            return false;
        
        case AR_ENT:
            if (record->event.pressed) {
                // .キーシフト中は右矢印
                if (ar_dot_is_hold) {
                    tap_code(KC_RIGHT);
                    ar_ent_pressed = false;  // フラグをクリアしておく
                    ar_ent_is_hold = true;   // ホールド扱いにしてリリース時の出力をスキップ
                    return false;
                }
                ar_ent_pressed = true;
                ar_ent_timer = timer_read();
                ar_ent_is_hold = false;
            } else {
                // リリース時
                if (!ar_ent_is_hold) {
                    // タップとして扱う = "o"を出力
                    send_alpha_key(KC_O);
                } else {
                    // ホールドとして扱われた = SYMBOL3レイヤーをオフ
                    layer_off(NT_ARTSEY_SYMBOL3);
                }
                ar_ent_pressed = false;
                ar_ent_is_hold = false;  // 次回のためにリセット
            }
            return false;
            
        case AR_DOT:
            if (record->event.pressed) {
                ar_dot_pressed = true;
                ar_dot_timer = timer_read();
                ar_dot_is_hold = false;
            } else {
                // リリース時
                if (!ar_dot_is_hold) {
                    // タップとして扱う = N/A（何も送信しない）
                    // tap_code(KC_SPC); // 削除
                } else {
                    // ホールドとして扱われた = NIKOSHIFTレイヤーをオフ
                    layer_off(NT_NIKOSHIFT);
                }
                ar_dot_pressed = false;
            }
            return false;
            
        case AR_ENT2:
            // Enterをリピート対応で送信
            if (record->event.pressed) {
                register_code(KC_ENT);
            } else {
                unregister_code(KC_ENT);
            }
            return false;
            
        case AR_7:
            // Backspaceをリピート対応で送信
            if (record->event.pressed) {
                register_code(KC_BSPC);
            } else {
                unregister_code(KC_BSPC);
            }
            return false;
    }
    
    // キープレス時のみ処理
    if (!record->event.pressed) {
        return true;
    }
    
    switch (keycode) {
        // ============================================================
        // ARTSEY_ALPHAレイヤー 単キー
        // ============================================================
        case AR_2:
            send_alpha_key(KC_R);
            return false;
        case AR_3:
            send_alpha_key(KC_T);
            return false;
        case AR_5:
            if (record->event.pressed) {
                // .キーシフト中は下矢印
                if (ar_dot_is_hold) {
                    tap_code(KC_DOWN);
                } else {
                    send_alpha_key(KC_Y);
                }
            }
            return false;
        case AR_6:
            if (record->event.pressed) {
                // .キーシフト中は上矢印
                if (ar_dot_is_hold) {
                    tap_code(KC_UP);
                } else {
                    send_alpha_key(KC_I);
                }
            }
            return false;
        case AR_SPC:
            tap_code(KC_SPC);
            return false;
            
        // N/Aキー（何も送信しない）
        case AR_8:
        case AR_9:
        case AR_STAR:
        case AR_0:
        case AR_HASH:
            return false;
            
        // ============================================================
        // ARTSEY_ALPHAレイヤー 2キーコンボ
        // ============================================================
        case CMB_AR_12:  // 1+2 = f
            send_alpha_key(KC_F);
            return false;
        case CMB_AR_23:  // 2+3 = g
            send_alpha_key(KC_G);
            return false;
        case CMB_AR_3M:  // 3+- = j
            send_alpha_key(KC_J);
            return false;
        case CMB_AR_45:  // 4+5 = c
            send_alpha_key(KC_C);
            return false;
        case CMB_AR_56:  // 5+6 = u
            send_alpha_key(KC_U);
            return false;
        case CMB_AR_6E:  // 6+Ent = n
            send_alpha_key(KC_N);
            return false;
        case CMB_AR_14:  // 1+4 = Enter
            tap_code(KC_ENT);
            return false;
        case CMB_AR_25:  // 2+5 = '
            tap_code(KC_QUOT);
            return false;
        case CMB_AR_36:  // 3+6 = !
            register_code(KC_LSFT);
            tap_code(KC_1);
            unregister_code(KC_LSFT);
            return false;
        case CMB_AR_ME:  // -+Ent = ?
            register_code(KC_LSFT);
            tap_code(KC_SLSH);
            unregister_code(KC_LSFT);
            return false;
        case CMB_AR_1M:  // 1+- = w
            send_alpha_key(KC_W);
            return false;
        case CMB_AR_4E:  // 4+Ent = b
            send_alpha_key(KC_B);
            return false;
        case CMB_AR_2M:  // 2+- = v
            send_alpha_key(KC_V);
            return false;
        case CMB_AR_46:  // 4+6 = h
            send_alpha_key(KC_H);
            return false;
        case CMB_AR_5E:  // 5+Ent = k
            send_alpha_key(KC_K);
            return false;
        case CMB_AR_15:  // 1+5 = ,
            tap_code(KC_COMM);
            return false;
        case CMB_AR_26:  // 2+6 = Delete
            tap_code(KC_DEL);
            return false;
        case CMB_AR_16:  // 1+6 = .
            tap_code(KC_DOT);
            return false;
        case CMB_AR_1E:  // 1+Ent = /
            tap_code(KC_SLSH);
            return false;
        case CMB_AR_24:  // 2+4 = Backspace（新規）
            tap_code(KC_BSPC);
            return false;
            
        // ============================================================
        // ARTSEY_ALPHAレイヤー 3キーコンボ
        // ============================================================
        case CMB_AR_123:  // 1+2+3 = d
            send_alpha_key(KC_D);
            return false;
        case CMB_AR_56E:  // 5+6+Ent = m
            send_alpha_key(KC_M);
            return false;
        case CMB_AR_13M:  // 1+3+- = q
            send_alpha_key(KC_Q);
            return false;
        case CMB_AR_46E:  // 4+6+Ent = p
            send_alpha_key(KC_P);
            return false;
        case CMB_AR_23M:  // 2+3+- = x
            send_alpha_key(KC_X);
            return false;
        case CMB_AR_456:  // 4+5+6 = l
            send_alpha_key(KC_L);
            return false;
            
        // ============================================================
        // ARTSEY_ALPHAレイヤー 4キーコンボ
        // ============================================================
        case CMB_AR_456E:  // 4+5+6+Ent = Space
            tap_code(KC_SPC);
            return false;
        case CMB_AR_123M:  // 1+2+3+- = z
            send_alpha_key(KC_Z);
            return false;
        case CMB_AR_12E:  // 1+2+Ent = Esc（新規）
            tap_code(KC_ESC);
            return false;
        case CMB_AR_123E:  // 1+2+3+Ent = Tab（新規）
            tap_code(KC_TAB);
            return false;
        case CMB_AR_423M:  // 4+2+3+- = Once Shift（新規）
            artsey_once_shift = true;
            return false;
        case CMB_AR_156E:  // 1+5+6+Ent = Toggle Shift（新規）
            artsey_toggle_shift = !artsey_toggle_shift;
            return false;
            
        // ============================================================
        // ARTSEY_KAKOレイヤー 単キー
        // ============================================================
        case AR_K_1:  // 1キー = N/A（何も送信しない）
            return false;
        case AR_K_2:  // 2キー = (
            register_code(KC_LSFT);
            tap_code(KC_9);
            unregister_code(KC_LSFT);
            return false;
        case AR_K_3:  // 3キー = [
            tap_code(KC_LBRC);
            return false;
        case AR_K_MINUS:  // -キー = {
            register_code(KC_LSFT);
            tap_code(KC_LBRC);
            unregister_code(KC_LSFT);
            return false;
        case AR_K_4:  // 4キー = N/A（何も送信しない）
            return false;
        case AR_K_5:  // 5キー = )
            register_code(KC_LSFT);
            tap_code(KC_0);
            unregister_code(KC_LSFT);
            return false;
        case AR_K_6:  // 6キー = ]
            tap_code(KC_RBRC);
            return false;
        case AR_K_ENT:  // Entキー = }
            register_code(KC_LSFT);
            tap_code(KC_RBRC);
            unregister_code(KC_LSFT);
            return false;
        case AR_K_DOT:  // .キー = N/A（何も送信しない）
            return false;
            
        // ============================================================
        // ARTSEY_SYMBOL1レイヤー 単キー
        // SYMBOL1: 1=#, 2=`, 3=;, -=backslash, 4=N/A, 5=@, 6=-, Ent==
        // ============================================================
        case AR_S1_1:  // 1キー = #
            register_code(KC_LSFT);
            tap_code(KC_3);
            unregister_code(KC_LSFT);
            return false;
        case AR_S1_2:  // 2キー = `
            tap_code(KC_GRV);
            return false;
        case AR_S1_3:  // 3キー = ;
            tap_code(KC_SCLN);
            return false;
        case AR_S1_MINUS:  // -キー = \ (バックスラッシュ)
            tap_code(KC_BSLS);
            return false;
        case AR_S1_4:  // 4キー = N/A（何も送信しない）
            return false;
        case AR_S1_5:  // 5キー = @
            register_code(KC_LSFT);
            tap_code(KC_2);
            unregister_code(KC_LSFT);
            return false;
        case AR_S1_6:  // 6キー = -
            tap_code(KC_MINS);
            return false;
        case AR_S1_ENT:  // Entキー = =
            tap_code(KC_EQL);
            return false;
        case AR_S1_DOT:  // .キー = N/A（何も送信しない）
            return false;
            
        // ============================================================
        // ARTSEY_SYMBOL2レイヤー 単キー
        // SYMBOL2: 1=$, 2=%, 3=^, -=N/A, 4=&, 5=*, 6=_, Ent=N/A
        // ============================================================
        case AR_S2_1:  // 1キー = $
            register_code(KC_LSFT);
            tap_code(KC_4);
            unregister_code(KC_LSFT);
            return false;
        case AR_S2_2:  // 2キー = %
            register_code(KC_LSFT);
            tap_code(KC_5);
            unregister_code(KC_LSFT);
            return false;
        case AR_S2_3:  // 3キー = ^
            register_code(KC_LSFT);
            tap_code(KC_6);
            unregister_code(KC_LSFT);
            return false;
        case AR_S2_MINUS:  // -キー = N/A（何も送信しない）
            return false;
        case AR_S2_4:  // 4キー = &
            register_code(KC_LSFT);
            tap_code(KC_7);
            unregister_code(KC_LSFT);
            return false;
        case AR_S2_5:  // 5キー = *
            register_code(KC_LSFT);
            tap_code(KC_8);
            unregister_code(KC_LSFT);
            return false;
        case AR_S2_6:  // 6キー = _
            register_code(KC_LSFT);
            tap_code(KC_MINS);
            unregister_code(KC_LSFT);
            return false;
        case AR_S2_ENT:  // Entキー = N/A（何も送信しない）
            return false;
        case AR_S2_DOT:  // .キー = N/A（何も送信しない）
            return false;
            
        // ============================================================
        // ARTSEY_SYMBOL3レイヤー 単キー
        // SYMBOL3: 1=+, 2=|, 3=:, -=N/A, 4=<, 5=>, 6=~, Ent=N/A
        // ============================================================
        case AR_S3_1:  // 1キー = +
            register_code(KC_LSFT);
            tap_code(KC_EQL);
            unregister_code(KC_LSFT);
            return false;
        case AR_S3_2:  // 2キー = |
            register_code(KC_LSFT);
            tap_code(KC_BSLS);
            unregister_code(KC_LSFT);
            return false;
        case AR_S3_3:  // 3キー = :
            register_code(KC_LSFT);
            tap_code(KC_SCLN);
            unregister_code(KC_LSFT);
            return false;
        case AR_S3_MINUS:  // -キー = N/A（何も送信しない）
            return false;
        case AR_S3_4:  // 4キー = <
            register_code(KC_LSFT);
            tap_code(KC_COMM);
            unregister_code(KC_LSFT);
            return false;
        case AR_S3_5:  // 5キー = >
            register_code(KC_LSFT);
            tap_code(KC_DOT);
            unregister_code(KC_LSFT);
            return false;
        case AR_S3_6:  // 6キー = ~
            register_code(KC_LSFT);
            tap_code(KC_GRV);
            unregister_code(KC_LSFT);
            return false;
        case AR_S3_ENT:  // Entキー = N/A（何も送信しない）
            return false;
        case AR_S3_DOT:  // .キー = N/A（何も送信しない）
            return false;
    }
    
    return true;
}
