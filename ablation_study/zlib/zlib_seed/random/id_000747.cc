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
//<ID> 747
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data, allocate buffers and obtain zlib version
    const char src[] = "zlib API sequence test payload for deflateInit2_, inflateReset, gzread and inflateEnd";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong compAlloc = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compAlloc);
    memset(compBuf, 0, (size_t)compAlloc);
    uLong outAlloc = sourceLen + 64;
    Bytef *outBuf = (Bytef *)malloc((size_t)outAlloc);
    memset(outBuf, 0, (size_t)outAlloc);

    // step 2: Initialize deflate stream with deflateInit2_, compress source into compBuf, then cleanup
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_deflate_init = deflateInit2_(&dstrm, 6, 8, 15, 8, 0, version, (int)sizeof(z_stream));
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)compAlloc;
    int rc_deflate_call = deflate(&dstrm, 4); // Z_FINISH == 4
    uLong compProduced = (uLong)dstrm.total_out;
    int rc_deflate_end = deflateEnd(&dstrm);

    // step 3: Persist original data to gz file, read it back using gzread
    gzFile gf_w = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf_w, source, (unsigned int)sourceLen);
    int rc_gzclose_w = gzclose(gf_w);
    gzFile gf_r = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    Bytef *gzReadBuf = (Bytef *)malloc((size_t)(sourceLen + 16));
    memset(gzReadBuf, 0, (size_t)(sourceLen + 16));
    int rc_gzread = gzread(gf_r, gzReadBuf, (unsigned int)sourceLen);
    int rc_gzclose_r = gzclose(gf_r);

    // step 4: Initialize inflate stream, reset it using inflateReset, inflate compressed buffer into outBuf, then cleanup
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inflate_init = inflateInit2_(&istrm, 15, version, (int)sizeof(z_stream));
    int rc_inflate_reset = inflateReset(&istrm);
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compProduced;
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)outAlloc;
    int rc_inflate_call = inflate(&istrm, 0);
    uLong inflatedSize = (uLong)istrm.total_out;
    int rc_inflate_end = inflateEnd(&istrm);

    // cleanup allocations
    free(compBuf);
    free(outBuf);
    free(gzReadBuf);

    (void)rc_deflate_init;
    (void)rc_deflate_call;
    (void)compProduced;
    (void)rc_deflate_end;
    (void)rc_gzwrite;
    (void)rc_gzclose_w;
    (void)rc_gzread;
    (void)rc_gzclose_r;
    (void)rc_inflate_init;
    (void)rc_inflate_reset;
    (void)rc_inflate_call;
    (void)inflatedSize;
    (void)rc_inflate_end;

    // API sequence test completed successfully
    return 66;
}