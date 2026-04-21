# ==============================================================================
# MuPDF 1.27.2 — CMake build (forked from Artifex Software)
# ==============================================================================
# Source: https://casper.mupdf.com/downloads/archive/mupdf-1.27.2-source.tar.gz
#
# Removed from the original source tree to keep the repo smaller:
#   docs/                        — Sphinx documentation
#   platform/gl/                 — OpenGL viewer application
#   platform/java/               — Java JNI bindings
#   platform/wasm/               — WebAssembly port
#   platform/win32/              — Visual Studio solution/project files
#   platform/x11/                — X11 viewer application
#   source/tools/                — CLI tools (mudraw, mutool, etc.)
#   source/tests/                — Test suite
#   source/helpers/mu-office-lib/— Windows-only Office helper library
#   thirdparty/curl/             — HTTP client (not needed for core PDF)
#   thirdparty/freeglut/         — OpenGL viewer dependency
#   thirdparty/leptonica/        — OCR image-processing dependency
#   thirdparty/tesseract/        — OCR engine
#   thirdparty/zint/             — Barcode generation
#   thirdparty/zxing-cpp/        — Barcode detection
# Not present in 1.27.2 tarball (removed upstream before this release):
#   thirdparty/jpegxr/           — JPEG XR codec (XPS JPEG XR images unsupported)
#   scripts/                     — Build/generation scripts (all except
#                                  scripts/freetype/ and scripts/libjpeg/)
#   Makefile / Makelists / Makerules / Makethird — replaced by this file
#   setup.py / pyproject.toml    — Python packaging
#   .readthedocs.yaml            — ReadTheDocs config
#
# Targets provided:
#   mupdf::mupdf  — core library (links mupdf_third transitively)
# ==============================================================================

cmake_minimum_required(VERSION 3.21)
project(mupdf VERSION 1.27.2 LANGUAGES C CXX)

# Detect compiler family for flag gating
if(MSVC)
    set(_mupdf_is_msvc TRUE)
else()
    set(_mupdf_is_msvc FALSE)
endif()

# ==============================================================================
# mupdf_third — bundled third-party codec/utility libraries
# Each component that needs distinct compile flags gets its own OBJECT library.
# ==============================================================================

# ------------------------------------------------------------------------------
# FreeType
# ------------------------------------------------------------------------------
add_library(mupdf_ft OBJECT
    thirdparty/freetype/src/base/ftbase.c
    thirdparty/freetype/src/base/ftbbox.c
    thirdparty/freetype/src/base/ftbitmap.c
    thirdparty/freetype/src/base/ftdebug.c
    thirdparty/freetype/src/base/ftfstype.c
    thirdparty/freetype/src/base/ftgasp.c
    thirdparty/freetype/src/base/ftglyph.c
    thirdparty/freetype/src/base/ftinit.c
    thirdparty/freetype/src/base/ftmm.c
    thirdparty/freetype/src/base/ftstroke.c
    thirdparty/freetype/src/base/ftsynth.c
    thirdparty/freetype/src/base/ftsystem.c
    thirdparty/freetype/src/base/fttype1.c
    thirdparty/freetype/src/cff/cff.c
    thirdparty/freetype/src/cid/type1cid.c
    thirdparty/freetype/src/psaux/psaux.c
    thirdparty/freetype/src/pshinter/pshinter.c
    thirdparty/freetype/src/psnames/psnames.c
    thirdparty/freetype/src/raster/raster.c
    thirdparty/freetype/src/sfnt/sfnt.c
    thirdparty/freetype/src/smooth/smooth.c
    thirdparty/freetype/src/truetype/truetype.c
    thirdparty/freetype/src/type1/type1.c
)
target_include_directories(mupdf_ft PRIVATE
    thirdparty/freetype/include
    scripts/freetype
)
target_compile_definitions(mupdf_ft PRIVATE
    FT2_BUILD_LIBRARY
    "FT_CONFIG_MODULES_H=<slimftmodules.h>"
    "FT_CONFIG_OPTIONS_H=<slimftoptions.h>"
)

