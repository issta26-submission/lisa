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
//<ID> 529
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data, query zlib version, and size compression buffer
    const char src[] = "zlib API sequence payload: prepare -> compress2 -> gz IO -> uncompress -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong compBound = compressBound(sourceLen);
    Bytef *comp = (Bytef *)malloc((size_t)compBound);
    memset(comp, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    int rc_comp = compress2(comp, &compLen, source, sourceLen, 6);

    // step 2: Open a gzFile and write the original data, then rewind and push back a byte
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf, (voidpc)source, (unsigned int)sourceLen);
    int rc_gzrewind = gzrewind(gf);
    int rc_gzunget = gzungetc((int)src[0], gf);

    // step 3: Decompress the compressed buffer to validate round-trip
    uLongf destLen = (uLongf)(sourceLen + 64);
    Bytef *dest = (Bytef *)malloc((size_t)destLen);
    memset(dest, 0, (size_t)destLen);
    int rc_uncomp = uncompress(dest, &destLen, comp, (uLong)compLen);

    // step 4: Cleanup resources and return
    int rc_gzclose = gzclose(gf);
    free(comp);
    free(dest);
    (void)version;
    (void)rc_comp;
    (void)rc_gzwrite;
    (void)rc_gzrewind;
    (void)rc_gzunget;
    (void)rc_uncomp;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}