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
//<ID> 524
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and source preparation
    const char src[] = "zlib API sequence payload: prepare -> compressBound -> compress -> gz IO -> rewind/ungetc -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();

    // step 2: Determine compression bound and compress the source into a buffer
    uLong bound = compressBound(sourceLen);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress(comp, &compLen, source, sourceLen);

    // step 3: Write original uncompressed data to a gzFile, close, reopen for reading, push back a byte, and rewind
    const char *fname = "test_zlib_api_sequence_tmp.gz";
    gzFile gw = gzopen(fname, "wb");
    int rc_gzwrite = gzwrite(gw, (voidpc)source, (unsigned int)sourceLen);
    int rc_gwclose = gzclose(gw);
    gzFile gr = gzopen(fname, "rb");
    int first_byte = (int)comp[0];
    int rc_ungetc = gzungetc(first_byte, gr);
    int rc_rewind = gzrewind(gr);
    int rc_grclose = gzclose(gr);

    // step 4: Cleanup and finalization
    free(comp);
    (void)version;
    (void)rc_comp;
    (void)rc_gzwrite;
    (void)rc_gwclose;
    (void)rc_ungetc;
    (void)rc_rewind;
    (void)rc_grclose;
    (void)first_byte;
    (void)bound;
    (void)compLen;
    (void)sourceLen;
    // API sequence test completed successfully
    return 66;
}