# ------------------------------------------------------------------------------
# HarfBuzz — text shaping (C++, no RTTI/exceptions)
# ------------------------------------------------------------------------------
add_library(mupdf_hb OBJECT
    thirdparty/harfbuzz/src/graph/gsubgpos-context.cc
    thirdparty/harfbuzz/src/hb-aat-layout.cc
    thirdparty/harfbuzz/src/hb-aat-map.cc
    thirdparty/harfbuzz/src/hb-blob.cc
    thirdparty/harfbuzz/src/hb-buffer.cc
    thirdparty/harfbuzz/src/hb-buffer-verify.cc
    thirdparty/harfbuzz/src/hb-buffer-serialize.cc
    thirdparty/harfbuzz/src/hb-common.cc
    thirdparty/harfbuzz/src/hb-face.cc
    thirdparty/harfbuzz/src/hb-fallback-shape.cc
    thirdparty/harfbuzz/src/hb-font.cc
    thirdparty/harfbuzz/src/hb-ft.cc
    thirdparty/harfbuzz/src/hb-map.cc
    thirdparty/harfbuzz/src/hb-number.cc
    thirdparty/harfbuzz/src/hb-ot-cff1-table.cc
    thirdparty/harfbuzz/src/hb-ot-cff2-table.cc
    thirdparty/harfbuzz/src/hb-ot-color.cc
    thirdparty/harfbuzz/src/hb-ot-face.cc
    thirdparty/harfbuzz/src/hb-ot-font.cc
    thirdparty/harfbuzz/src/hb-ot-layout.cc
    thirdparty/harfbuzz/src/hb-ot-map.cc
    thirdparty/harfbuzz/src/hb-ot-math.cc
    thirdparty/harfbuzz/src/hb-ot-meta.cc
    thirdparty/harfbuzz/src/hb-ot-metrics.cc
    thirdparty/harfbuzz/src/hb-ot-name.cc
    thirdparty/harfbuzz/src/hb-ot-shape.cc
    thirdparty/harfbuzz/src/hb-ot-shape-fallback.cc
    thirdparty/harfbuzz/src/hb-ot-shape-normalize.cc
    thirdparty/harfbuzz/src/hb-ot-shaper-arabic.cc
    thirdparty/harfbuzz/src/hb-ot-shaper-default.cc
    thirdparty/harfbuzz/src/hb-ot-shaper-hangul.cc
    thirdparty/harfbuzz/src/hb-ot-shaper-hebrew.cc
    thirdparty/harfbuzz/src/hb-ot-shaper-indic.cc
    thirdparty/harfbuzz/src/hb-ot-shaper-indic-table.cc
    thirdparty/harfbuzz/src/hb-ot-shaper-khmer.cc
    thirdparty/harfbuzz/src/hb-ot-shaper-myanmar.cc
    thirdparty/harfbuzz/src/hb-ot-shaper-syllabic.cc
    thirdparty/harfbuzz/src/hb-ot-shaper-thai.cc
    thirdparty/harfbuzz/src/hb-ot-shaper-use.cc
    thirdparty/harfbuzz/src/hb-ot-shaper-vowel-constraints.cc
    thirdparty/harfbuzz/src/hb-ot-tag.cc
    thirdparty/harfbuzz/src/hb-ot-var.cc
    thirdparty/harfbuzz/src/hb-set.cc
    thirdparty/harfbuzz/src/hb-shape.cc
    thirdparty/harfbuzz/src/hb-shape-plan.cc
    thirdparty/harfbuzz/src/hb-shaper.cc
    thirdparty/harfbuzz/src/hb-static.cc
    thirdparty/harfbuzz/src/hb-subset.cc
    thirdparty/harfbuzz/src/hb-subset-cff1.cc
    thirdparty/harfbuzz/src/hb-subset-cff2.cc
    thirdparty/harfbuzz/src/hb-subset-cff-common.cc
    thirdparty/harfbuzz/src/hb-subset-input.cc
    thirdparty/harfbuzz/src/hb-subset-plan.cc
    thirdparty/harfbuzz/src/hb-ucd.cc
    thirdparty/harfbuzz/src/hb-unicode.cc
)
target_include_directories(mupdf_hb PRIVATE
    thirdparty/harfbuzz/src
    include/mupdf
    thirdparty/freetype/include
    scripts/freetype
)
target_compile_definitions(mupdf_hb PRIVATE
    HAVE_FALLBACK=1
    HAVE_FREETYPE
    HAVE_OT
    HAVE_ROUND
    HAVE_UCDN
    HB_NO_MT
    HB_NO_PRAGMA_GCC_DIAGNOSTIC
    hb_malloc_impl=fz_hb_malloc
    hb_calloc_impl=fz_hb_calloc
    hb_free_impl=fz_hb_free
    hb_realloc_impl=fz_hb_realloc
)
if(NOT _mupdf_is_msvc)
    target_compile_options(mupdf_hb PRIVATE
        -fno-exceptions
        -fno-rtti
        -fno-threadsafe-statics
        -fvisibility-inlines-hidden
        -w
    )
