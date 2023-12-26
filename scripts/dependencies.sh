#!/bin/bash

export DEBIAN_FRONTEND=noninteractive
echo "Installing dependencies"
sudo apt update;
sudo -E apt install -y gcc g++ libyaml-cpp-dev make cmake git ccache clang-format clang-tidy gdb valgrind sqlite sqlite3 libsqlite3-dev qtbase5-dev qtchooser qt5-qmake qtbase5-dev-tools openmpi-bin openmpi-common openmpi-doc libopenmpi-dev doxygen graphviz imagemagick python3-sphinx dia imagemagick texlive dvipng latexmk texlive-extra-utils texlive-latex-extra texlive-font-utils libeigen3-dev gsl-bin libgsl-dev libgslcblas0 libxml2 libxml2-dev libgtk-3-dev lxc-utils lxc-templates vtun uml-utilities ebtables bridge-utils libxml2 libxml2-dev libboost-all-dev python3-pip > apt.log 2>&1
python3 -m pip install --user cppyy==2.4.2
#export PATH=$PATH:/home/$USER/.local/bin
