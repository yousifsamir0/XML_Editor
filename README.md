# XML_Editor

Lightweight XML editor and utilities (format, minify, convert to JSON, compress).

Branch for this refactor: `refactor/clean-headers`.

Build
 - Requires Qt (qmake) and a standard make tool.
 - From the repository root run:

```sh
lua build.lua
```

Or run manually:

```sh
qmake XML-Editor.pro
make -j$(nproc)
```

Usage
 - Run the produced executable (from build output) and use the UI to open, edit, format, minify, convert, compress XML files.

Notes
 - This refactor cleans header `Headers/tree.h` to provide a consistent, safer API.
 - `build.lua` is a simple helper that invokes `qmake` and `make` for convenience.

If you want, I can continue: split large source files, add unit tests, or modernize build to CMake.