endif()

# ------------------------------------------------------------------------------
# Gumbo — HTML5 parser
# ------------------------------------------------------------------------------
add_library(mupdf_gumbo OBJECT
    thirdparty/gumbo-parser/src/attribute.c
    thirdparty/gumbo-parser/src/char_ref.c
    thirdparty/gumbo-parser/src/error.c
    thirdparty/gumbo-parser/src/parser.c
    thirdparty/gumbo-parser/src/string_buffer.c
    thirdparty/gumbo-parser/src/string_piece.c
    thirdparty/gumbo-parser/src/tag.c
    thirdparty/gumbo-parser/src/tokenizer.c
    thirdparty/gumbo-parser/src/utf8.c
    thirdparty/gumbo-parser/src/util.c
    thirdparty/gumbo-parser/src/vector.c
)
target_include_directories(mupdf_gumbo PRIVATE
    thirdparty/gumbo-parser/src
    # Windows (MSVC and Clang-on-Windows) has no system <strings.h>, so the
    # compat shim must be on the path. On Linux/macOS/Emscripten the shim would
    # shadow the real system header (causing a #pragma-once self-block), so we
    # only add it on WIN32.
    $<$<BOOL:${WIN32}>:${CMAKE_CURRENT_SOURCE_DIR}/scripts/compat>
)
if(NOT _mupdf_is_msvc)
    target_compile_options(mupdf_gumbo PRIVATE -std=c99)
endif()

# ------------------------------------------------------------------------------
# zlib
# ------------------------------------------------------------------------------
add_library(mupdf_zlib OBJECT
    thirdparty/zlib/adler32.c
    thirdparty/zlib/compress.c
    thirdparty/zlib/crc32.c
    thirdparty/zlib/deflate.c
    thirdparty/zlib/inffast.c
    thirdparty/zlib/inflate.c
    thirdparty/zlib/inftrees.c
    thirdparty/zlib/trees.c
    thirdparty/zlib/uncompr.c
    thirdparty/zlib/zutil.c
)
target_include_directories(mupdf_zlib PRIVATE thirdparty/zlib)
target_compile_definitions(mupdf_zlib PRIVATE
    HAVE_STDARG_H
    # HAVE_UNISTD_H omitted on Windows — unistd.h does not exist there
    $<$<NOT:$<BOOL:${WIN32}>>:HAVE_UNISTD_H>
)

