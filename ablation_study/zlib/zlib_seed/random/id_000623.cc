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
//<ID> 623
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compress into a buffer
    const char src[] = "zlib API sequence test payload for compress() then gzwrite()";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLongf compBufCap = (uLongf)(sourceLen + 256);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBufCap);
    memset(compBuf, 0, (size_t)compBufCap);
    uLongf compLen = compBufCap;
    int rc_compress = compress(compBuf, &compLen, source, sourceLen);

    // step 2: Initialize deflate stream, copy it, and initialize an inflate stream
    z_stream dstrm;
    z_stream dstrm_copy;
    z_stream istrm;
    memset(&dstrm, 0, sizeof(z_stream));
    memset(&dstrm_copy, 0, sizeof(z_stream));
    memset(&istrm, 0, sizeof(z_stream));
    const char *ver = zlibVersion();
    int rc_def_init = deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    int rc_def_copy = deflateCopy(&dstrm_copy, &dstrm);
    int rc_inf_init = inflateInit_(&istrm, ver, (int)sizeof(z_stream));

    // step 3: Write compressed bytes to a gzFile and query EOF state
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf, compBuf, (unsigned int)compLen);
    int rc_gzeof = gzeof(gf);

    // step 4: Cleanup all zlib objects and buffers
    int rc_gzclose = gzclose(gf);
    int rc_def_end_src = deflateEnd(&dstrm);
    int rc_def_end_copy = deflateEnd(&dstrm_copy);
    int rc_inf_end = inflateEnd(&istrm);
    free(compBuf);
    (void)rc_compress;
    (void)rc_def_init;
    (void)rc_def_copy;
    (void)rc_inf_init;
    (void)rc_gzwrite;
    (void)rc_gzeof;
    (void)rc_gzclose;
    (void)rc_def_end_src;
    (void)rc_def_end_copy;
    (void)rc_inf_end;
    // API sequence test completed successfully
    return 66;
}