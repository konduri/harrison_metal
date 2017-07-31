#run as a bash script

echo -e ¨install libusb¨
sudo apt-get install libusb-1.0-0.dev

echo -e ¨install lib_boost-all-dev¨
sudo apt-get install libboost-all-dev

echo -e ¨install cmake¨
sudo apt-get install cmake

echo -e ¨clone the repo¨
git clone https://github.com/charmedlabs/pixy.git


echo -e ¨build the library¨
cd pixy/scripts
./build_libpixyusb.sh

echo -e ¨INSTALL THE LIBRARY¨
sudo ./install_libpixyusb.sh
