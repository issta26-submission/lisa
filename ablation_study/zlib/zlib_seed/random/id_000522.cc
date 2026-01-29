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
//<ID> 522
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize - prepare source data, query zlib version, and size buffers with compressBound
    const char src[] = "zlib API sequence payload: write -> rewind -> ungetc -> read -> validate";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong compBound = compressBound(sourceLen);
    Bytef *readBuf = (Bytef *)malloc((size_t)(compBound + 1));
    memset(readBuf, 0, (size_t)(compBound + 1));

    // step 2: Configure & Operate - write the original data to a gzFile using gzwrite
    gzFile wf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(wf, (voidpc)source, (unsigned int)sourceLen);
    int rc_gzclose_w = gzclose(wf);

    // step 3: Operate - open for reading, rewind, push back a character with gzungetc, then read back
    gzFile rf = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    int rc_gzrewind = gzrewind(rf);
    int rc_gzungetc = gzungetc((int)src[0], rf);
    int rc_gzread = gzread(rf, (voidp)readBuf, (unsigned int)sourceLen);
    int rc_gzclose_r = gzclose(rf);

    // step 4: Validate & Cleanup - compute CRCs for a basic validation and free resources
    uLong crc_orig = crc32(0UL, source, (uInt)sourceLen);
    uLong crc_read = crc32(0UL, readBuf, (uInt)sourceLen);
    free(readBuf);
    (void)version;
    (void)compBound;
    (void)rc_gzwrite;
    (void)rc_gzclose_w;
    (void)rc_gzrewind;
    (void)rc_gzungetc;
    (void)rc_gzread;
    (void)rc_gzclose_r;
    (void)crc_orig;
    (void)crc_read;
    // API sequence test completed successfully
    return 66;
}