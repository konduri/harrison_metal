sudo apt-get install vim
sudo apt-get install terminator
sudo apt-gt install qt4-dev-tools
sudo apt-get install qt4-qmake
sudo apt-get install qt4-default
sudo apt-get install g++
cd pixy/scritps
./build_pixymon_src.sh

cd ../src/host/linux
sudo cp pixy.rules /etc/udev/rules.d

