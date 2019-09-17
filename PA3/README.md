# PA2: Interaction: Menus, Keyboard, and Mouse

## Instructions for Use
The makefile generates a file called ```CP_PA3``` to run. If no arguments are provided, it will run using the default shaders located in ```assets/shaders/```. Use ```-v "path"``` to load a different vertex shader, and ```-f "path"``` to load a different fragment shader.

Both of these options can be used as many times as you like, though due to insufficient knowledge on my behalf, the program will only load the last specified vertex and fragment shader in the list of arguments. Ignores all other arguments.

## Interacting
Press the left mouse button or 'Q' on the keyboard to reverse the rotation of the cube. Press the right mouse button or 'W' on the keyboard to reverse the orbit of the cube. Press 'A' to pause / unpause the rotation of the cube, and 'S' to pause / unpause the orbit of the cube.


# Dependencies, Building, and Running

## Dependency Instructions
For both of the operating systems to run this project installation of these three programs are required [GLEW](http://glew.sourceforge.net/), [GLM](http://glm.g-truc.net/0.9.7/index.html), and [SDL2](https://wiki.libsdl.org/Tutorials).

This project uses OpenGL 3.3. Some computers, such as virtual machines in the ECC, can not run this version. In in order to run OpenGL 2.7 follow the instructions at [Using OpenGL 2.7](https://github.com/HPC-Vis/computer-graphics/wiki/Using-OpenGL-2.7)

### Ubuntu/Linux
```bash
sudo apt-get install libglew-dev libglm-dev libsdl2-dev
```

### Mac OSX
Installation of brew is suggested to easily install the libs. Ensure that the latest version of the Developer Tools is installed.
```bash
brew install glew glm sdl2
```

## Building and Running
Running the make in a separate directory will allow easy cleanup of the build data, and an easy way to prevent unnecessary data to be added to the git repository.  

### Makefile Instructions 
The makefile works as expected and must be updated with new files added in.

```bash
mkdir build
cd build
cp ../makefile .
make
./CP_PA3
```

Example run code:
```
./CP_PA3 -f ../assets/shaders/fragment_pa1.txt -v ../assets/shaders/vertex_pa1.txt
```

## Ubuntu.cse.unr.edu
OpenGL 3.3 will run on the [ubuntu.cse.unr.edu](https://ubuntu.cse.unr.edu/) website. To do so follow the build instructions, but when running the executable use this line to execute.
```bash
/usr/NX/scripts/vgl/vglrun ./CP_PA3
```
