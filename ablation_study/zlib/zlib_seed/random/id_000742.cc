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
//<ID> 742
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare data and initialize deflate
    const char src[] = "zlib API sequence test payload for deflateInit2_, inflateReset, gzread and inflateEnd";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong compAlloc = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compAlloc);
    memset(compBuf, 0, (size_t)compAlloc);
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)compAlloc;
    int rc_def_init = deflateInit2_(&dstrm, 6, 8, 15, 8, 0, version, (int)sizeof(z_stream));

    // step 2: Compress with deflate and finalize deflate state
    int rc_def = deflate(&dstrm, 0);
    uLong compProduced = (uLong)dstrm.total_out;
    int rc_def_end = deflateEnd(&dstrm);

    // step 3: Persist original data to a gz file and read it back using gzread
    gzFile gw = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gz_write = gzwrite(gw, source, (unsigned int)sourceLen);
    int rc_gz_close_w = gzclose(gw);
    gzFile gr = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    Bytef *gzBuf = (Bytef *)malloc((size_t)(sourceLen + 16));
    memset(gzBuf, 0, (size_t)(sourceLen + 16));
    int rc_gz_read = gzread(gr, gzBuf, (unsigned int)sourceLen);
    int rc_gz_close_r = gzclose(gr);

    // step 4: Initialize inflate stream, reset it to ensure clean state, then cleanup
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int windowBits = 15;
    int rc_inf_init = inflateInit2_(&istrm, windowBits, version, (int)sizeof(z_stream));
    int rc_inf_reset = inflateReset(&istrm);
    int rc_inf_end = inflateEnd(&istrm);

    free(compBuf);
    free(gzBuf);

    (void)rc_def_init;
    (void)rc_def;
    (void)compProduced;
    (void)rc_def_end;
    (void)rc_gz_write;
    (void)rc_gz_close_w;
    (void)rc_gz_read;
    (void)rc_gz_close_r;
    (void)rc_inf_init;
    (void)rc_inf_reset;
    (void)rc_inf_end;
    // API sequence test completed successfully
    return 66;
}