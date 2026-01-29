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
//<ID> 638
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source, buffers and compress the source into a temporary buffer
    const char src[] = "zlib API sequence test payload for gzfread + uncompress2 + inflateInit2_";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLongf compBufAlloc = (uLongf)compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBufAlloc);
    memset(compBuf, 0, (size_t)compBufAlloc);
    uLongf compDestLen = compBufAlloc;
    int rc_compress = compress2(compBuf, &compDestLen, source, sourceLen, 6);

    // step 2: Persist compressed data to a gzFile, then reopen and read it using gzfread
    gzFile gf_w = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf_w, compBuf, (unsigned int)compDestLen);
    int rc_gzclose_w = gzclose(gf_w);
    gzFile gf_r = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    Bytef *readBuf = (Bytef *)malloc((size_t)compDestLen);
    memset(readBuf, 0, (size_t)compDestLen);
    z_size_t items_read = gzfread((voidp)readBuf, (z_size_t)1, (z_size_t)compDestLen, gf_r);
    int gz_errnum = 0;
    const char *gz_errstr = gzerror(gf_r, &gz_errnum);

    // step 3: Decompress the read bytes using uncompress2 and initialize an inflate stream with inflateInit2_
    uLong compReadLen = (uLong)items_read;
    uLongf outBufAlloc = (uLongf)(sourceLen + 64);
    Bytef *outBuf = (Bytef *)malloc((size_t)outBufAlloc);
    memset(outBuf, 0, (size_t)outBufAlloc);
    uLongf outLen = outBufAlloc;
    int rc_uncompress2 = uncompress2(outBuf, &outLen, readBuf, &compReadLen);
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_inflate_init2 = inflateInit2_(&istrm, 15, version, (int)sizeof(z_stream));
    int rc_inflate_end = inflateEnd(&istrm);

    // step 4: Cleanup resources and finalize
    int rc_gzclose_r = gzclose(gf_r);
    free(compBuf);
    free(readBuf);
    free(outBuf);
    (void)rc_compress;
    (void)rc_gzwrite;
    (void)rc_gzclose_w;
    (void)items_read;
    (void)gz_errstr;
    (void)gz_errnum;
    (void)rc_uncompress2;
    (void)rc_inflate_init2;
    (void)rc_inflate_end;
    (void)rc_gzclose_r;
    // API sequence test completed successfully
    return 66;
}