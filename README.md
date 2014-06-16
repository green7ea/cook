Cook
====


Summary
-------

Cook is an experimental build system. Much like ekam, it is a reverse
makefile: instead of specifying dependencies, cook is supposed to
figure them out for you. Cook is written in C++ and concentrates on
compiling C and C++.

Cook is in pre alpha. At this stage, it is only supposed to be used to
explore the possibilities of the build system.

So far it can handle:
  - c++ compilation
  - symbol extraction from .o files
  - symbol dependency solving
  - linking files containing a main symbol with it's immediate dependecies
  - basic YAML configuration files
  - continuous building based on inotify
  - header awareness

Near term planned features:
  - default values
  - parallel building
  - emacs integration
  - autotools files generation


Dependencies
------------

Cook only works on linux based systems since it depends on inotify. It
is written in C++11 and requires a modern compiler (gcc or clang). It
also depends on jansson to parse YAML.


Using
-----

To use cook, simply call the cook command with julia installed from a
repository containing a file called Recipe. A Recipe file must be
valid YAML. Here is a brief example:

```YAML
{
  "compiler": "clang++",
  "nm": "nm",
  "flags": ["-pipe", "-Wall", "-pedantic", "-std=c++11"],
  "packages": ["jansson"]
}
```

- compiler: should point to a valid compiler.
- nm: should point to a valid nm program for the generated .o files.
- flags: contains the compiler flags you might want.
- libs: contains the compiler flags added during the linking phase.
- packages: uses pkg-config to generate the appropriate cflags and
  libs for the packages. Automatically adds them to flags and libs.


Installation
------------

There is no automatic installation option at the moment since this is
experimental software.
