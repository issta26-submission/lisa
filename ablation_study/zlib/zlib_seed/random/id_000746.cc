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
//<ID> 746
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and initialize deflate stream with deflateInit2_
    const char src[] = "zlib API sequence test payload for deflateInit2_, inflateReset, gzread and inflateEnd";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong compAlloc = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compAlloc);
    memset(compBuf, 0, (size_t)compAlloc);
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_deflate_init = deflateInit2_(&dstrm, 6, 8, 15, 8, 0, version, (int)sizeof(z_stream));
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)compAlloc;
    int rc_deflate_finish = deflate(&dstrm, 4);
    uLong compProduced = (uLong)dstrm.total_out;
    int rc_deflate_end = deflateEnd(&dstrm);

    // step 2: Persist original data to a gz file, then reopen and read via gzread
    gzFile gw = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gw, source, (unsigned int)sourceLen);
    int rc_gw_close = gzclose(gw);
    gzFile gr = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    Bytef *gzReadBuf = (Bytef *)malloc((size_t)(sourceLen + 16));
    memset(gzReadBuf, 0, (size_t)(sourceLen + 16));
    int rc_gzread = gzread(gr, gzReadBuf, (unsigned int)sourceLen);
    int rc_gr_close = gzclose(gr);

    // step 3: Initialize inflate stream, feed compressed data, and perform inflate
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inflate_init = inflateInit2_(&istrm, 15, version, (int)sizeof(z_stream));
    uLong outAlloc = sourceLen + 64;
    Bytef *outBuf = (Bytef *)malloc((size_t)outAlloc);
    memset(outBuf, 0, (size_t)outAlloc);
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compProduced;
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)outAlloc;
    int rc_inflate = inflate(&istrm, 0);

    // step 4: Validate/reset and cleanup resources
    int rc_inflate_reset = inflateReset(&istrm);
    int rc_inflate_end = inflateEnd(&istrm);
    free(compBuf);
    free(gzReadBuf);
    free(outBuf);
    (void)rc_deflate_init;
    (void)rc_deflate_finish;
    (void)rc_deflate_end;
    (void)rc_gzwrite;
    (void)rc_gw_close;
    (void)rc_gzread;
    (void)rc_gr_close;
    (void)rc_inflate_init;
    (void)rc_inflate;
    (void)rc_inflate_reset;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}