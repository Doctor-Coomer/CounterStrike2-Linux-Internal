#!/bin/bash

sudo pacman -S base-devel cmake sdl2 gdb vulkan-devel

cd /tmp
git clone https://aur.archlinux.org/execstack.git
cd execstack
makepkg -is

rm -rf /tmp/execstack
