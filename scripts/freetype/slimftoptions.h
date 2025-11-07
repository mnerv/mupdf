#ifndef SLIMFTOPTIONS_H
#define SLIMFTOPTIONS_H

/* Pull in FreeType's default option set, then strip what MuPDF doesn't need. */
#include <freetype/config/ftoption.h>

/* MuPDF bundles its own zlib (thirdparty/zlib) — disable FreeType's copy
 * to avoid duplicate symbol conflicts. */
#undef FT_CONFIG_OPTION_USE_ZLIB
#undef FT_CONFIG_OPTION_USE_BZIP2

#endif /* SLIMFTOPTIONS_H */
