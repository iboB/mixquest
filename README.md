MixQuest
========

MixQuest is a game-like demo and, alas, is not an actual playable game (yet).

## Demo

The project gives a real-world example for the following:

* [DynaMix](https://github.com/iboB/dynamix) - A dynamic type composition, OOP, and polymorphism library.
* [yama](https://github.com/iboB/yama) - A game math library.

## Setup instructions

There are submodules in this repo. Don't forget to call `git clone --recursive` or after cloning `git submodule update --init --recursive`

### Linux/Unix

Only clang is supported!

Install glfw3 with your package manager or from source (for Ubuntu `$ sudo apt-get install glfw3-dev`)

```bash
$ cd build
$ mkdir gen
$ cd gen
$ cmake ../.. -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++
$ make
```

To run use the repo's root directory as for a working directory. Eg `$ ./bin/mixquest`

### Windows

Only Visual C++ 2015 or later is supported

Download glfw 3.2.1 from source. Build and install in the `vendor/glfw-3.2.1` subdirectory.

Generate build scripts or project files with cmake using the same generator that you used for the glfw build.

To run start from the repo's root directory. Eg `> bin\mixquest`

## License and Copyright

### Source code

The source code outside of the `third_party` directory is distributed under the MIT Software License. See LICENSE.txt for further details or copy [here](http://opensource.org/licenses/MIT).

Copyright &copy; 2017-2018 Borislav Stanimirov.

### Assets

The assets in the `assets` directory are licensed as listed in `assets/README.md`. Copyright by their respective authors.

