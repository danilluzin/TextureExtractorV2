# Texture extraction from Photographs
## Danil Luzin

Tool generates texture images for 3D models using camera information from reconstruction tools and source images.

Supported reconstruction tools:
- Visual SFM
- Bundler SFM
- 3D laser scanning software developed in the CTU FEE Department of Computer Graphics and Interaction


## Getting Started

File `manual.pdf` is a user manual and describes all the information neccessary to get started.

### Prerequisites

A tool can be build using CMake. ( https://cmake.org )

Required libraries are
- OpenCV https://opencv.org
- GLM https://glm.g-truc.net/0.9.8/index.html
- TBB https://www.threadingbuildingblocks.org

### Quick Start

Configuration of the tool is handled using .INI files.

You can generate example .INI file using the following command:
```
./tex_extractor -genIni
```
The complete explanation of .INI file can be found in the user manual.

A tool can then be used as following:
```
./tex_extractor path/to/config.ini
```

### Demo
You can try to run an example dataset demo provided in this repository:
```
./tex_extractor example/demo.ini
```
It will output final textures into an `example\result` folder. It may take several minutes to complete, depending on you system.


NOTE: You may need to flip final textures vertically (y = -1) depending on what rendering software you use.

## Repository structure
- `manual.pdf`: a user manual. Includes explanation of INI config file.
- `renders` : folder that contains some render results using textures our tool is capable of generating.
- `doc`: folder with Doxygen code documentation.
- `example`: folder with demo example dataset.
- `CmakeLists.txt`: main Cmake file.
- `Doxyfile`: Doxygen configuration file.
- `BP_Luzin_Danil_2018.pdf`: Bachelors thesis text.