# ------------------------------------------------------------------------------
# libjpeg
# ------------------------------------------------------------------------------
add_library(mupdf_jpeg OBJECT
    thirdparty/libjpeg/jaricom.c
    thirdparty/libjpeg/jcapimin.c
    thirdparty/libjpeg/jcapistd.c
    thirdparty/libjpeg/jcarith.c
    thirdparty/libjpeg/jccoefct.c
    thirdparty/libjpeg/jccolor.c
    thirdparty/libjpeg/jcdctmgr.c
    thirdparty/libjpeg/jchuff.c
    thirdparty/libjpeg/jcinit.c
    thirdparty/libjpeg/jcmainct.c
    thirdparty/libjpeg/jcmarker.c
    thirdparty/libjpeg/jcmaster.c
    thirdparty/libjpeg/jcomapi.c
    thirdparty/libjpeg/jcparam.c
    thirdparty/libjpeg/jcprepct.c
    thirdparty/libjpeg/jcsample.c
    thirdparty/libjpeg/jdapimin.c
    thirdparty/libjpeg/jdapistd.c
    thirdparty/libjpeg/jdarith.c
    thirdparty/libjpeg/jdatadst.c
    thirdparty/libjpeg/jdatasrc.c
    thirdparty/libjpeg/jdcoefct.c
    thirdparty/libjpeg/jdcolor.c
    thirdparty/libjpeg/jddctmgr.c
    thirdparty/libjpeg/jdhuff.c
    thirdparty/libjpeg/jdinput.c
    thirdparty/libjpeg/jdmainct.c
    thirdparty/libjpeg/jdmarker.c
    thirdparty/libjpeg/jdmaster.c
    thirdparty/libjpeg/jdmerge.c
    thirdparty/libjpeg/jdpostct.c
    thirdparty/libjpeg/jdsample.c
    thirdparty/libjpeg/jdtrans.c
    thirdparty/libjpeg/jerror.c
    thirdparty/libjpeg/jfdctflt.c
    thirdparty/libjpeg/jfdctfst.c
    thirdparty/libjpeg/jfdctint.c
    thirdparty/libjpeg/jidctflt.c
    thirdparty/libjpeg/jidctfst.c
    thirdparty/libjpeg/jidctint.c
    thirdparty/libjpeg/jmemmgr.c
    thirdparty/libjpeg/jquant1.c
    thirdparty/libjpeg/jquant2.c
    thirdparty/libjpeg/jutils.c
)
target_include_directories(mupdf_jpeg PRIVATE
    thirdparty/libjpeg
    scripts/libjpeg
)

# ------------------------------------------------------------------------------
# OpenJPEG — JPEG2000
# ------------------------------------------------------------------------------
add_library(mupdf_openjpeg OBJECT
    thirdparty/openjpeg/src/lib/openjp2/bio.c
    thirdparty/openjpeg/src/lib/openjp2/cio.c
    thirdparty/openjpeg/src/lib/openjp2/dwt.c
    thirdparty/openjpeg/src/lib/openjp2/event.c
    thirdparty/openjpeg/src/lib/openjp2/function_list.c
    thirdparty/openjpeg/src/lib/openjp2/ht_dec.c
    thirdparty/openjpeg/src/lib/openjp2/image.c
    thirdparty/openjpeg/src/lib/openjp2/invert.c
    thirdparty/openjpeg/src/lib/openjp2/j2k.c
    thirdparty/openjpeg/src/lib/openjp2/jp2.c
    thirdparty/openjpeg/src/lib/openjp2/mct.c
    thirdparty/openjpeg/src/lib/openjp2/mqc.c
    thirdparty/openjpeg/src/lib/openjp2/openjpeg.c
    thirdparty/openjpeg/src/lib/openjp2/pi.c
    thirdparty/openjpeg/src/lib/openjp2/sparse_array.c
    thirdparty/openjpeg/src/lib/openjp2/t1.c
    thirdparty/openjpeg/src/lib/openjp2/t2.c
    thirdparty/openjpeg/src/lib/openjp2/tcd.c
    thirdparty/openjpeg/src/lib/openjp2/tgt.c
    thirdparty/openjpeg/src/lib/openjp2/thread.c
)
target_include_directories(mupdf_openjpeg PRIVATE
    thirdparty/openjpeg/src/lib/openjp2
)
target_compile_definitions(mupdf_openjpeg PRIVATE
    OPJ_STATIC
    OPJ_HAVE_INTTYPES_H
    OPJ_HAVE_STDINT_H
    MUTEX_pthread=0
)

