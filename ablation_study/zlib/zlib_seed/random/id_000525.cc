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
//<ID> 525
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and preparation
    const char src[] = "zlib API sequence payload: test gzwrite, gzungetc, gzrewind, zlibVersion, compressBound";
    const char *version = zlibVersion();
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong bound = compressBound(sourceLen);
    unsigned char *buffer = (unsigned char *)malloc((size_t)(bound + 16));
    memset(buffer, 0, (size_t)(bound + 16));

    // step 2: Write the source to a gz file using gzwrite
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf, (voidpc)src, (unsigned int)sourceLen);
    int rc_close_w = gzclose(gf);

    // step 3: Reopen for reading, exercise gzungetc and gzrewind, then read back into buffer
    gzFile gf2 = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    int rc_ungetc = gzungetc((int)src[0], gf2);
    int rc_rewind = gzrewind(gf2);
    int rc_gzread = gzread(gf2, (voidp)buffer, (unsigned int)sourceLen);
    int rc_close_r = gzclose(gf2);

    // step 4: Validate simple usage flow and cleanup
    (void)version;
    (void)rc_gzwrite;
    (void)rc_close_w;
    (void)rc_ungetc;
    (void)rc_rewind;
    (void)rc_gzread;
    (void)rc_close_r;
    (void)bound;
    free(buffer);
    // API sequence test completed successfully
    return 66;
}