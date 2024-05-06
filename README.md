## OpenGL Platformer
A 2D platformer made from scratch with C and OpenGL. 

### Dependencies
#### Linux (Debian) 
```
sudo apt install autoconf automake autotools-dev build-essential cmake \
         libxkbcommon-dev libwayland-dev pkg-config xorg-dev
```

### Building
#### Linux
```
git clone --recurse-submodules https://github.com/justint9696/opengl-platformer.git
cd opengl-platformer
make libs all
```

After successfully building the application, it can be ran from the **root directory** with ```bin/game```.
