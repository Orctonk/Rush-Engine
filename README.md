# Rush-Engine
Rush is a graphics engine in the early development stages. Though it is currently only tested on Linux the end goal is platform independence.

[![License](https://img.shields.io/github/license/Orctonk/Rush-Engine)](https://www.gnu.org/licenses/gpl-3.0)

## Building Rush


To build Rush from scratch, including all dependencies (See [Dependencies](#Dependencies) for more info), start off by cloning the repo and all submodules:

On Linux:
```
$ git clone https://github.com/Orctonk/Rush-Engine.git --recurse-submodules
```
The program can then be build using CMake:

On Linux:
```
$ mkdir Rush-Engine/build && cd Rush-Engine/build
$ cmake .. && cmake --build .
```
This will build the Rush library as well as the Rudder editor. If you wish to simply use the Rush library, please refer to the [Sandbox Application](./Sandbox/)

## Features
Rush is currently pretty bare-bones and provides only the most basic features. These include:
* Basic window management.
* Basic 3D-rendering.
* Basic model loading and materials.
* Basic Framebuffers.
* Basic Editor.
* Event system.
* Basic layer stack for event propagation.

## TODO
While Rush is pretty bare-bones currently. Many more features are planned for the future:
* More advanced material system.
* More flexible model loading.
* Particle system.
* Camera manager.
* Scene saving and loading.
* Instrumentation.
* Custom platform agnostic file utilities.

## Dependencies
Rush is built using [CMake](https://cmake.org/) and uses the following external libraries:
* [Assimp](https://www.assimp.org/) for model loading.
* [Stb_image](https://github.com/nothings/stb/blob/master/stb_image.h) for texture loading.
* [ImGui](https://github.com/ocornut/imgui) for GUI rendering.
* [EnTT](https://github.com/skypjack/entt) for entity management.
* [GLM](https://glm.g-truc.net/) for mathematics.

Additionally, Rush currently only support OpenGL. As such both [GLFW](https://www.glfw.org/) and [Glad](https://glad.dav1d.de/) are required.

All these dependencies are included as submodules to this repository and can be built from source. However, due to the size of Assimp, this library can be located on the local machine if installed to reduce overall compile times. This is done by setting the `RUSH_BUILD_ASSIMP` option in CMake, which is on by default.

## License
[![License](https://img.shields.io/github/license/Orctonk/Rush-Engine)](https://www.gnu.org/licenses/gpl-3.0)

* Copyright 2020 © Filip Henningsson
* Licensed under [GPLv3](https://www.gnu.org/licenses/gpl-3.0). Licence file found [here](./LICENSE)
