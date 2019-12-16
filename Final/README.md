# Final
Group Project:
Chris Parks
Grant Thompson

## This project

This project is a first person physics demo, featuring a movable character, several colored lights, a modified imported level from Valve's popular 2007 game *Team Fortress 2*, and several basic shapes and props that can be manipulated by walking into them or firing a projectile into them.

## Controls

Use WASD to move, and Left Mouse Button to shoot a projectile.

## Instructions for Use
The makefile generates a file called ```Final``` to run.

## Defining Objects to Render
Objects are defined within ```/Final/main.ini```. Look at the included file to see how it should be formatted. Some notes about the attributes:

The first line must be the name you give the object. Use ```[<name>]``` to define the beginning of a new object and mark the end of the old object definition.

The parent argument refers to the name of the object (as described above).

The scene_path argument refers to the model you wish to import.  **The path is relative to ```/Final/```.**

**Note that the last object defined will not be loaded; make sure that ```[EOF]``` is the final line in the file to ensure all objects are loaded.**

# Dependencies, Building, and Running

## Dependency Instructions
For both of the operating systems to run this project installation of these three programs are required [GLEW](http://glew.sourceforge.net/), [GLM](http://glm.g-truc.net/0.9.7/index.html), and [SDL2](https://wiki.libsdl.org/Tutorials).

This project uses OpenGL 3.3. Some computers, such as virtual machines in the ECC, can not run this version. In in order to run OpenGL 2.7 follow the instructions at [Using OpenGL 2.7](https://github.com/HPC-Vis/computer-graphics/wiki/Using-OpenGL-2.7)

This project also uses [Assimp 4.0.1](https://github.com/assimp/assimp/releases/tag/v4.1.0/) and [stb_image.h](https://raw.githubusercontent.com/nothings/stb/master/stb_image.h) to implement texture loading.

Finally, this project implements the [Bullet Physics Engine](https://pybullet.org/Bullet/BulletFull/index.html).

### Ubuntu/Linux
```bash
sudo apt-get install libglew-dev libglm-dev libsdl2-dev assimp-dev
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
./Final
```

Example run code:
```
./Final
```

## Ubuntu.cse.unr.edu
OpenGL 3.3 will run on the [ubuntu.cse.unr.edu](https://ubuntu.cse.unr.edu/) website. To do so follow the build instructions, but when running the executable use this line to execute.
```bash
/usr/NX/scripts/vgl/vglrun ./Final
```