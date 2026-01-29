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
//<ID> 1341
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib api sequence payload using compress2, deflateEnd, gzseek64, inflatePrime, gzrewind";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    const char *version = zlibVersion();

    uLong bound1 = compressBound(srcLen);
    Bytef *compBuf1 = new Bytef[(size_t)bound1];
    memset(compBuf1, 0, (size_t)bound1);
    uLongf compLen1_f = (uLongf)bound1;

    Bytef *outBuf = new Bytef[(size_t)srcLen];
    memset(outBuf, 0, (size_t)srcLen);

    // step 2: Setup (compress2 and initialize inflate and deflate streams)
    int rc_compress = compress2(compBuf1, &compLen1_f, src, srcLen, 6);

    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf1;
    istrm.avail_in = (uInt)compLen1_f;
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)srcLen;

    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    int rc_deflate_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    uLong bound2 = deflateBound(&dstrm, srcLen);
    Bytef *compBuf2 = new Bytef[(size_t)bound2];
    memset(compBuf2, 0, (size_t)bound2);
    dstrm.next_in = src;
    dstrm.avail_in = (uInt)srcLen;
    dstrm.next_out = compBuf2;
    dstrm.avail_out = (uInt)bound2;
    int rc_deflate_finish = deflate(&dstrm, 4); // Z_FINISH numeric
    uLong compLen2 = bound2 - (uLong)dstrm.avail_out;

    // step 3: Operate (finalize deflate, write gz, use gzrewind/gzseek64, inflatePrime and inflate)
    int rc_deflate_end = deflateEnd(&dstrm);

    gzFile gzf_w = gzopen64("zlib_api_seq_out.gz", "wb");
    int rc_gzwrite = gzwrite(gzf_w, (voidpc)compBuf2, (unsigned int)compLen2);
    int rc_gzclose_w = gzclose_w(gzf_w);

    gzFile gzf_r = gzopen64("zlib_api_seq_out.gz", "rb");
    int rc_gzrewind = gzrewind(gzf_r);
    off64_t rc_gzseek = gzseek64(gzf_r, (off64_t)0, 0); // SEEK_SET == 0

    int rc_inflate_prime = inflatePrime(&istrm, 1, 1);
    int rc_inflate = inflate(&istrm, 0);

    int rc_gzclose_r = gzclose(gzf_r);

    // step 4: Cleanup and finalization
    int rc_inflate_end = inflateEnd(&istrm);
    delete [] src;
    delete [] compBuf1;
    delete [] compBuf2;
    delete [] outBuf;

    (void)version;
    (void)bound1;
    (void)bound2;
    (void)rc_compress;
    (void)rc_inflate_init;
    (void)rc_deflate_init;
    (void)rc_deflate_finish;
    (void)rc_deflate_end;
    (void)rc_gzwrite;
    (void)rc_gzclose_w;
    (void)rc_gzrewind;
    (void)rc_gzseek;
    (void)rc_inflate_prime;
    (void)rc_inflate;
    (void)rc_gzclose_r;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}