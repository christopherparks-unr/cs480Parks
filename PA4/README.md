# PA3: Moons

## Adding objects to the project
Within ```/assets/objects/``` are a few sample object files. cube.obj is the orginial object that was hard-coded in the first project. tray.obj is the object I created in Blender. dragon.obj is the sample file found on the class website, already included for your convenience.

You don't need to specifically add objects into this folder, though it is the convention. **Once you add a new object into any directory, you must add it as a new record to the ```object_list.txt``` file located at /PA4/. If not, the program will not load the vertex and index information.** The purpose of using object_list.txt is to allow easier implementation of multiple instances of an object. It doesn't serve much purpose for this project, but could be useful later.

## Instructions for Use
The makefile generates a file called ```CP_PA4``` to run. If no arguments are provided, it will run using the default shaders located in ```assets/shaders/```. Use ```-v <path>``` to load a different vertex shader, and ```-f <path>``` to load a different fragment shader.

**Use ```-o <filename>``` to use that model when loading.** If this argument is not specified, it defaults to cube.obj (therefore this object should not be removed from object_list.txt). Note that this argument is different from -v and -f in that it is not a path, only a file name.

All of these options can be used as many times as you like, though due to insufficient knowledge on my behalf, the program will only load the last specified vertex shader, fragment shader, and object specified in the list of arguments. Ignores all other arguments.

## Interacting
There is only one object to interact with in this project. Press '1' on the keyboard (not the numpad) to select the object, and press '0' to remove active selection. **By default, there is no active selection.**

Once an object is selected, use the following keys to manipulate the object:

| Keypress      | Effect                        |
|:-------------:|:-----------------------------:|
| Q or LMB      | Reverse rotation              |
| A             | Pause / Unpause rotation      |
| W or RMB      | Reverse orbit                 |
| S             | Pause / Unpause orbit         |

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
./CP_PA4
```

Example run code:
```
./CP_PA4 -f ../assets/shaders/fragment_pa1.txt -v ../assets/shaders/vertex_pa1.txt -o dragon.obj
```

## Ubuntu.cse.unr.edu
OpenGL 3.3 will run on the [ubuntu.cse.unr.edu](https://ubuntu.cse.unr.edu/) website. To do so follow the build instructions, but when running the executable use this line to execute.
```bash
/usr/NX/scripts/vgl/vglrun ./CP_PA4
```
