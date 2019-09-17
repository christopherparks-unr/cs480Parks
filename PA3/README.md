# PA3: Moons

## Instructions for Use
The makefile generates a file called ```CP_PA3``` to run. If no arguments are provided, it will run using the default shaders located in ```assets/shaders/```. Use ```-v "path"``` to load a different vertex shader, and ```-f "path"``` to load a different fragment shader.

Both of these options can be used as many times as you like, though due to insufficient knowledge on my behalf, the program will only load the last specified vertex and fragment shader in the list of arguments. Ignores all other arguments.

## Interacting
There are two objects to interact with in this project: the "planet" and the "moon". Interacting is based on a selection system: Press '1' on the keyboard (not the numpad) to select the planet, and press '2' to select the moon. Press '0' to remove active selection. **By default, there is no active selection.**

Once an object is selected, use the following keys to manipulate the object:
| Keypress      | Effect                        |
| ------------- | ----------------------------- |
| Q or LMB      | Reverse rotation              |
| A             | Pause / Unpause rotation      |
| W or RMB      | Reverse orbit                 |
| S             | Pause / Unpause orbit         |

##Extra Credit
The moon is currently set to be a hard-coded value of 0.5 times the "default size". Objects require a scale parameter as part of their initialization; this includes the planet, which has been hard-coded to 1.0. While there is no support to change scale via the launch options or during run-time, scale for either object can currently be modified by altering the 4th argument on lines 48 and 49 within ```src/graphics.cpp```.

Example of initializing a planet to 0.75 times the default size:
```
m_cube = new Object(1.0f, 1.0f, 1.0f, 0.75f, nullptr);
```

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