# ------------------------------------------------------------------------------
# jbig2dec
# ------------------------------------------------------------------------------
add_library(mupdf_jbig2 OBJECT
    thirdparty/jbig2dec/jbig2.c
    thirdparty/jbig2dec/jbig2_arith.c
    thirdparty/jbig2dec/jbig2_arith_iaid.c
    thirdparty/jbig2dec/jbig2_arith_int.c
    thirdparty/jbig2dec/jbig2_generic.c
    thirdparty/jbig2dec/jbig2_halftone.c
    thirdparty/jbig2dec/jbig2_huffman.c
    thirdparty/jbig2dec/jbig2_hufftab.c
    thirdparty/jbig2dec/jbig2_image.c
    thirdparty/jbig2dec/jbig2_mmr.c
    thirdparty/jbig2dec/jbig2_page.c
    thirdparty/jbig2dec/jbig2_refinement.c
    thirdparty/jbig2dec/jbig2_segment.c
    thirdparty/jbig2dec/jbig2_symbol_dict.c
    thirdparty/jbig2dec/jbig2_text.c
)
target_include_directories(mupdf_jbig2 PRIVATE
    thirdparty/jbig2dec
    include
)
target_compile_definitions(mupdf_jbig2 PRIVATE
    HAVE_STDINT_H
    "JBIG_EXTERNAL_MEMENTO_H=\"mupdf/memento.h\""
)

# ------------------------------------------------------------------------------
# Brotli — compression
# ------------------------------------------------------------------------------
add_library(mupdf_brotli OBJECT
    thirdparty/brotli/c/common/constants.c
    thirdparty/brotli/c/common/context.c
    thirdparty/brotli/c/common/dictionary.c
    thirdparty/brotli/c/common/platform.c
    thirdparty/brotli/c/common/shared_dictionary.c
    thirdparty/brotli/c/common/transform.c
    thirdparty/brotli/c/dec/bit_reader.c
    thirdparty/brotli/c/dec/decode.c
    thirdparty/brotli/c/dec/huffman.c
    thirdparty/brotli/c/dec/state.c
    thirdparty/brotli/c/enc/backward_references.c
    thirdparty/brotli/c/enc/backward_references_hq.c
    thirdparty/brotli/c/enc/bit_cost.c
    thirdparty/brotli/c/enc/block_splitter.c
    thirdparty/brotli/c/enc/brotli_bit_stream.c
    thirdparty/brotli/c/enc/cluster.c
    thirdparty/brotli/c/enc/command.c
    thirdparty/brotli/c/enc/compound_dictionary.c
    thirdparty/brotli/c/enc/compress_fragment.c
    thirdparty/brotli/c/enc/compress_fragment_two_pass.c
    thirdparty/brotli/c/enc/dictionary_hash.c
    thirdparty/brotli/c/enc/encode.c
    thirdparty/brotli/c/enc/encoder_dict.c
    thirdparty/brotli/c/enc/entropy_encode.c
    thirdparty/brotli/c/enc/fast_log.c
    thirdparty/brotli/c/enc/histogram.c
    thirdparty/brotli/c/enc/literal_cost.c
    thirdparty/brotli/c/enc/memory.c
    thirdparty/brotli/c/enc/metablock.c
    thirdparty/brotli/c/enc/static_dict.c
    thirdparty/brotli/c/enc/utf8_util.c
)
target_include_directories(mupdf_brotli PRIVATE thirdparty/brotli/c/include)

