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
//<ID> 1625
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations (Initialize & prepare compressed data)
    const char payload[] = "zlib api payload for inflate, crc and reset2 sequence";
    const size_t payloadLen = (size_t)(sizeof(payload) - 1);
    uLong srcLen = (uLong)payloadLen;
    Bytef *src = new Bytef[payloadLen];
    memset(src, 0, payloadLen);
    memcpy(src, payload, payloadLen);
    const z_crc_t *crc_table = get_crc_table();
    uLong crc = crc32_z(0UL, src, (z_size_t)payloadLen);
    const char *version = zlibVersion();
    uLong bound = compressBound(srcLen);
    Bytef *compBuf = new Bytef[(size_t)(bound ? bound : 1)];
    memset(compBuf, 0, (size_t)(bound ? bound : 1));
    uLongf compLen = bound ? bound : 1;
    int rc_compress = compress2(compBuf, &compLen, src, srcLen, 6);

    // step 2: Setup (initialize inflate stream)
    z_stream inf;
    memset(&inf, 0, (size_t)sizeof(z_stream));
    inf.next_in = compBuf;
    inf.avail_in = (uInt)compLen;
    uLong outBufLen = srcLen + 64;
    Bytef *outBuf = new Bytef[(size_t)outBufLen];
    memset(outBuf, 0, (size_t)outBufLen);
    inf.next_out = outBuf;
    inf.avail_out = (uInt)outBufLen;
    int rc_inflate_init = inflateInit_(&inf, version, (int)sizeof(z_stream));

    // step 3: Core operations (inflate, reset window bits, inflate again)
    int rc_inflate_first = inflate(&inf, 0);
    int rc_reset2 = inflateReset2(&inf, 15);
    inf.next_in = compBuf;
    inf.avail_in = (uInt)compLen;
    inf.next_out = outBuf;
    inf.avail_out = (uInt)outBufLen;
    int rc_inflate_second = inflate(&inf, 0);

    // step 4: Validate and Cleanup
    int rc_inflate_end = inflateEnd(&inf);
    delete [] src;
    delete [] compBuf;
    delete [] outBuf;
    (void)payloadLen;
    (void)srcLen;
    (void)crc_table;
    (void)crc;
    (void)version;
    (void)bound;
    (void)compLen;
    (void)rc_compress;
    (void)rc_inflate_init;
    (void)rc_inflate_first;
    (void)rc_reset2;
    (void)rc_inflate_second;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}