# ExprParse
[![Actions Status](https://github.com/amgary/exprparse/workflows/ci/badge.svg)](https://github.com/amgary/exprparse/actions)

A simple c++ library to compute values from simple math expressions stored as strings.

## License
`exprparse` is distributed under the MIT license.

## Repository Structure

**exprparse** is the main library, and provides the api.

**exprcalc** is a simple calculator application that demonstrates how to use the `exprparse` library.

**tests** contains unit tests for the `exprparse` library using the googletest library.

## Building ExprParse
`exprparse` uses [cmake](https://cmake.org/) to generate cross-platform build files.

The project and `.gitignore` file is setup for out of source builds in a top level
directory called `build`. However, any out of source folder is fine.

`exprparse` depends on the [googletest](https://github.com/google/googletest) suite for unit tests. The cmake project will download and build this dependency if testing is enabled. To disable building the tests, pass `-DBUILD_TESTING=OFF` to cmake.

### Example build
Starting from a terminal open in the same directory as this Readme

```bash
# Make build directory
mkdir build
cd build

# Run cmake to generate build files (pass -DBUILD_TESTING=OFF to skip building the testsuite)
cmake ../src

# Build
make

# Run tests
ctest

# Install
make install
```
