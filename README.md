# CRUDER

CRUDER is a C header-only library to deal with lists of variables, it is designed to be fast and easy to use.

CRUDER is meant to be compatible with gcc-compatible compilers, it is 100% macro-based.

CRUDER is actually a older version of BRUTER, if you want a more complete, portable, readable, debuggable, maintainable and **predictable** library, you should really checkout the current BRUTER https://github.com/brutopolis/bruter, it is 100% C99 compatible.

Unlike BRUTER, CRUDER doesnt have type system, key system, _Generics, static inlined functions, or dispatching, it is also not meant to be 100% C99, it does require some gcc extensions;

CRUDER doesnt have a base type or union like BRUTER, you can create a list of any type you want, all the macros are type-agnostic;