# ------------------------------------------------------------------------------
# lcms2 — color management (multi-threaded fork)
# ------------------------------------------------------------------------------
add_library(mupdf_lcms2 OBJECT
    thirdparty/lcms2/src/cmsalpha.c
    thirdparty/lcms2/src/cmscam02.c
    thirdparty/lcms2/src/cmscgats.c
    thirdparty/lcms2/src/cmscnvrt.c
    thirdparty/lcms2/src/cmserr.c
    thirdparty/lcms2/src/cmsgamma.c
    thirdparty/lcms2/src/cmsgmt.c
    thirdparty/lcms2/src/cmshalf.c
    thirdparty/lcms2/src/cmsintrp.c
    thirdparty/lcms2/src/cmsio0.c
    thirdparty/lcms2/src/cmsio1.c
    thirdparty/lcms2/src/cmslut.c
    thirdparty/lcms2/src/cmsmd5.c
    thirdparty/lcms2/src/cmsmtrx.c
    thirdparty/lcms2/src/cmsnamed.c
    thirdparty/lcms2/src/cmsopt.c
    thirdparty/lcms2/src/cmspack.c
    thirdparty/lcms2/src/cmspcs.c
    thirdparty/lcms2/src/cmsplugin.c
    thirdparty/lcms2/src/cmsps2.c
    thirdparty/lcms2/src/cmssamp.c
    thirdparty/lcms2/src/cmssm.c
    thirdparty/lcms2/src/cmstypes.c
    thirdparty/lcms2/src/cmsvirt.c
    thirdparty/lcms2/src/cmswtpnt.c
    thirdparty/lcms2/src/cmsxform.c
)
target_include_directories(mupdf_lcms2 PRIVATE thirdparty/lcms2/include)
target_compile_definitions(mupdf_lcms2 PRIVATE
    HAVE_LCMS2MT
    LCMS2MT_PREFIX=lcms2mt_
)

# JPEG XR was removed from MuPDF's bundled thirdparty in 1.27.x.
# XPS JPEG XR image support is disabled (HAVE_JPEGXR not set).

# ------------------------------------------------------------------------------
# MuJS — JavaScript engine (for PDF forms/actions)
# ------------------------------------------------------------------------------
add_library(mupdf_mujs OBJECT thirdparty/mujs/one.c)
target_include_directories(mupdf_mujs PRIVATE thirdparty/mujs)

# ------------------------------------------------------------------------------
# Extract — PDF text/structure extraction
# ------------------------------------------------------------------------------
add_library(mupdf_extract OBJECT
    thirdparty/extract/src/alloc.c
    thirdparty/extract/src/astring.c
    thirdparty/extract/src/boxer.c
    thirdparty/extract/src/buffer.c
    thirdparty/extract/src/document.c
    thirdparty/extract/src/docx.c
    thirdparty/extract/src/docx_template.c
    thirdparty/extract/src/extract.c
    thirdparty/extract/src/join.c
    thirdparty/extract/src/json.c
    thirdparty/extract/src/mem.c
    thirdparty/extract/src/html.c
    thirdparty/extract/src/odt.c
    thirdparty/extract/src/odt_template.c
    thirdparty/extract/src/outf.c
    thirdparty/extract/src/rect.c
    thirdparty/extract/src/sys.c
    thirdparty/extract/src/text.c
    thirdparty/extract/src/xml.c
    thirdparty/extract/src/zip.c
)
target_include_directories(mupdf_extract PRIVATE
    thirdparty/extract/include
    thirdparty/extract/src
    thirdparty/zlib
)

