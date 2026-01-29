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
//<ID> 1595
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations (Initialize)
    const char payload[] = "zlib api sequence payload for deflatePrime, adler32_z, crc32_z, gzputc and gzclose";
    const size_t payloadLen = (size_t)(sizeof(payload) - 1);
    uLong srcLen = (uLong)payloadLen;
    Bytef *src = new Bytef[payloadLen];
    memset(src, 0, payloadLen);
    memcpy(src, payload, payloadLen);
    uLong adler = adler32_z(1UL, src, (z_size_t)payloadLen);
    uLong crc = crc32_z(0UL, src, (z_size_t)payloadLen);

    // determine compressed buffer size and compress
    uLong bound = compressBound(srcLen);
    Bytef *compBuf = new Bytef[(size_t)bound];
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = bound;
    int rc_compress = compress2(compBuf, &compLen, src, srcLen, 6);

    // step 2: Configure (prepare gz output and write a byte)
    const char *fname = "test_zlib_api_sequence.gz";
    gzFile gzf = gzopen(fname, "wb");
    int rc_gzputc = gzputc(gzf, compLen > 0 ? (int)compBuf[0] : 0);
    (void)rc_gzputc;

    // step 3: Operate (initialize a deflate stream, prime it, then finalize)
    z_stream def_strm;
    memset(&def_strm, 0, (size_t)sizeof(z_stream));
    def_strm.next_in = src;
    def_strm.avail_in = (uInt)srcLen;
    Bytef *outBuf = new Bytef[payloadLen + 64];
    memset(outBuf, 0, payloadLen + 64);
    def_strm.next_out = outBuf;
    def_strm.avail_out = (uInt)(payloadLen + 64);
    const char *version = zlibVersion();
    int rc_deflate_init = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    int rc_deflate_prime = deflatePrime(&def_strm, 3, 5);
    int rc_deflate_end = deflateEnd(&def_strm);
    (void)rc_deflate_init;
    (void)rc_deflate_prime;
    (void)rc_deflate_end;

    // step 4: Validate and Cleanup
    int rc_gzclose = gzclose(gzf);
    delete [] src;
    delete [] compBuf;
    delete [] outBuf;
    (void)payloadLen;
    (void)srcLen;
    (void)bound;
    (void)compLen;
    (void)rc_compress;
    (void)adler;
    (void)crc;
    (void)version;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}