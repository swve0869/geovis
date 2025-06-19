Conceptual notes: 
openGL is specification it is NOT a library. (its a cross lang cross platform API). like a blueprint describing what a graphics driver should do when a command to it is sent. 
Different graphics card companies implent the openGL spec in their hardware specific drivers. 
 -> So an openGL implementation is already on your system in the graphics driver. and GLAD is a library that allows us to interact with the driver. 

glfw is NOT openGL. it is a platform independent windowing library, and event handler that creates an openGL context but DOES NOT have any renderring abilities.
 -> the actual functions that use openGL are exposed by the current graphics driver running in the kernel. User space apps can get these function pointers from the grahpics 
    driver by requesting whether the driver supports a specific openGL action (eg. glCreateShader) the driver will return the corresponding function pointer if it does. This   
    function pointer will allow for the user space app to directly interact with the graphics driver using the openGL spec.
        -> This process of requesting a function pointer can be done but its very tedious. this why GLAD exists so that we can get access to all the openGL functions that the 
            the current graphics driver supports.     

Remember that functions declared (but not defined) in a header file. that are part of a library must be linked using the -l<libname> flag


Steps taken

# compile guide for glfw
https://www.glfw.org/docs/latest/compile_guide.html#compile_deps

# install dev deps for ubuntu
sudo apt install libwayland-dev libxkbcommon-dev xorg-dev

#create build directory for glfw cmake and src directory
#run from geovis root to compile glfw using cmake. The default for cmake on linux is to include wayland and x11 support
cmake -S glfw-3.4 -B build

#cmake has now generated a make file in the build directory. 
#This will run the platform specific build tool the directory was generated for.
cd build 
make
sudo make install           #THIS STEP IS NOT MENTIONED ON THE SITE (it will make glfw3.h available by adding it to the system directory usr/local/include)

#install glad at this link. (c/c++ gl v:4.6)
https://glad.dav1d.de/https://rpxomi.github.io/
#extract the zip and copy the glad directory and the KHR directory under include to /usr/include. also copy glad.c from the src folder into the project.
cd ~/Downlods/glad
sudo cp -r include/* /usr/include
cp src/ ~/path/to/project

# glfw starter guide for building applications
https://www.glfw.org/docs/latest/quick_guide.html
# this guide uses linmath.h. It's a header only library so I can just throw the header file into /usr/local/include  
wget https://raw.githubusercontent.com/datenwolf/linmath.h/master/linmath.h
sudo cp linmath.h /usr/local/include/


https://www.glfw.org/docs/latest/build_guide.html


Current compile command:    explanation : glad.c needs to be compiled because main.c has glad.h in it. -lglfw3 links the GLFW library and -lm links math.h
gcc main.c glad.c -o output -lglfw3 -lm