# ------------------------------------------------------------------------------
# mupdf_third — assembles all codec objects into one static library
# ------------------------------------------------------------------------------
add_library(mupdf_third STATIC
    $<TARGET_OBJECTS:mupdf_ft>
    $<TARGET_OBJECTS:mupdf_hb>
    $<TARGET_OBJECTS:mupdf_gumbo>
    $<TARGET_OBJECTS:mupdf_zlib>
    $<TARGET_OBJECTS:mupdf_jpeg>
    $<TARGET_OBJECTS:mupdf_openjpeg>
    $<TARGET_OBJECTS:mupdf_jbig2>
    $<TARGET_OBJECTS:mupdf_brotli>
    $<TARGET_OBJECTS:mupdf_lcms2>
    $<TARGET_OBJECTS:mupdf_mujs>
    $<TARGET_OBJECTS:mupdf_extract>
)

# ==============================================================================
# mupdf — core library (fitz + format handlers + embedded resources)
# ==============================================================================
file(GLOB FITZ_SRC  CONFIGURE_DEPENDS source/fitz/*.c)
file(GLOB PDF_SRC   CONFIGURE_DEPENDS source/pdf/*.c)
file(GLOB XPS_SRC   CONFIGURE_DEPENDS source/xps/*.c)
file(GLOB HTML_SRC  CONFIGURE_DEPENDS source/html/*.c)
file(GLOB SVG_SRC   CONFIGURE_DEPENDS source/svg/*.c)
file(GLOB CBZ_SRC   CONFIGURE_DEPENDS source/cbz/*.c)
file(GLOB REFLOW_SRC CONFIGURE_DEPENDS source/reflow/*.c)

# Pre-generated resource C arrays (fonts + hyphenation)
file(GLOB_RECURSE GENERATED_SRC CONFIGURE_DEPENDS generated/*.c)

add_library(mupdf STATIC
    ${FITZ_SRC}
    ${PDF_SRC}
    ${XPS_SRC}
    ${HTML_SRC}
    ${SVG_SRC}
    ${CBZ_SRC}
    ${REFLOW_SRC}
    ${GENERATED_SRC}
)

target_include_directories(mupdf SYSTEM PUBLIC include)
target_include_directories(mupdf PRIVATE
    thirdparty/freetype/include
    thirdparty/harfbuzz/src
    thirdparty/gumbo-parser/src
    thirdparty/mujs
    thirdparty/zlib
    thirdparty/libjpeg
    scripts/libjpeg
    thirdparty/openjpeg/src/lib/openjp2
    thirdparty/jbig2dec
    thirdparty/brotli/c/include
    thirdparty/lcms2/include
    thirdparty/extract/include
    thirdparty/extract/src
    generated
)

target_compile_definitions(mupdf PRIVATE
    # Barcode support removed (thirdparty/zxing-cpp and thirdparty/zint deleted)
    FZ_ENABLE_BARCODE=0
    # OCR support removed (thirdparty/tesseract and thirdparty/leptonica deleted)
    FZ_ENABLE_OCR_FALLBACK=0
    # lcms2 multi-threaded fork
    HAVE_LCMS2MT
    LCMS2MT_PREFIX=lcms2mt_
    # OpenJPEG as static lib
    OPJ_STATIC
    # Only the URW Base14 C arrays are pre-generated in generated/.
    # Noto (160 fonts), CJK (DroidSans/SourceHan), and SIL fonts require a
    # font2c code-generation step that we have not wired up; skip them for now.
    # PDFs that embed their own fonts are unaffected; standard PDF fonts fall
    # back to the URW equivalents.
    TOFU_NOTO
    TOFU_CJK
    TOFU_SIL
    TOFU_EMOJI
)

# skew.c / deskew_sse.h use SSSE3 and SSE4.1 intrinsics; safe on any modern x86-64.
# Emscripten targets wasm32 which has no x86 SIMD via -msse4.1.
if(NOT _mupdf_is_msvc AND NOT EMSCRIPTEN)
    target_compile_options(mupdf PRIVATE -msse4.1)
endif()

# mupdf_third links into mupdf transitively
target_link_libraries(mupdf PUBLIC mupdf_third)

# ==============================================================================
# Alias target
# ==============================================================================
add_library(mupdf::mupdf ALIAS mupdf)
