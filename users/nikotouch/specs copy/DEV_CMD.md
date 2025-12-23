# ニコタッチキーマップ 開発コマンド一覧

## ファームウェアのビルド

### 通常ビルド
```bash
cd /home/tetsuo/workspace/qmk_firmware
source .venv/bin/activate
make keychron/q0_max/encoder:nikotouch
```

生成されるファイル：
- `.build/keychron_q0_max_encoder_nikotouch.bin`
- `keychron_q0_max_encoder_nikotouch.bin`（ルートディレクトリにコピー）

### ビルド＆書き込み
```bash
cd /home/tetsuo/workspace/qmk_firmware
source .venv/bin/activate
make keychron/q0_max/encoder:nikotouch:flash
```

## DFUモードの操作

### DFUモードに入る方法

#### 方法1: ソフトウェアリセット（推奨）
キーボード操作で：
1. **M5キー（左下）を押しながら**
2. **右上の`-`キー（QK_BOOT）を押す**

#### 方法2: 物理リセットボタン
1. USBケーブルを抜く
2. キーボード裏面のリセットボタンをクリップで押しながら
3. USBケーブルを接続

### DFUモードの確認
```bash
lsusb | grep -i "DFU\|STM"
```

期待される出力：
```
Bus 007 Device 006: ID 0483:df11 STMicroelectronics STM Device in DFU Mode
```

### 全USBデバイスの確認
```bash
lsusb
```

### DFUモードから通常モードに戻る

#### 方法1: コマンドで戻す
```bash
dfu-util -a 0 -s 0x08000000:leave
```

#### 方法2: 物理的に戻す
USBケーブルを抜き差しする

## ファームウェア書き込みの完全な流れ

### 初回（物理リセット使用）
```bash
# 1. ビルド
cd /home/tetsuo/workspace/qmk_firmware
source .venv/bin/activate
make keychron/q0_max/encoder:nikotouch

# 2. DFUモードに入る（物理リセットボタン）
# 3. 確認
lsusb | grep -i "DFU\|STM"

# 4. 書き込み
make keychron/q0_max/encoder:nikotouch:flash
```

### 2回目以降（ソフトリセット使用）
```bash
# 1. ビルド
cd /home/tetsuo/workspace/qmk_firmware
source .venv/bin/activate
make keychron/q0_max/encoder:nikotouch

# 2. キーボードでDFUモードに入る（M5 + 右上の-キー）
# 3. 書き込み
make keychron/q0_max/encoder:nikotouch:flash
```

## トラブルシューティング

### DFUモードで固まった場合
```bash
# コマンドで通常モードに戻す
dfu-util -a 0 -s 0x08000000:leave

# または
# USBケーブルを抜き差し
```

### ビルドエラーの場合
```bash
# クリーンビルド
make clean
make keychron/q0_max/encoder:nikotouch
```

### 仮想環境が有効か確認
```bash
which python
# /home/tetsuo/workspace/qmk_firmware/.venv/bin/python と表示されればOK
```

## キーマップの配置

### ファイル構成
```
keyboards/keychron/q0_max/encoder/keymaps/nikotouch/
├── keymap.c        # メインキーマップファイル
├── rules.mk        # ビルド設定
└── specs/
    ├── 001.md          # 仕様書
    ├── NIKOTOUCH.csv   # 変換テーブル
    └── DEV_CMD.md      # このファイル
```

## よく使うgitコマンド

### 変更の確認
```bash
git status
git diff
```

### コミット
```bash
git add keyboards/keychron/q0_max/encoder/keymaps/nikotouch/
git commit -m "Update nikotouch keymap"
```

### プッシュ
```bash
git push origin nikotouch-keymap
```
