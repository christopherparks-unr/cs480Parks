# PA6: Assimp Texture Loading
Group Project:
Chris Parks
Grant Thompson

## Adding objects to the project
Within ```/assets/scenes/``` are a few sample object files. cube.obj is the orginal object that was hard-coded in the first project. dragon.obj is the sample file found on the class website, already included for your convenience. The same applies for buddha.obj.

You can put objects anywhere you like so long as you reference the correct path -- you are not limited to the /assets/scenes/ folder.

## Instructions for Use
The makefile generates a file called ```PA6``` to run. If no arguments are provided, it will run using the default shaders located in ```assets/shaders/```. Use ```-v <path>``` to load a different vertex shader, and ```-f <path>``` to load a different fragment shader.

You may specify multiple vertex and fragment shaders, though due to insufficient knowledge on my behalf, the program will only load the last ones specified. Ignores all other arguments.

## Defining Objects to Render
Objects are defined within ```/PA6/main.ini```. Look at the included file to see how it should be formatted. Some notes about the attributes:

The first line must be the name you give the object. Use ```[<name>]``` to define the beginning of a new object and mark the end of the old object definition.

The parent argument refers to the name of the object (as described above).

The scene_path argument refers to the model you wish to import.  **The path is relative to ```/PA6/```.**

**Note that the last object defined will not be loaded; make sure that ```[EOF]``` is the final line in the file to ensure all objects are loaded.**

The mesh_name argument refers to the specific mesh you want to use. You must use the name of the mesh as defined by Assimp (specifically, the ```aiMesh.mName``` variable. All objects included are defined in main.ini with their correct mesh names).

Do not define scale to be 0.0. It will break, and it doesn't make sense to use that value anyways.

# Dependencies, Building, and Running

## Dependency Instructions
For both of the operating systems to run this project installation of these three programs are required [GLEW](http://glew.sourceforge.net/), [GLM](http://glm.g-truc.net/0.9.7/index.html), and [SDL2](https://wiki.libsdl.org/Tutorials).

This project uses OpenGL 3.3. Some computers, such as virtual machines in the ECC, can not run this version. In in order to run OpenGL 2.7 follow the instructions at [Using OpenGL 2.7](https://github.com/HPC-Vis/computer-graphics/wiki/Using-OpenGL-2.7)

This project also uses [Assimp 4.0.1](https://github.com/assimp/assimp/releases/tag/v4.1.0/).
It also uses [stb_image.h](https://raw.githubusercontent.com/nothings/stb/master/stb_image.h) to implement texture loading.

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
./PA6
```

Example run code:
```
./PA6 -f ../assets/shaders/fragment_pa1.txt -v ../assets/shaders/vertex_pa1.txt
```

## Ubuntu.cse.unr.edu
OpenGL 3.3 will run on the [ubuntu.cse.unr.edu](https://ubuntu.cse.unr.edu/) website. To do so follow the build instructions, but when running the executable use this line to execute.
```bash
/usr/NX/scripts/vgl/vglrun ./PA6
```
