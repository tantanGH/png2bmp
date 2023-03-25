# PNG2BMP.X

PNG to BMP converter for X680x0/Human68k

PNGファイルをBMPファイルに変換します。24bitRGBまたは32bitRGBAのPNGのみサポートしています。

---

### インストール方法

PNGBMxxx.ZIP をダウンロードして展開し、PNG2BMP.X をパスの通ったディレクトリに置きます。

---

### 使用方法

    png2bmp [オプション] <PNGファイル名>

      -o <出力ファイル名> ... 省略した場合はPNGファイルの拡張子を.bmpに変えたものになります
      -s    ... 半分のサイズに縮小します
      -h    ... ヘルプメッセージを表示します

060loadhigh.x を使ったハイメモリ上での実行に対応しています。

---

### Special Thanks

* xdev68k thanks to ファミべのよっしんさん
* HAS060.X on run68mac thanks to YuNKさん / M.Kamadaさん / GOROmanさん
* HLK301.X on run68mac thanks to SALTさん / GOROmanさん

---

### History

* 0.1.0 (2023/03/25) ... 初版
