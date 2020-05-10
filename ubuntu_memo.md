# Ubuntuトラブルメモ

## イヤホンから音が出ないときの対処法  
参考(https://forums.ubuntulinux.jp/viewtopic.php?pid=63098)

/etc/modprobe.d/alsa-base.conf　の最後に  
options snd-hda-intel model=basic  
と書き込み、保存、再起動で解決

## gnuplotのインストール



## nccatの使い方