## OpenGL Platformer
A 2D platformer with an in-house level editor made from scratch using C and
OpenGL.

### Dependencies
```
sudo apt install autoconf automake autotools-dev build-essential cmake \
         libfreetype6-dev libxkbcommon-dev libwayland-dev pkg-config xorg-dev
```

### Building
```
git clone --recurse-submodules https://github.com/justint9696/opengl-platformer.git
cd opengl-platformer
make libs all
```

### Running
After successfully building the application, it can be ran from the **root directory** with ```bin/game```. 

Level files can be imported at runtime by exporting the relative path to an environment variable:
```
export FNAME="data/level.dat"
make run
```

Passing the relative path directly to the executable works as well:
```
bin/game "data/level.dat"
```
