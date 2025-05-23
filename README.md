## Build GKExtension_PCL
### Set environment variables
Set environment variables:
- RDF_ENGINE_INCLUDE= path to engine.h 
- RDF_ENGINE_LIB= path to engine.lib 
- RDF_ENGINE_EXTENSIONS_PATH= target path to build Release
- RDF_ENGINE_EXTENSIONS_PATH_DEBUG= target path to build Debug + intermediate(.obj) fies
### Build with VS 2022
- Open GKExtension_PCL\GKExtension_PCL.vcxproj
- Select Release-x64 target 
### Dependencies
This repo includes pre-build PCL an its dependecies
- headers
- libraried for Win x64 Release/Debug
- binaries for Win x64 Release
Thus you can run x64 Release/Debug, and run x64 Release out of the box.
Other targets will require to install PCL. Follow the instructions on https://github.com/PointCloudLibrary/pcl 
