# Vulkan-Triangle

This is a (relatively) simple project I decided to do in order to learn about the Vulkan API. The source code is (or will be) heavily commented so this project can be used as a reference to look back on if I ever forget anything. And who knows, maybe someone will find this useful on their own Vulkan journey. :)

## Installation

First of all, clone this repository somewhere on your computer. After cloning, go into the `shader` directory and compile the shaders by running the correct script for your operating system (`.sh` for Linux and `.bat` for Windows).

### Windows

- Download and install the [Vulkan SDK](https://vulkan.lunarg.com/). (Preferably the same version used in the project, currently version 1.3.231.1. You could probably use more recent versions.)
- Change the `VULKAN_SDK` variable in the CMakeLists file to where you've installed the SDK. Also change the `VULKAN_VERSION` variable if you've chosen to go for a more recent version.
- Configure the CMake project and build it.
- The executable should be located in the `build` folder.

### Linux

- Coming soon.

### MacOS

- I'm not planning on adding MacOS support.