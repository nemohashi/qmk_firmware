// Copyright 2026 Goronaya (@nemohashi)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "quantum.h"

/*
 * BELL-56-mini キーボードレイアウト
 *
 * 物理レイアウト:
 * ┌──────┬──────┬──────┬──────┐
 * │      │      │      │  Fn  │  (k04)
 * ├──────┼──────┼──────┼──────┤
 * │  1   │  2   │  3   │  BS  │  (k00, k01, k02, k03)
 * ├──────┼──────┼──────┼──────┤
 * │  4   │  5   │  6   │      │  (k10, k11, k12)
 * ├──────┼──────┼──────┤ Ent  │  (k24)
 * │  7   │  8   │  9   │      │  (k13, k14, k20)
 * ├──────┴──────┼──────┴──────┤
 * │   Space     │      0      │  (k21, k22)
 * └─────────────┴─────────────┘
 *
 * マトリックス配置:
 *     COL0  COL1  COL2  COL3  COL4
 * ROW0  1     2     3    Bs     Fn
 * ROW1  4     5     6     7     8
 * ROW2  9   Space   0    -     Ent
 * ROW3  -     -     -     -      -
 * ROW4  -     -     -     -      -
 */

#define LAYOUT( \
    k00, k01, k02, k03, k04, \
    k10, k11, k12, k13, k14, \
    k20, k21, k22, k24 \
) { \
    { k00,   k01,   k02,   k03,   k04   }, \
    { k10,   k11,   k12,   k13,   k14   }, \
    { k20,   k21,   k22,   KC_NO, k24   }, \
    { KC_NO, KC_NO, KC_NO, KC_NO, KC_NO }, \
    { KC_NO, KC_NO, KC_NO, KC_NO, KC_NO }  \
}
