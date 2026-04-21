# MuPDF 1.27.2 — CMake fork

Fork of [MuPDF 1.27.2](https://mupdf.com/) by Artifex Software with a CMake
build system replacing the original Makefile.

## Requirements

- **CMake** 3.21 or newer
- **C and C++ compiler**
  - MSVC (Visual Studio 2019+), GCC, or Clang
  - x86-64 CPU with SSE4.1 support (required by the deskew path on non-MSVC)
- No external library dependencies — all third-party libraries are bundled

## Build

```sh
cmake -B build -DCMAKE_BUILD_TYPE=Release
```

```sh
cmake --build build
```

On Windows with Visual Studio you can also open the folder directly in the IDE
or use:

```sh
cmake -B build -G "Visual Studio 17 2022"
```

```sh
cmake --build build --config Release
```

### Emscripten / WebAssembly

Use `emcmake` to configure and then build normally. With the official Docker image:

```sh
docker run -it --rm -v "$(pwd):/src" emscripten/emsdk:5.0.2 emcmake cmake -S . -B build-web -DCMAKE_BUILD_TYPE=Release
```

```sh
docker run -it --rm -v "$(pwd):/src" emscripten/emsdk:5.0.2 cmake --build build-web -j
```

This produces `libmupdf.a` and `libmupdf_third.a` under `build-web/` that you can link into your own Emscripten application target.

## Consuming the library

The build exports a single target. Add this repository as a subdirectory or use
`FetchContent`:

```cmake
add_subdirectory(mupdf)
target_link_libraries(my_target PRIVATE mupdf::mupdf)
```

The public headers are under `include/`.

## What is included / excluded

All third-party codecs (FreeType, HarfBuzz, zlib, libjpeg, OpenJPEG, jbig2dec,
Brotli, lcms2, MuJS, Gumbo, Extract) are bundled and compiled from source.

The following parts of the upstream source tree were removed to keep the
repository smaller:

| Removed                                        | Reason                                            |
| ---------------------------------------------- | ------------------------------------------------- |
| `thirdparty/tesseract`, `thirdparty/leptonica` | OCR support disabled (`FZ_ENABLE_OCR_FALLBACK=0`) |
| `thirdparty/zxing-cpp`, `thirdparty/zint`      | Barcode support disabled (`FZ_ENABLE_BARCODE=0`)  |
| `thirdparty/curl`                              | HTTP client not needed for core PDF               |
| `thirdparty/freeglut`, `platform/gl/`          | OpenGL viewer not built                           |
| `platform/java/`                               | Java JNI bindings not built                       |
| `platform/wasm/`                               | WebAssembly port not built                        |
| `platform/win32/`                              | Visual Studio projects replaced by CMake          |
| `platform/x11/`                                | X11 viewer not built                              |
| `source/tools/`                                | CLI tools (mutool, mudraw, etc.) not built        |
| `source/tests/`                                | Test suite not built                              |
| `docs/`                                        | Sphinx documentation not included                 |
| `.github/`                                     | CI workflows and GitHub metadata not included     |
| `.gitmodules`                                  | Submodules replaced by vendored source            |

Noto, CJK, SIL, and Emoji fonts are not pre-generated (`TOFU_NOTO`, `TOFU_CJK`,
`TOFU_SIL`, `TOFU_EMOJI`). Only the URW Base14 fonts are compiled in. PDFs that
embed their own fonts are unaffected; standard PDF fonts fall back to the URW
equivalents.
