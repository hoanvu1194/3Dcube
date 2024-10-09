## Requirements:
1. Write a program using C/C++ to draw a 3D cube that can rotate using OpenGL.
2. Use the arrow keys to rotate the cube around the X and Y axes.
3. The program should allow the user to change the color of each face of the cube using the number keys from 1 to 6 (each number corresponds to one face of the cube). Example: pressing key 1 changes the color of the front face to a random color.
4.Apply textures to the cube's faces. These textures should be loaded from external image files (e.g., .jpg or .png). Switch between using colors and using textures by pressing the "T" key.
5. Auto-rotation feature: The cube should be able to automatically rotate around one axis at an adjustable speed. The user can enable or disable auto-rotation by pressing the "A" key. The auto-rotation speed should be adjustable using the mouse wheel.
6. Use optimization techniques such as VBO (Vertex Buffer Objects) and VAO (Vertex Array Objects) to increase rendering speed.

## Tested on Ubuntu 22.04

## Before build
sudo apt update
sudo apt install g++
sudo apt install build-essential libglfw3-dev libglew-dev libglm-dev

## Build and run command
./build_and_run.sh
