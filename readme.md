# Community Patch
A feature update patch for Age of Empires II: The Conquerors, building on UserPatch 1.5 to further expand modding capabilities.

## End User Documentation
Please see the [docs/][] folder.

## Code guidelines
The patch should be a standalone DLL. At the moment this means that most C++ standard library features are out of the question: with MinGW, even statically linking libstdc++ dynamically imports `pthreads`. If there is a way to disable the standard library's thread-safety, we may be able to use it!

clang-format decides how the code should be formatted:
```bash
clang-format -style=file -i src/*.h src/*.cpp src/*/*.h src/*/*.cpp
```

## License
This code is private for now, and possibly for ever. If you have access to this repo you may distribute the `aoc-community-patch.dll` file, but not the source code.

[docs/]: docs/
