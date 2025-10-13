#!/bin/bash
sudo dnf -y install git cmake gdb gcc g++ make SDL2-devel vulkan-loader-devel execstack
sudo dnf -y group install "c-development"
sudo dnf -y group install "development-tools"
