# Algorithms and Data Structures Library
This is a personal project to design and implement common data structures and algorithms using C++ 20 features, including concepts and static polymorphism. Many of the design features mirror C++ STL designs, with some changes. The solution includes both source code and test project with unit tests using GoogleTest and GMock.

## Setup
The project uses CMake to build the unit tests. Read the full [CMake documentation](https://cmake.org/cmake/help/book/mastering-cmake/index.html) for more information. This will automatically compile all unit tests and build the result to a specified output directory.

### Install CMake
Use your preferred method to install cmake on your machine, such as: <br>
* [pip](https://pypi.org/project/cmake/) - install will ```pip install cmake```
* [chocolatey](https://community.chocolatey.org/packages/cmake) - install with ```choco install cmake```
* [Install manually](https://cmake.org/download/)
  
## Build Unit Tests
Opening the project in Visual Studio IDE with [CMake extensions](https://learn.microsoft.com/en-us/cpp/build/cmake-projects-in-visual-studio?view=msvc-170) installed will automatically build the tests; No user interactions neccessary. Test executables will be available under the startup dropdown.<br>

Otherwise if you are building tests manually run the following commands from the project's root directory:
```
cmake -S . -B out/build
cmake --build out/build
```

and cmake will build all visible unit tests.

## Run Tests
Again, if using Visual Studio IDE simply run the tests under the startup tab and they will appear in your IDEs Test Explorer. To run tests from the command line run the ctest command from the build directory:
```
cd out/build
ctest
```