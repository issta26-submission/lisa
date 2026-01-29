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
//<ID> 745
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and initialize deflate stream (deflateInit2_)
    const char src[] = "zlib API sequence test payload for deflateInit2_, inflateReset, gzread and inflateEnd";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit2_(&dstrm, 6, 8, 15, 8, 0, version, (int)sizeof(z_stream));
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    uLong compAlloc = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compAlloc);
    memset(compBuf, 0, (size_t)compAlloc);
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)compAlloc;
    int rc_deflate = deflate(&dstrm, 4);
    uLong compProduced = dstrm.total_out;

    // step 2: Persist original data to a gz file then read it back with gzread
    gzFile gf_w = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gz_write = gzwrite(gf_w, source, (unsigned int)sourceLen);
    int rc_gz_close_w = gzclose(gf_w);
    gzFile gf_r = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    Bytef *gzReadBuf = (Bytef *)malloc((size_t)(sourceLen + 16));
    memset(gzReadBuf, 0, (size_t)(sourceLen + 16));
    int rc_gz_read = gzread(gf_r, (voidp)gzReadBuf, (unsigned int)sourceLen);
    int rc_gz_close_r = gzclose(gf_r);

    // step 3: Initialize inflate stream, reset it, inflate compressed buffer into output
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    int rc_inf_reset = inflateReset(&istrm);
    Bytef *outBuf = (Bytef *)malloc((size_t)(sourceLen + 64));
    memset(outBuf, 0, (size_t)(sourceLen + 64));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compProduced;
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)(sourceLen + 64);
    int rc_inflate = inflate(&istrm, 0);
    uLong inflatedSize = istrm.total_out;

    // step 4: Cleanup resources and end streams
    int rc_def_end = deflateEnd(&dstrm);
    int rc_inf_end = inflateEnd(&istrm);
    free(compBuf);
    free(gzReadBuf);
    free(outBuf);
    (void)rc_def_init;
    (void)rc_deflate;
    (void)compProduced;
    (void)rc_gz_write;
    (void)rc_gz_close_w;
    (void)rc_gz_read;
    (void)rc_gz_close_r;
    (void)rc_inf_init;
    (void)rc_inf_reset;
    (void)rc_inflate;
    (void)inflatedSize;
    (void)rc_def_end;
    (void)rc_inf_end;
    // API sequence test completed successfully
    return 66;
}