Runner
======
A game written in mostly C99 with some caveats to handle MSVC oddities.

Building
-------

### Windows

On windows there are two options:

**Visual Studio 2013**

Although MSVC has not supported most C99/11 features more recently due to the need to support some of these features for C++11/14 compliance we have a lot more to work with.  Theoretically all the external dependencies are included and setup so all
you should have to do is open up runner.sln and it'll compile and run.

**mingw-w64**

Setting up the mingw-w64 toolchain is a bit more annoying.

The SDL2-mingw-w64 and SDL2_image-mingw-w64 libs will need to be extracted on your machine.  Currently the Makefile expects them to be in C:/dev/lib/x86.  The include directory is expected to be C:/dev/include.

This should probably change so that the mingw-w64 libs are in the External/ directory in the project.

Assuming git and the mingw-w64 binaries are in your PATH the whole process should look like this once you get the dependencies setup.

```
git clone https://github.com/tedajax/runner.git
cd runner
make
./copy_dll_32.bat
./runner
```

Running copy_dll_32.bat is important because it copies the .dlls from the External directory into the top level directory so they're next to the executable.  If you run the executable and it immediately exits make sure you run this step.

### Linux

Current dependencies are SDL2 and SDL2_image.

This has only been tested on Arch so far.

```
sudo pacman -S sdl2 sdl2_image
```

But I'm sure you can figure out how to install those packages on your distro.

Cloning, building, and running is then all very simple.

```
git clone https://github.com/tedajax/runner.git
cd runner
make
./runner
```

### OSX

I haven't tested this at all but it should work.  Same instructions as the **Linux** section but you'll have to use something like brew to install the dependencies

```
brew install sdl2 sdl2_image
```

License
-------

Currently licensed under the MIT license (see LICENSE.md)