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
//<ID> 1592
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
    uLong adler_src = adler32_z(1UL, src, (z_size_t)payloadLen);
    uLong crc_src = crc32_z(0UL, src, (z_size_t)payloadLen);

    z_stream def_strm;
    memset(&def_strm, 0, (size_t)sizeof(z_stream));
    def_strm.next_in = src;
    def_strm.avail_in = (uInt)srcLen;
    const char *version = zlibVersion();

    // step 2: Configure (initialize deflate and prepare output buffer)
    int rc_deflate_init = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    uInt outBufSize = (uInt)(payloadLen + 64);
    Bytef *compBuf = new Bytef[(size_t)outBufSize];
    memset(compBuf, 0, (size_t)outBufSize);
    def_strm.next_out = compBuf;
    def_strm.avail_out = outBufSize;

    // step 3: Operate (prime the deflate bit buffer, run a deflate call, compute checksums, write bytes to gz)
    int rc_deflate_prime = deflatePrime(&def_strm, 3, 5);
    int rc_deflate = deflate(&def_strm, 0);
    uLong compProduced = def_strm.total_out;
    uLong adler_comp = adler32_z(0UL, compBuf, (z_size_t)compProduced);
    uLong crc_comp = crc32_z(0UL, compBuf, (z_size_t)compProduced);

    const char *fname = "test_zlib_api_sequence.gz";
    gzFile gzf = gzopen(fname, "wb");
    int rc_gzputc_1 = gzputc(gzf, (int)compBuf[0]);
    int rc_gzputc_2 = gzputc(gzf, (int)src[0]);
    int rc_gzclose = gzclose(gzf);

    // step 4: Validate and Cleanup
    int rc_deflate_end = deflateEnd(&def_strm);
    delete [] src;
    delete [] compBuf;
    (void)payloadLen;
    (void)srcLen;
    (void)version;
    (void)adler_src;
    (void)crc_src;
    (void)rc_deflate_init;
    (void)rc_deflate_prime;
    (void)rc_deflate;
    (void)compProduced;
    (void)adler_comp;
    (void)crc_comp;
    (void)rc_gzputc_1;
    (void)rc_gzputc_2;
    (void)rc_gzclose;
    (void)rc_deflate_end;
    // API sequence test completed successfully
    return 66;
}