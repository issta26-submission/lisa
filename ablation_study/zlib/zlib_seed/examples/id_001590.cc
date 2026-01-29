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
//<ID> 1590
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
    const char *version = zlibVersion();
    z_stream def_strm;
    memset(&def_strm, 0, (size_t)sizeof(z_stream));
    def_strm.next_in = src;
    def_strm.avail_in = (uInt)srcLen;
    int rc_deflate_init = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));

    // step 2: Configure (prepare output buffer and prime the deflate bit buffer)
    uLong bound = deflateBound(&def_strm, srcLen);
    Bytef *compBuf = new Bytef[(size_t)bound];
    memset(compBuf, 0, (size_t)bound);
    def_strm.next_out = compBuf;
    def_strm.avail_out = (uInt)bound;
    int rc_prime = deflatePrime(&def_strm, 3, 0x5);

    // step 3: Operate (perform deflate, compute checksums)
    int rc_deflate = deflate(&def_strm, 4); /* 4 corresponds to Z_FINISH */
    uLong compLen = def_strm.total_out;
    uLong adler = adler32_z(0UL, src, (z_size_t)payloadLen);
    uLong crc = crc32_z(0UL, src, (z_size_t)payloadLen);

    // step 4: Validate and Cleanup (write one byte to a gz file, close and free resources)
    gzFile gzf = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gzputc = gzputc(gzf, (int)src[0]);
    int rc_gzclose = gzclose(gzf);
    int rc_deflate_end = deflateEnd(&def_strm);
    delete [] src;
    delete [] compBuf;
    (void)payloadLen;
    (void)srcLen;
    (void)version;
    (void)rc_deflate_init;
    (void)bound;
    (void)rc_prime;
    (void)rc_deflate;
    (void)compLen;
    (void)adler;
    (void)crc;
    (void)rc_gzputc;
    (void)rc_gzclose;
    (void)rc_deflate_end;
    // API sequence test completed successfully
    return 66;
}