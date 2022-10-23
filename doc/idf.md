# Idf setup

this is how to setup a development environment for this project

go to your thirdparty directory 

```bash
git clone https://github.com/espressif/esp-idf.git
git submodule update --init --recursive
cd esp-idf
git checkout v4.4
./install.sh
. ./export/sh
```
go back to this directory and do 

```bash
idf.py set-target esp32
idf.py build
idf.py -p /dev/<wherever> flash
```
