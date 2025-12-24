/* Copyright 2024 nemohashi
 *
 * 56式ポケベル入力 (56-style Pager Input)
 * 共通ヘッダーファイル
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include QMK_KEYBOARD_H

// ============================================================
// 設定値（キーマップ側で上書き可能）
// ============================================================
#ifndef NIKOTOUCH_TIMEOUT_MS
#define NIKOTOUCH_TIMEOUT_MS 3000  // タイムアウト時間（ミリ秒）
#endif

#ifndef COMBO_TERM_56
#define COMBO_TERM_56 50  // コンボ検出時間（ミリ秒）
#endif

#ifndef TAPPING_TERM_M5
#define TAPPING_TERM_M5 200  // M5タップ/ホールド判定時間（ミリ秒）
#endif

#ifndef NIKOTOUCH_HOLD_TERM
#define NIKOTOUCH_HOLD_TERM 160  // 長押し判定時間（ミリ秒）
#endif

// 確定前バッファ設定
#ifndef PRECONFIRM_BUFFER_MAX_CHARS
#define PRECONFIRM_BUFFER_MAX_CHARS 20  // 確定前バッファ最大文字数
#endif
#define PRECONFIRM_BUFFER_SIZE (PRECONFIRM_BUFFER_MAX_CHARS * 2)  // キー入力数（1文字=2キー）

// ============================================================
// カスタムキーコード
// ============================================================
enum nikotouch_keycodes {
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
    NK_SENT,     // Shift + Enter（改行）
    NK_M1,       // M1: BASEレイヤー切替 + 英数
    NK_M2,       // M2: NIKOTOUCHレイヤー切替 + かな
    NK_M3,       // M3: 未割り当て
    NK_M4,       // M4: MO(FN)
    NK_M5,       // M5: LT(FN, NK_STAR)
    CMB_56,      // 5+6 コンボ（濁点変換）
    CMB_67,      // 6+7 コンボ（0キー相当）
    CMB_79,      // 7+9 コンボ（確定前バッファから残りを再入力）
    CMB_1BS,     // 1+BS コンボ（2文字削除）
    CMB_2BS,     // 2+BS コンボ（3文字削除）
    CMB_3BS,     // 3+BS コンボ（4文字削除）
    NIKOTOUCH_SAFE_RANGE,  // キーマップ側で追加のキーコードを定義する場合に使用
};

// ============================================================
// 共通レイヤー定義
// ============================================================
enum nikotouch_layers {
    NT_BASE = 0,
    NT_FN,
    NT_NIKOTOUCH,
    NT_NIKOSHIFT,
    NT_ARTSEY_ALPHA,
    NT_ARTSEY_KAKO,
    NT_ARTSEY_SYMBOL1,
    NT_ARTSEY_SYMBOL2,
    NT_ARTSEY_SYMBOL3,
    NT_LAYER_COUNT,  // = 9
};

// ============================================================
// 変換テーブル構造体
// ============================================================
typedef struct {
    uint8_t key1;           // 1回目のキー (0-9)
    uint8_t key2;           // 2回目のキー (0-9)
    const char *output;     // 送信文字列
    const char *star1;      // *1回目変換（NULL = *入力待ちなし）
    const char *star2;      // *2回目変換（NULL = *2回目なし）
} nikotouch_entry_t;

// ============================================================
// 公開関数
// ============================================================

// キーマップから呼び出す関数
bool process_record_nikotouch(uint16_t keycode, keyrecord_t *record);
void matrix_scan_nikotouch(void);

// 状態クリア関数（キーマップからも呼び出し可能）
void niko_clear(void);
void star_clear(void);
void preconfirm_clear(void);  // 確定前バッファクリア

// ============================================================
// 外部参照変数（M5のホールド状態確認用）
// ============================================================
extern bool m5_is_hold;
