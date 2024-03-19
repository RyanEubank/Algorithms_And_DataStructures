# Algorithms and Data Structures Library
This is a personal project to design and implement common data structures and algorithms using C++20 features, including concepts and static polymorphism. Many of the design features mirror C++ STL designs, with some changes. The solution includes both source code and test project with unit tests using GoogleTest and GMock.

## Setup
Reccomended to use Visual Studio IDE to run tests, however the current MSVC test adapter for GoogleTest does not include GMock. Please follow [this tutorial](https://stackoverflow.com/questions/60486110/how-to-use-googlemock-in-visual-studio) to setup the GoogleTest/GMock library as an external project (list as Option #2) to link with the Unit Test project.

## Run Tests
Compile and run gtest_main.cc, ensure C++20 features are enabled. Alternatively run tests with Visual Studio Test Explorer.
