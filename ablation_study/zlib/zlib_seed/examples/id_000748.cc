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
//<ID> 748
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and version info
    const char src[] = "zlib API sequence test payload for deflateInit2_, inflateReset, gzread and inflateEnd";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();

    // step 2: Initialize and run a deflate stream to produce compressed data
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_deflate_init = deflateInit2_(&dstrm, 6, Z_DEFLATED, 15, 8, 0, version, (int)sizeof(z_stream));
    uLong compAlloc = deflateBound(&dstrm, sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compAlloc);
    memset(compBuf, 0, (size_t)compAlloc);
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)compAlloc;
    int rc_deflate = deflate(&dstrm, Z_FINISH);
    uLong compLen = dstrm.total_out;
    int rc_deflate_end = deflateEnd(&dstrm);

    // step 3: Write original data to a gzFile and read it back with gzread
    gzFile gf_w = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf_w, source, (unsigned int)sourceLen);
    int rc_gzclose_w = gzclose(gf_w);
    gzFile gf_r = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    Bytef *gzReadBuf = (Bytef *)malloc((size_t)(sourceLen + 16));
    memset(gzReadBuf, 0, (size_t)(sourceLen + 16));
    int rc_gzread = gzread(gf_r, gzReadBuf, (unsigned int)sourceLen);
    int rc_gzclose_r = gzclose(gf_r);

    // step 4: Initialize inflate stream, reset it, inflate compressed buffer and cleanup
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inflate_init = inflateInit2_(&istrm, 15, version, (int)sizeof(z_stream));
    int rc_inflate_reset = inflateReset(&istrm);
    uLong outAlloc = sourceLen + 64;
    Bytef *outBuf = (Bytef *)malloc((size_t)outAlloc);
    memset(outBuf, 0, (size_t)outAlloc);
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)outAlloc;
    int rc_inflate = inflate(&istrm, Z_FINISH);
    uLong inflatedSize = istrm.total_out;
    int rc_inflate_end = inflateEnd(&istrm);

    free(compBuf);
    free(gzReadBuf);
    free(outBuf);
    (void)rc_deflate_init;
    (void)rc_deflate;
    (void)rc_deflate_end;
    (void)rc_gzwrite;
    (void)rc_gzclose_w;
    (void)rc_gzread;
    (void)rc_gzclose_r;
    (void)rc_inflate_init;
    (void)rc_inflate_reset;
    (void)rc_inflate;
    (void)inflatedSize;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}