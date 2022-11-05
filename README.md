# Vulkan-Triangle

This is a (relatively) simple project I decided to do in order to learn about the Vulkan API. The source code is (or will be) heavily commented so this project can be used as a reference to look back on if I ever forget anything. And who knows, maybe someone will find this useful on their own Vulkan journey. :)

## Installation

First of all, clone this repository somewhere on your computer.

### Windows

- Download and install the [Vulkan SDK](https://vulkan.lunarg.com/). (Preferably the same version used in the project, currently version 1.3.231.1. You could probably use more recent versions.)
- Change the `VULKAN_SDK` variable in the CMakeLists file to where you've installed the SDK. Also change the `VULKAN_VERSION` variable if you've chosen to go for a more recent version.
- Configure the CMake project and build it.
- Go to the `shaders` directory and run the `compile.bat` script to compile the shaders.
- The executable should be located in the `build` folder.

### Linux

The following instructions apply to Arch Linux. Other distributions have different names for the packages.

- Install the following packages from pacman: `vulkan-devel shaderc`. (`vulkan-devel` contains everything needed for vulkan development, including Vulkan headers and the library required for linking. The `shaderc` package contains the `glslc` binary used to compile GLSL code into SPIR-V bytecode.)
- Go to the `shaders` directory and run the `compile.sh` script to compile the shaders. If permission is denied, open a terminal, change directories to the `shader` directory, and run `sh compile.sh`.

### MacOS

- I'm not planning on adding MacOS support.

## Developer Notes

- As you might have noticed, the window is not resizable. This is because I haven't implemented the code to resize the viewport and framebuffers. This also causes tiling window managers to render the window as floating.
