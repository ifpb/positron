#!/bin/bash

export DEBIAN_FRONTEND=noninteractive
echo "Installing dependencies"
sudo apt update;
#sudo -E apt-get install -y g++ python3 python3-dev pkg-config sqlite3 git python3-setuptools qt5-default mercurial gdb valgrind gir1.2-goocanvas-2.0 python-gi python-gi-cairo python3-gi python3-gi-cairo python3-pygraphviz gir1.2-gtk-3.0 ipython3 openmpi-bin openmpi-common openmpi-doc libopenmpi-dev autoconf cvs bzr unrar uncrustify doxygen graphviz imagemagick texlive texlive-extra-utils texlive-latex-extra texlive-font-utils dvipng latexmk python3-sphinx dia  gsl-bin libgsl-dev libgsl23 libgslcblas0 tcpdump sqlite sqlite3 libsqlite3-dev libxml2 libxml2-dev cmake libc6-dev libc6-dev-i386 libclang-6.0-dev llvm-6.0-dev automake python3-pip libgtk-3-dev vtun lxc uml-utilities libboost-dev libboost-filesystem-dev wget libyaml-cpp-dev vim > apt.log 2>&1
sudo -E apt install -y make cmake python3-pip sqlite sqlite3 libsqlite3-dev libyaml-cpp-dev > apt.log 2>&1
python3 -m pip install --user cppyy=2.4.2
export PATH=$PATH:/home/$USER/.local/bin
