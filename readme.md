# Community Patch

## Code guidelines
The patch should be a standalone DLL. At the moment this means that most C++ standard library features are out of the question: with MinGW, even statically linking libstdc++ dynamically imports `pthreads`. If there is a way to disable the standard library's thread-safety, we may be able to use it!

clang-format decides how the code should be formatted.
