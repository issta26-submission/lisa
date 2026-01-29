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
//<ID> 363
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and prepare payload and version
    const char payload[] = "zlib API sequence payload: compress2 -> crc32_z -> deflate -> gzclose";
    const Bytef *source = (const Bytef *)payload;
    uLong sourceLen = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();
    uLong compBound = compressBound(sourceLen);
    uLongf compLen = (uLongf)compBound;
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);

    // step 2: Compress source with compress2 and compute crc32 of original data
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);
    uLong crc_orig = crc32_z(0UL, source, (z_size_t)sourceLen);

    // step 3: Initialize a deflate stream, compute an upper bound for further deflation, then deflate
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    dstrm.next_in = compBuf;
    dstrm.avail_in = (uInt)compLen;
    uLong outBound = deflateBound(&dstrm, compLen);
    Bytef *outBuf = (Bytef *)malloc((size_t)outBound);
    memset(outBuf, 0, (size_t)outBound);
    dstrm.next_out = outBuf;
    dstrm.avail_out = (uInt)outBound;
    int rc_deflate = deflate(&dstrm, 0);
    int rc_def_end = deflateEnd(&dstrm);

    // step 4: Write compressed data to a gzip file and close it, then cleanup buffers
    const char *fname = "test_zlib_api_sequence.gz";
    gzFile gzf = gzopen(fname, "wb");
    int rc_gzwrite = gzwrite(gzf, (voidpc)compBuf, (unsigned int)compLen);
    int rc_gzclose = gzclose(gzf);
    free(compBuf);
    free(outBuf);

    (void)version;
    (void)rc_comp;
    (void)crc_orig;
    (void)rc_def_init;
    (void)rc_deflate;
    (void)rc_def_end;
    (void)rc_gzwrite;
    (void)rc_gzclose;

    // API sequence test completed successfully
    return 66;
}