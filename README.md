# Project001

This project contains a Custom Game Framework, an application to test said framework, and games made with the framework.

The framework has:

An Entity Component System with contiguous per-component storage and entity-to-component indexing.

A renderer abstraction with interchangeable OpenGL and Vulkan backends.

A rendering system with mesh and texture resource management, mesh batching, instanced rendering, render-order sorting, and frustum culling to reduce unnecessary draw calls.

A 2D rigid-body physics and collision system supporting multiple collision shapes, collision manifolds, friction, and quadtree-based spatial partitioning.

Functions for Font, Mesh, and Texture resource loading

Mesh generation utilities for 2D and 3D shapes, lines, and text

Various missilanious funcitons useful for game dev

Libraries: GLAD, GLFW, GLM, miniaudio, stb.

Games made can be downloaded here:

https://vik10.itch.io/trick-or-treat-on-spooky-street

https://vik10.itch.io/cool-penguin-snowball

To build the project you'll need CMake and Visual Studio.

*You may need to update the cmake_generator value in the Setup.bat to the version you have.*

Run the Setup.bat script.

It'll pull submodules, build vendors, and generate the visual studio solution.