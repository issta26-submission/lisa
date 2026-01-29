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
//<ID> 1344
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib api sequence payload for compress2, inflatePrime, gzrewind and gzseek64";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    const char *version = zlibVersion();
    uLong bound = compressBound(srcLen);
    Bytef *compBuf = new Bytef[(size_t)bound];
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen_f = (uLongf)bound;
    Bytef *outBuf = new Bytef[(size_t)srcLen];
    memset(outBuf, 0, (size_t)srcLen);
    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));

    // step 2: Setup (compress source, init deflate and inflate streams)
    int rc_compress = compress2(compBuf, &compLen_f, src, srcLen, 6);
    int rc_deflate_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    uLong est_def_bound = deflateBound(&dstrm, srcLen);
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen_f;
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)srcLen;

    // step 3: Operate (prime inflate bits, inflate, write/read gzip file, seek/rewind)
    int rc_inflate_prime = inflatePrime(&istrm, 1, 0);
    int rc_inflate = inflate(&istrm, 0);
    uLong crc_src = crc32_z(0UL, src, (z_size_t)srcLen);
    uLong crc_out = crc32_z(0UL, outBuf, (z_size_t)srcLen);
    gzFile gzf_w = gzopen("zlib_api_seq_tmp.gz", "wb");
    int rc_gzwrite = 0;
    if (gzf_w) rc_gzwrite = gzwrite(gzf_w, (voidpc)compBuf, (unsigned int)compLen_f);
    int rc_gzclose_w = 0;
    if (gzf_w) rc_gzclose_w = gzclose(gzf_w);
    gzFile gzf_r = gzopen("zlib_api_seq_tmp.gz", "rb");
    int rc_gzrewind = 0;
    off64_t rc_gzseek64 = -1;
    if (gzf_r) rc_gzrewind = gzrewind(gzf_r);
    if (gzf_r) rc_gzseek64 = gzseek64(gzf_r, (off64_t)0, 0);
    int rc_gzclose_r = 0;
    if (gzf_r) rc_gzclose_r = gzclose(gzf_r);

    // step 4: Cleanup and finalization
    int rc_deflate_end = deflateEnd(&dstrm);
    int rc_inflate_end = inflateEnd(&istrm);
    delete [] src;
    delete [] compBuf;
    delete [] outBuf;
    (void)version;
    (void)bound;
    (void)rc_compress;
    (void)rc_deflate_init;
    (void)est_def_bound;
    (void)rc_inflate_init;
    (void)rc_inflate_prime;
    (void)rc_inflate;
    (void)crc_src;
    (void)crc_out;
    (void)rc_gzwrite;
    (void)rc_gzclose_w;
    (void)rc_gzrewind;
    (void)rc_gzseek64;
    (void)rc_gzclose_r;
    (void)rc_deflate_end;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}