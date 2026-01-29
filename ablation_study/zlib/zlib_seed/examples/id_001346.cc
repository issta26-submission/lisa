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
//<ID> 1346
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib api sequence payload: compress2 -> inflatePrime -> gzseek64/gzrewind -> deflateEnd";
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

    // step 2: Setup (compress source, initialize deflate and inflate streams)
    int rc_compress = compress2(compBuf, &compLen_f, src, srcLen, 6);
    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    int rc_deflate_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen_f;
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)srcLen;

    // step 3: Core operations (prime inflate, perform inflate, write/read gz file and seek/rewind)
    int rc_inflate_prime = inflatePrime(&istrm, 3, 1);
    int rc_inflate = inflate(&istrm, 0);
    gzFile gzw = gzopen("zlib_api_seq_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gzw, (voidpc)compBuf, (unsigned int)compLen_f);
    int rc_gzclose_w = gzclose(gzw);
    gzFile gzr = gzopen("zlib_api_seq_tmp.gz", "rb");
    int rc_gzrewind = gzrewind(gzr);
    off64_t rc_gzseek64 = gzseek64(gzr, (off64_t)0, SEEK_SET);
    int rc_gzclose_r = gzclose(gzr);

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
    (void)rc_inflate_init;
    (void)rc_inflate_prime;
    (void)rc_inflate;
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