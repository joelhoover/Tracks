# Tracks

A submission for the 5th SFML game jam, with the theme "Failure is an option".

An endless time trial on infinite rail tracks, interspersed with gaps.

Controls:

- ```Spacebar``` - jump
- ```F``` - Rocket boost

## Building

To build Tracks it is necessary to obtain the following libraries:

- [SFGUI](https://github.com/TankOs/SFGUI)
- [SFML](https://github.com/SFML/SFML)
- [Thor](https://github.com/Bromeon/Thor)
- [Boost](http://www.boost.org) (header-only)

Then, Tracks may be built by

```
mkdir build
cd build
cmake ..
make install
```

To elide the need for root privileges to install, set the CMAKE_INSTALL_PREFIX. Alternatively, ensure that the png and font files are in the working directory of the executable.