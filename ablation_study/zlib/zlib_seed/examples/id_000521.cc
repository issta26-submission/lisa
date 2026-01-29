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
//<ID> 521
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and query library/version info; size a scratch buffer with compressBound
    const char src[] = "zlib API sequence payload: write -> rewind -> ungetc -> write-scratch -> cleanup";
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong bound = compressBound(sourceLen);
    unsigned char *scratch = (unsigned char *)malloc((size_t)bound);
    memset(scratch, 0, (size_t)bound);

    // step 2: Open a gzFile for writing
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");

    // step 3: Operate: write original data, rewind to start, push back a byte, write one byte from scratch
    int rc_write1 = gzwrite(gf, (voidpc)src, (unsigned int)sourceLen);
    int rc_rewind = gzrewind(gf);
    int rc_ungetc = gzungetc('X', gf);
    int rc_write2 = gzwrite(gf, (voidpc)scratch, (unsigned int)1);

    // step 4: Cleanup and finalization
    int rc_close = gzclose(gf);
    free(scratch);
    (void)version;
    (void)bound;
    (void)rc_write1;
    (void)rc_rewind;
    (void)rc_ungetc;
    (void)rc_write2;
    (void)rc_close;
    // API sequence test completed successfully
    return 66;
}