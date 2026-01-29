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
//<ID> 743
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and get zlib version
    const char src[] = "zlib API sequence test payload for deflateInit2_, inflateReset, gzread and inflateEnd";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();

    // step 2: Compress source into an in-memory buffer using deflateInit2_ / deflate / deflateEnd
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_deflate_init = deflateInit2_(&dstrm, 6, 8 /* Z_DEFLATED */, 15, 8, 0 /* Z_DEFAULT_STRATEGY */, version, (int)sizeof(z_stream));
    uLong compAlloc = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compAlloc);
    memset(compBuf, 0, (size_t)compAlloc);
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)compAlloc;
    int rc_deflate = deflate(&dstrm, 4 /* Z_FINISH */);
    uLong compProduced = (uLong)dstrm.total_out;
    int rc_deflate_end = deflateEnd(&dstrm);

    // step 3: Persist original data to a gz file and read it back using gzread
    gzFile gf_w = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gz_write = gzwrite(gf_w, source, (unsigned int)sourceLen);
    int rc_gz_close_w = gzclose(gf_w);
    gzFile gf_r = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    Bytef *gzReadBuf = (Bytef *)malloc((size_t)(sourceLen + 16));
    memset(gzReadBuf, 0, (size_t)(sourceLen + 16));
    int rc_gz_read = gzread(gf_r, gzReadBuf, (unsigned int)sourceLen);
    int rc_gz_close_r = gzclose(gf_r);
    uLong crc_from_gz = crc32(0UL, gzReadBuf, (uInt)sourceLen);

    // step 4: Initialize inflate with inflateInit2_, reset it with inflateReset, inflate compressed data, then cleanup with inflateEnd
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inflate_init = inflateInit2_(&istrm, 15, version, (int)sizeof(z_stream));
    int rc_inflate_reset = inflateReset(&istrm);
    uLong outAlloc = sourceLen + 64;
    Bytef *outBuf = (Bytef *)malloc((size_t)outAlloc);
    memset(outBuf, 0, (size_t)outAlloc);
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compProduced;
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)outAlloc;
    int rc_inflate = inflate(&istrm, 0);
    uLong inflatedSize = (uLong)istrm.total_out;
    int rc_inflate_end = inflateEnd(&istrm);

    free(compBuf);
    free(gzReadBuf);
    free(outBuf);

    (void)rc_deflate_init;
    (void)rc_deflate;
    (void)rc_deflate_end;
    (void)compProduced;
    (void)rc_gz_write;
    (void)rc_gz_close_w;
    (void)rc_gz_read;
    (void)rc_gz_close_r;
    (void)crc_from_gz;
    (void)rc_inflate_init;
    (void)rc_inflate_reset;
    (void)rc_inflate;
    (void)inflatedSize;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}