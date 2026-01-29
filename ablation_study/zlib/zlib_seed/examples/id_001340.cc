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
//<ID> 1340
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib fuzz payload for api sequence using compress2, inflatePrime, gzseek64 and deflateEnd";
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

    // step 2: Setup (compress source and initialize inflate and deflate streams)
    int rc_compress = compress2(compBuf, &compLen_f, src, srcLen, 6);
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen_f;
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)srcLen;
    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    int rc_deflate_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));

    // step 3: Core operations (prime inflate, inflate, write gz, rewind and seek)
    int rc_inflate_prime = inflatePrime(&istrm, 1, 1);
    int rc_inflate = inflate(&istrm, 0);
    gzFile gzf = gzopen("zlib_api_seq_out.gz", "wb");
    int rc_gzwrite = gzwrite(gzf, (voidpc)compBuf, (unsigned int)compLen_f);
    int rc_gzrewind = gzrewind(gzf);
    off64_t seek_pos = gzseek64(gzf, (off64_t)0, 0);

    // step 4: Validate and Cleanup
    uLong crc_src = crc32_z(0UL, src, (z_size_t)srcLen);
    uLong crc_out = crc32_z(0UL, outBuf, (z_size_t)srcLen);
    int rc_inflate_end = inflateEnd(&istrm);
    int rc_deflate_end = deflateEnd(&dstrm);
    int rc_gzclose = gzclose(gzf);
    delete [] src;
    delete [] compBuf;
    delete [] outBuf;
    (void)version;
    (void)bound;
    (void)rc_compress;
    (void)rc_inflate_init;
    (void)rc_deflate_init;
    (void)rc_inflate_prime;
    (void)rc_inflate;
    (void)rc_gzwrite;
    (void)rc_gzrewind;
    (void)seek_pos;
    (void)crc_src;
    (void)crc_out;
    (void)rc_inflate_end;
    (void)rc_deflate_end;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}