# Image Processor
I present to you a simple photo editor in C++ using GTK3 and Magick++
In this editor you can:
- Change the image size
- Rotate the image
- Add a text watermark

Supported image formats:
- png
- jpg/jpeg
- gif
- tiff
- BMP

### Requirements
- pkg-config
- clang++ / g++
- cmake
- Magick++ 6/7
- gtkmm-3.0

To install these packages on ubuntu you can use the following command:
```
sudo apt install pkg-config g++ cmake libmagick++-dev libgtkmm-3.0-dev
```
### How to build
I used the ```cmake``` build system for this project.
You may build it yourself or you may use the ```build.sh``` script

##### Usage:
```
sh ./build.sh
```

### Important notes
The ```build.sh``` script will compile the program and put all the cmake files in the ```./build``` folder, and the ```image_processor``` program file will be in the root of the repository (where the script itself is).

The ```ui.glade``` file describes the appearance of the application window.
This file must be in the same directory as the program file.