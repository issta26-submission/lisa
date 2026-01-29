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
//<ID> 523
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compute a safe buffer size with compressBound
    const char src[] = "zlib API sequence payload: write -> rewind -> ungetc -> write -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong bufBound = compressBound(sourceLen);
    Bytef *buf = (Bytef *)malloc((size_t)bufBound);
    memset(buf, 0, (size_t)bufBound);
    memcpy(buf, source, (size_t)sourceLen);

    // step 2: Open a gzFile and write the prepared buffer
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_write1 = gzwrite(gf, (voidpc)buf, (unsigned int)sourceLen);

    // step 3: Rewind the gzFile, push back the first character with gzungetc, then write again
    int rc_rewind = gzrewind(gf);
    int rc_unget = gzungetc((int)buf[0], gf);
    int rc_write2 = gzwrite(gf, (voidpc)source, (unsigned int)sourceLen);

    // step 4: Cleanup resources and finish
    int rc_close = gzclose(gf);
    free(buf);
    (void)version;
    (void)bufBound;
    (void)rc_write1;
    (void)rc_rewind;
    (void)rc_unget;
    (void)rc_write2;
    (void)rc_close;
    // API sequence test completed successfully
    return 66;
}