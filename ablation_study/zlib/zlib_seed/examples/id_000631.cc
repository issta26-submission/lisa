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
//<ID> 631
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source, buffers and query version
    const char src[] = "zlib API sequence payload for testing";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLongf compBufAlloc = (uLongf)(sourceLen + 128);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBufAlloc);
    memset(compBuf, 0, (size_t)compBufAlloc);
    uLongf compDestLen = compBufAlloc;
    const char *version = zlibVersion();

    // step 2: One-shot compress and initialize an inflate stream (for API coverage)
    int rc_compress = compress2(compBuf, &compDestLen, source, sourceLen, 6);
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inflate_init2 = inflateInit2_(&istrm, 15, version, (int)sizeof(z_stream));

    // step 3: Create a gz file by writing the original source, then read it back with gzfread and query gzerror
    gzFile gf_w = gzopen("test_zlib_api_sequence_temp.gz", "wb");
    int rc_gzwrite = gzwrite(gf_w, source, (unsigned int)sourceLen);
    int rc_gzclose_w = gzclose_w(gf_w);
    gzFile gf_r = gzopen("test_zlib_api_sequence_temp.gz", "rb");
    Bytef *gzReadBuf = (Bytef *)malloc((size_t)(sourceLen + 1));
    memset(gzReadBuf, 0, (size_t)(sourceLen + 1));
    z_size_t items_read = gzfread((voidp)gzReadBuf, 1, (z_size_t)sourceLen, gf_r);
    int gz_errnum = 0;
    const char *gz_errstr = gzerror(gf_r, &gz_errnum);

    // step 4: Decompress the one-shot compressed buffer using uncompress2, cleanup streams and buffers
    uLongf outAlloc = (uLongf)(sourceLen + 64);
    Bytef *outBuf = (Bytef *)malloc((size_t)outAlloc);
    memset(outBuf, 0, (size_t)outAlloc);
    uLongf destLen = outAlloc;
    uLong compSrcLen = (uLong)compDestLen;
    int rc_uncompress2 = uncompress2(outBuf, &destLen, compBuf, &compSrcLen);
    int rc_gzclose_r = gzclose_r(gf_r);
    int rc_inflate_end = inflateEnd(&istrm);
    free(compBuf);
    free(gzReadBuf);
    free(outBuf);
    (void)rc_compress;
    (void)rc_inflate_init2;
    (void)rc_gzwrite;
    (void)rc_gzclose_w;
    (void)items_read;
    (void)gz_errnum;
    (void)gz_errstr;
    (void)rc_uncompress2;
    (void)rc_gzclose_r;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}