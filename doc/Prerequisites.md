
Prerequisites {#Prerequisites}
------------

[TOC]
 
Hopefully complete list of hardware, software and libraries required to build the Audio Engine.

# Platform {#Platform} 

Supported platforms are:

  * Windows XP or later
  * Mac OS X 10.5 or later

Porting to Linux should be relatively easy but has not been done yet.

# Compiler {#Compiler}

A compiler supporting the C99 standard is required. Currently tested are:

  * clang 3.1   or later (Apple call this compiler "4.0.0 based of SVN 3.1")
  * GCC   4.6.2 or later
  
The default Windows compiler (Visual Studio) is not supported, MinGW/MSYS is recommended.

# Tools {#Tools}

### CMake

<http://www.cmake.org>

Required for building the libraries and binaries.

At present the *only* supported generators are Makefiles on Linux OS X, and MSYS Makefiles on
Windows. Cygwin Makefiles *might* work, but is not well tested, and not recommended as the Cygwin
compiler might not generate standard Windows binaries.

### Modulo

<https://github.com/hanshoglund/modulo>

Required for generating headers and low-level language bindings.

### Dist

<https://github.com/hanshoglund/dist>

Required for automatic package resolution. Not required if all dependecies are built locally.

### Doxygen

<http://www.stack.nl/~dimitri/doxygen>

Required for generating the documentation.

### GNU Make

Required for building language bindings and documentation.



# Libraries {#Libraries}

These are typically handled by the package mangager, see [build and install](@ref BuildAndInstall).

### Portaudio

<http://www.portaudio.com>
<http://portmedia.sourceforge.net>

Required for real-time audio streams on all systems.

### Portmidi                                       

<http://portmedia.sourceforge.net>

Required for real-time midi streams on all systems.

### Libsndfile

<http://www.mega-nerd.com/libsndfile>

Required for non-real-time audio streams on all systems.

### Fluidsynth

<http://sourceforge.net/apps/trac/fluidsynth>

Required for using the Fluidsynth audio processor.


               