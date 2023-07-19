#!/bin/bash

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
PROJECT_DIR="${SCRIPT_DIR}/.."
BUILD_DIR=`mktemp -d -p /tmp`
NUM_CORES=`grep -c ^processor /proc/cpuinfo`

function register_service() {
  SERVICE_NAME=$1
  SERVICE_FILE=$2
  [[ -z "$SERVICE_FILE" ]] && SERVICE_FILE="${PROJECT_DIR}/scripts/${SERVICE_NAME}.service"

  echo -ne "Configuring service: ${SERVICE_NAME} : "
  [[ ! -f "/etc/systemd/system/${SERVICE_FILE}" ]] && sudo cp ${SERVICE_FILE} /etc/systemd/system/ && sudo /usr/bin/systemctl daemon-reload && echo -ne "OK\n" || echo -ne "Already configured.\n"
  echo -ne "Enabling service: ${SERVICE_NAME} : "
  [[ ! $(/usr/bin/systemctl status ${SERVICE_NAME} | grep "Loaded:" | grep "; enabled;") ]] && sudo /usr/bin/systemctl enable ${SERVICE_NAME} && echo -ne "OK\n" || echo -ne "Already enabled.\n"
  echo -ne "Starting service: ${SERVICE_NAME} : "
  [[ ! $(/usr/bin/systemctl status ${SERVICE_NAME} | grep "Active:" | grep "(running)") ]] && sudo /usr/bin/systemctl start ${SERVICE_NAME} && echo -ne "OK\n"|| echo -ne "Already started.\n"
}

# echo -e "Checking host configuration: "
# sudo apt update
# sudo apt upgrade
# sudo useradd startrack -G users,dialout,plugdev,adm,sudo -m -p startrack
# sudo adduser --add-extra-groups startrack

echo -e "Installing Source and Prerequisites: "
sudo apt -yq install avahi-daemon network-manager cmake g++ gcc gdb build-essential libstdc++-13-dev libusb-1.0-0-dev libyaml-cpp-dev libeigen3-dev libgtest-dev libspdlog-dev libfmt-dev

echo -e "Enabling hotplug support for USB mass storage: "
if [[ ! -f "/etc/media-automount.d" ]]
then
  pushd ~
  git clone https://github.com/Ferk/udev-media-automount.git
  cd udev-media-automount
  sudo make install
  sudo udevadm control --reload-rules
  sudo udevadm trigger
  popd
fi

# cd ~
# git clone git@github.com:inertialsense/StarTrack.git

echo -e "Building StarTrack: "
pushd ${PROJECT_DIR}
/usr/bin/cmake -DCMAKE_BUILD_TYPE=Debug -G "CodeBlocks - Unix Makefiles" -S ${PROJECT_DIR} -B ${BUILD_DIR}
/usr/bin/cmake --build ${BUILD_DIR} -- -j ${NUM_CORES}
popd

echo -e "Installing systemd services: "
pushd ${BUILD_DIR}
sudo make install pigpiod
sudo make install StarTrack
sudo ldconfig
popd

register_service "pigpiod"
register_service "startrack"

sudo rm -rf ${BUILD_DIR}