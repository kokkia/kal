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

## cpp
### souece file間をまたいでグローバル変数を使う
- 外部リンケージをもたせる
- a.cpp で変数定義
- b.cpp でextern + 変数。このとき代入等の処理はせず、実体はつくらない
- 参考：http://www7b.biglobe.ne.jp/~robe/cpphtml/html01/cpp01069.html

### クラス内のメンバとしてのクラスの初期化
- コンストラクタ初期化子で初期化を行う
- デフォルトコンストラクタ+初期化関数でも機能はするが、代入であって、初期化ではない

### 変数のメモリガードの受け渡し
- atomic変数を利用
- 変数が多い場合は、mutexで処理自体をロックする

## ROS update error
- エラー:11 http://packages.ros.org/ros/ubuntu focal/main amd64....................
- のエラーが出てROS packageのupdateやinstallができない場合、keyの期限が切れているらしい
sudo apt install curl # if you haven't already installed curl
curl -s https://raw.githubusercontent.com/ros/rosdistro/master/ros.asc | sudo apt-key add -
