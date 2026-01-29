#include <zlib.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 528
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and size a compression buffer using compressBound, then compress the data
    const char src[] = "zlib API sequence payload: prepare -> compressBound -> compress -> gz IO -> rewind/ungetc -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong compBound = compressBound(sourceLen);
    Bytef *comp = (Bytef *)malloc((size_t)compBound);
    memset(comp, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    int rc_comp = compress(comp, &compLen, source, sourceLen);

    // step 2: Open a gzFile and write the compressed data to it using gzwrite
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf, (voidpc)comp, (unsigned int)compLen);

    // step 3: Rewind the gzFile, push back a byte with gzungetc, and query the zlib version string
    int rc_rewind = gzrewind(gf);
    int rc_ungetc = gzungetc((int)(unsigned char)src[0], gf);
    const char *version = zlibVersion();

    // step 4: Cleanup resources and close the gzFile
    int rc_gzclose = gzclose(gf);
    free(comp);

    (void)rc_comp;
    (void)rc_gzwrite;
    (void)rc_rewind;
    (void)rc_ungetc;
    (void)version;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}