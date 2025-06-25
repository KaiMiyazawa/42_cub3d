# 開発日記

## 2024/06/20 kisobe
- コンパイルするには、include/cub3d.h の `# include "../mlx-mms/mlx.h"` を `# include "../mlx-linux/mlx.h"` に変更し、libmlx.dylib を削除してください。

## 2024/06/21 kisobe
- イベントフック（W, A, S, D, ->, <-）を実装
- 床と天井の色を実装
- 壁との衝突判定を実装
- event.c と move.c を追加

## 2024/06/25 kmiyazaw
- ブランチを分けました
- 分けたブランチの先で、ft_err_printfをprintfに置き換えました

## 2024/06/29 kmiyazaw
- kmiyazaw_inputChecker ブランチを作成しました
  - マップの入力チェックを行う関数を追加する予定です
- kmiyazaw_def ブランチを整理し、pull requestを出しました

## 2024/06/30 kmiyazaw
- kmiyazaw_inputChecker ブランチで、入力.cubが正しかった場合の情報抽出をする関数を追加しました
- これにより、non_map情報を含むinputしか(現状)読めなくなりました
- non_map情報の過不足はエラーで弾けるようになりました（多分）
- また、non_map情報の順番は関係ないようになりました
- non_mapの情報のsuffix確認のみの追加なので、以下の項目は未着手です：
  - non_mapの情報のテクスチャの指定がvalidかどうかのチェック
  - non_mapの情報のRGBの値がvalidかどうかのチェック
  - mapの形がvalidかどうかのチェック
- mallocした変数（後々freeする必要のある変数）の命名規則の追加をしたいです
  - 例： `char **input_`
  - 変数名の最後にアンダースコアをつける

## 2024/07/07 kmiyazaw
- mapのチェックを作る関数を作成中です
  - 壁で囲まれているかどうかのチェックの方針（多分完成？）：
    1. map部分に対して、上下左右にpaddingした新たなmapを作る
    2. その新たなmapは、widthを揃えている（スペースなどの文字で埋める）
    3. padding-char（この場合は' '）の四方の隣が '1' or ' ' であることを確認する
    4. valid check
  - mapの文字の正しさチェック：
    - 順番に見ていって、'0' or '1' or '.' および、N, S, E, W があるかどうかを確認する
    - それ以外ならエラー
    - N, S, E, W が２つ以上ある場合はエラー
- エラーの出力は、わかりやすさ優先でsubjectに準拠せずに書いてあるものばかりです
- 多分、mapのチェックは完成したと思われます
- [ ] なぜか、caps lockをhookしていて、caps lockを押すとexitします