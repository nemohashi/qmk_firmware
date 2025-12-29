/* Copyright 2024 nemohashi
 *
 * Artsey入力方式 (Artsey Input Method)
 * ヘッダーファイル
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include QMK_KEYBOARD_H
#include "nikotouch.h"

// ============================================================
// 設定値
// ============================================================
#ifndef ARTSEY_COMBO_TERM
#define ARTSEY_COMBO_TERM COMBO_TERM_56  // コンボ検出時間（56式と同じ）
#endif

// ============================================================
// Artseyキーコード
// ============================================================
enum artsey_keycodes {
    // 基本キー（ARTSEY_ALPHAレイヤー）
    AR_1 = NIKOTOUCH_SAFE_RANGE,  // 1キー（押している間KAKO）
    AR_2,                          // 2キー
    AR_3,                          // 3キー
    AR_MINUS,                      // -キー
    AR_4,                          // 4キー（押している間SYMBOL）
    AR_5,                          // 5キー
    AR_6,                          // 6キー
    AR_ENT,                        // Entキー
    AR_7,                          // 7キー（N/A）
    AR_8,                          // 8キー（N/A）
    AR_9,                          // 9キー（N/A）
    AR_STAR,                       // *キー（N/A）
    AR_0,                          // 0キー（N/A）
    AR_HASH,                       // #キー（N/A）
    AR_ENT2,                       // Ent2キー（Backspace）
    AR_SPC,                        // Spaceキー
    AR_DOT,                        // .キー（押している間NIKOSHIFT）
    
    // ARTSEY_KAKOレイヤーキー
    AR_K_1,                        // KAKOの1キー = N/A
    AR_K_2,                        // KAKOの2キー = (
    AR_K_3,                        // KAKOの3キー = [
    AR_K_MINUS,                    // KAKOの-キー = {
    AR_K_4,                        // KAKOの4キー = N/A
    AR_K_5,                        // KAKOの5キー = )
    AR_K_6,                        // KAKOの6キー = ]
    AR_K_ENT,                      // KAKOのEntキー = }
    AR_K_DOT,                      // KAKOの.キー = N/A
    
    // ARTSEY_SYMBOL1レイヤーキー
    AR_S1_1,                       // SYMBOL1の1キー = #
    AR_S1_2,                       // SYMBOL1の2キー = `
    AR_S1_3,                       // SYMBOL1の3キー = ;
    AR_S1_MINUS,                   // SYMBOL1の-キー = backslash
    AR_S1_4,                       // SYMBOL1の4キー = N/A
    AR_S1_5,                       // SYMBOL1の5キー = @
    AR_S1_6,                       // SYMBOL1の6キー = -
    AR_S1_ENT,                     // SYMBOL1のEntキー = =
    AR_S1_DOT,                     // SYMBOL1の.キー = N/A
    
    // ARTSEY_SYMBOL2レイヤーキー
    AR_S2_1,                       // SYMBOL2の1キー = $
    AR_S2_2,                       // SYMBOL2の2キー = %
    AR_S2_3,                       // SYMBOL2の3キー = ^
    AR_S2_MINUS,                   // SYMBOL2の-キー = N/A
    AR_S2_4,                       // SYMBOL2の4キー = &
    AR_S2_5,                       // SYMBOL2の5キー = *
    AR_S2_6,                       // SYMBOL2の6キー = _
    AR_S2_ENT,                     // SYMBOL2のEntキー = N/A
    AR_S2_DOT,                     // SYMBOL2の.キー = N/A
    
    // ARTSEY_SYMBOL3レイヤーキー
    AR_S3_1,                       // SYMBOL3の1キー = +
    AR_S3_2,                       // SYMBOL3の2キー = |
    AR_S3_3,                       // SYMBOL3の3キー = :
    AR_S3_MINUS,                   // SYMBOL3の-キー = N/A
    AR_S3_4,                       // SYMBOL3の4キー = <
    AR_S3_5,                       // SYMBOL3の5キー = >
    AR_S3_6,                       // SYMBOL3の6キー = ~
    AR_S3_ENT,                     // SYMBOL3のEntキー = N/A
    AR_S3_DOT,                     // SYMBOL3の.キー = N/A
    
    // Artseyコンボキーコード
    // ARTSEY_ALPHAレイヤー 2キーコンボ
    CMB_AR_12,    // 1+2 = f
    CMB_AR_23,    // 2+3 = g
    CMB_AR_3M,    // 3+- = j
    CMB_AR_45,    // 4+5 = c
    CMB_AR_56,    // 5+6 = u
    CMB_AR_6E,    // 6+Ent = n
    CMB_AR_14,    // 1+4 = Enter
    CMB_AR_25,    // 2+5 = '
    CMB_AR_36,    // 3+6 = !
    CMB_AR_ME,    // -+Ent = ?
    CMB_AR_1M,    // 1+- = w
    CMB_AR_4E,    // 4+Ent = b
    CMB_AR_2M,    // 2+- = v
    CMB_AR_46,    // 4+6 = h
    CMB_AR_5E,    // 5+Ent = k
    CMB_AR_15,    // 1+5 = ,
    CMB_AR_26,    // 2+6 = Delete
    CMB_AR_16,    // 1+6 = .
    CMB_AR_1E,    // 1+Ent = /
    CMB_AR_24,    // 2+4 = Backspace（新規）
    
    // ARTSEY_ALPHAレイヤー 3キーコンボ
    CMB_AR_123,   // 1+2+3 = d
    CMB_AR_56E,   // 5+6+Ent = m
    CMB_AR_13M,   // 1+3+- = q
    CMB_AR_46E,   // 4+6+Ent = p
    CMB_AR_23M,   // 2+3+- = x
    CMB_AR_456,   // 4+5+6 = l
    CMB_AR_12E,   // 1+2+Ent = Esc（新規）
    
    // ARTSEY_ALPHAレイヤー 4キーコンボ
    CMB_AR_456E,  // 4+5+6+Ent = Space
    CMB_AR_123M,  // 1+2+3+- = z
    CMB_AR_123E,  // 1+2+3+Ent = Tab（新規）
    CMB_AR_423M,  // 4+2+3+- = Once Shift（新規）
    CMB_AR_156E,  // 1+5+6+Ent = Toggle Shift（新規）
    
    // ARTSEY_KAKOレイヤー（コンボなし - 単キーのみ）
    // コンボは削除（新仕様ではSYMBOL1/2/3に分割、コンボなし）
    
    ARTSEY_SAFE_RANGE,  // キーマップ側で追加のキーコードを定義する場合に使用
};

// ============================================================
// 公開関数
// ============================================================

// キーマップから呼び出す関数
bool process_record_artsey(uint16_t keycode, keyrecord_t *record);
void matrix_scan_artsey(void);

// 状態クリア関数
void artsey_clear(void);
