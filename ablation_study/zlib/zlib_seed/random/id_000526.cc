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
//<ID> 526
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and query zlib version and compress bound
    const char src[] = "zlib API sequence payload: demonstrate compressBound -> compress -> gz operations";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong bound = compressBound(sourceLen);

    // step 2: Allocate buffer and compress the source into it
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress(comp, &compLen, source, sourceLen);

    // step 3: Open a gzFile, write compressed data, rewind, push back a byte, then write original data
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite1 = gzwrite(gf, (voidpc)comp, (unsigned int)compLen);
    int rc_gzrewind = gzrewind(gf);
    int rc_gzungetc = gzungetc((int)src[0], gf);
    int rc_gzwrite2 = gzwrite(gf, (voidpc)source, (unsigned int)sourceLen);
    int rc_gzclose = gzclose(gf);

    // step 4: Cleanup allocated resources and finalize
    free(comp);
    (void)version;
    (void)rc_comp;
    (void)rc_gzwrite1;
    (void)rc_gzrewind;
    (void)rc_gzungetc;
    (void)rc_gzwrite2;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}