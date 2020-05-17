# Ubuntuメモ

## イヤホンから音が出ないときの対処法  
参考(https://forums.ubuntulinux.jp/viewtopic.php?pid=63098)

/etc/modprobe.d/alsa-base.conf　の最後に  
options snd-hda-intel model=basic  
と書き込み、保存、再起動で解決

## gnuplotのインストール
$ sudo apt-get install gnuplot
$ sudo apt-get intall gnuplot-x11


## nccatの使い方
1. 送信側  
nc -u remoteserver port  
  
1. 受信側  
nc -u -l port
