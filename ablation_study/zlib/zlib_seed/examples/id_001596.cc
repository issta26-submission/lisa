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
//<ID> 1596
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char payload[] = "zlib sequence payload for deflatePrime, adler32_z, crc32_z and gzputc usage";
    const size_t payloadLen = (size_t)(sizeof(payload) - 1);
    uLong srcLen = (uLong)payloadLen;
    Bytef *src = new Bytef[payloadLen];
    memset(src, 0, payloadLen);
    memcpy(src, payload, payloadLen);
    uLong adl = adler32_z(1UL, src, (z_size_t)payloadLen);
    uLong crc = crc32_z(0UL, src, (z_size_t)payloadLen);

    // step 2: Configure
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

    // step 3: Operate
    int rc_deflate_prime = deflatePrime(&def_strm, 3, 5);
    int rc_deflate = deflate(&def_strm, 0);
    def_strm.adler = adl;
    def_strm.reserved = (uLong)crc;
    const char *fname = "test_zlib_api_sequence.gz";
    gzFile gzf = gzopen(fname, "wb");
    int rc_put1 = gzputc(gzf, (int)payload[0]);
    int rc_put2 = gzputc(gzf, (int)payload[1]);
    int rc_put3 = gzputc(gzf, (int)(crc & 0xFF));
    int rc_put4 = gzputc(gzf, (int)(adl & 0xFF));

    // step 4: Validate and Cleanup
    int rc_gzclose = gzclose(gzf);
    int rc_deflate_end = deflateEnd(&def_strm);
    delete [] src;
    delete [] outBuf;
    (void)payloadLen;
    (void)srcLen;
    (void)adl;
    (void)crc;
    (void)rc_deflate_init;
    (void)rc_deflate_prime;
    (void)rc_deflate;
    (void)rc_put1;
    (void)rc_put2;
    (void)rc_put3;
    (void)rc_put4;
    (void)rc_gzclose;
    (void)rc_deflate_end;
    (void)version;
    // API sequence test completed successfully
    return 66;
}