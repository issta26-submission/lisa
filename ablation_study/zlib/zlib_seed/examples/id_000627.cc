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
//<ID> 627
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char src[] = "zlib API sequence test payload";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLongf compBufSize = (uLongf)(sourceLen + 128);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBufSize);
    memset(compBuf, 0, (size_t)compBufSize);
    const char *version = zlibVersion();

    // step 2: Setup - initialize deflate stream and make a copy
    z_stream dstrm;
    z_stream dstrm_copy;
    memset(&dstrm, 0, sizeof(z_stream));
    memset(&dstrm_copy, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int rc_def_copy = deflateCopy(&dstrm_copy, &dstrm);

    // step 3: Core operations - compress data and write to a gzFile, check EOF
    uLongf compDestLen = compBufSize;
    int rc_compress = compress(compBuf, &compDestLen, source, sourceLen);
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf, compBuf, (unsigned int)compDestLen);
    int rc_gzeof = gzeof(gf);

    // step 4: Validate & Cleanup - initialize inflate (validation), then end streams and free resources
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    int rc_inflate_end = inflateEnd(&istrm);
    int rc_def_end_copy = deflateEnd(&dstrm_copy);
    int rc_def_end = deflateEnd(&dstrm);
    int rc_gzclose = gzclose(gf);
    free(compBuf);
    (void)rc_def_init;
    (void)rc_def_copy;
    (void)rc_compress;
    (void)rc_gzwrite;
    (void)rc_gzeof;
    (void)rc_inflate_init;
    (void)rc_inflate_end;
    (void)rc_def_end_copy;
    (void)rc_def_end;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}