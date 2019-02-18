# cloudpose

## Installation

```bash
# general
sudo apt-get update
sudo apt-get remove unattended-upgrades
sudo apt-get --assume-yes install libopencv-dev cmake

# openpose
git clone https://github.com/CMU-Perceptual-Computing-Lab/openpose.git
cd openpose
bash scripts/ubuntu/install_deps_and_cuda.sh
mkdir build; cd build
cmake ..
make -j`nproc`
sudo make install
sudo ldconfig
cd

# go
sudo add-apt-repository ppa:gophers/archive
sudo apt-get update
sudo apt-get install golang-1.11-go
echo 'PATH="/usr/lib/go-1.11/bin:$PATH"' >> .profile
source .profile

# cloudpose
git clone https://github.com/256dpi/cloudpose.git
cd cloudpose
go build
cp -r ../openpose/models/ models
./cloudpose
```

## Running

```
# /etc/systemd/system/cloudpose.service

[Unit]
Description=cloudpose
After=network.target

[Service]
WorkingDirectory=/home/ubuntu/cloudpose
ExecStart=/home/ubuntu/cloudpose/cloudpose
Restart=always
RestartSec=3

[Install]
WantedBy=default.target
```

```bash
sudo systemctl start cloudpose.service
sudo systemctl enable cloudpose.service

sudo systemctl status cloudpose.service
journalctl -u cloudpose.service -b

sudo systemctl stop cloudpose.service
sudo systemctl disable cloudpose.service
```
