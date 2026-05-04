# 编译代码
mkdir build
cd build
cmake ..
make MAMBAF405_2022B


# 当前版本可以定高定点(3寸机)，烧录代码即可 20260504

# 问题
1. 电池没电没有自动下降程序


# 通道二打开，可以在没有搜到室内进行解锁
rxGetChannelValue(AUX2) > 1700;


