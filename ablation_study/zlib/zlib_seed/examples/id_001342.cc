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
//<ID> 1342
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib api sequence payload for compress2, deflateEnd, gzseek64, inflatePrime and gzrewind";
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

    // step 2: Setup (compress with compress2, then perform a deflate stream which will be ended)
    int rc_compress = compress2(compBuf, &compLen_f, src, srcLen, 6);
    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    int rc_deflate_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    uLong def_bound = deflateBound(&dstrm, srcLen);
    Bytef *defOut = new Bytef[(size_t)def_bound];
    memset(defOut, 0, (size_t)def_bound);
    dstrm.next_in = src;
    dstrm.avail_in = (uInt)srcLen;
    dstrm.next_out = defOut;
    dstrm.avail_out = (uInt)def_bound;
    int rc_deflate = deflate(&dstrm, 4); // Z_FINISH numeric
    uLong defOutLen = def_bound - (uLong)dstrm.avail_out;
    int rc_deflate_end = deflateEnd(&dstrm);

    // step 3: Core operations (inflate with a prime, compute crc32, write via gz, seek and rewind)
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen_f;
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)srcLen;
    int rc_inflate_prime = inflatePrime(&istrm, 3, 5);
    int rc_inflate = inflate(&istrm, 0);
    uLong crc_src = crc32(0UL, src, (uInt)srcLen);
    uLong crc_out = crc32(0UL, outBuf, (uInt)srcLen);
    gzFile gzf = gzopen("zlib_api_seq_out.gz", "wb");
    int rc_gzwrite = gzwrite(gzf, (voidpc)src, (unsigned int)srcLen);
    off64_t rc_gzseek = gzseek64(gzf, (off64_t)0, 0);
    int rc_gzrewind = gzrewind(gzf);
    int rc_gzclose = gzclose(gzf);

    // step 4: Cleanup and finalization
    int rc_inflate_end = inflateEnd(&istrm);
    delete [] src;
    delete [] compBuf;
    delete [] outBuf;
    delete [] defOut;
    (void)version;
    (void)bound;
    (void)rc_compress;
    (void)rc_deflate_init;
    (void)rc_deflate;
    (void)defOutLen;
    (void)rc_deflate_end;
    (void)rc_inflate_init;
    (void)rc_inflate_prime;
    (void)rc_inflate;
    (void)crc_src;
    (void)crc_out;
    (void)rc_gzwrite;
    (void)rc_gzseek;
    (void)rc_gzrewind;
    (void)rc_gzclose;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}