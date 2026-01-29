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
//<ID> 520
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize - prepare source data and query zlib version
    const char src[] = "zlib API sequence payload: write -> rewind -> ungetc -> write again";
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();

    // step 2: Configure - compute required buffer size with compressBound and allocate buffer
    uLong compBound = compressBound(sourceLen);
    unsigned char *compBuf = (unsigned char *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    memcpy(compBuf, src, (size_t)sourceLen);

    // step 3: Operate - open a gzFile, write the prepared buffer, rewind, push back a byte, write original data
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_write1 = gzwrite(gf, (voidpc)compBuf, (unsigned int)sourceLen);
    int rc_rewind = gzrewind(gf);
    int rc_unget = gzungetc((int)(unsigned char)src[0], gf);
    int rc_write2 = gzwrite(gf, (voidpc)src, (unsigned int)sourceLen);
    int rc_close = gzclose(gf);

    // step 4: Validate & Cleanup - touch variables for completeness, free memory, finish
    (void)version;
    (void)compBound;
    (void)rc_write1;
    (void)rc_rewind;
    (void)rc_unget;
    (void)rc_write2;
    (void)rc_close;
    free(compBuf);
    // API sequence test completed successfully
    return 66;
}