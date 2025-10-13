#!/bin/bash

# Setup the overlay that has execstack
sudo eselect repository enable gentoo-zh
sudo emaint -r gentoo-zh sync
sudo bash -c "echo \"dev-util/execstack ~amd64\" > /etc/portage/package.accept_keywords/execstack"

# Build and install as many packages with emerge as possible
sudo emerge media-libs/libsdl dev-debug/gdb dev-util/vulkan-headers dev-util/execstack 
