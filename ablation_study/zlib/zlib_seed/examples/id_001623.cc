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
//<ID> 1623
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations (Initialize)
    const char payload[] = "zlib api payload for inflateInit_, crc32_z, get_crc_table and inflateReset2 usage";
    const size_t payloadLen = (size_t)(sizeof(payload) - 1);
    uLong srcLen = (uLong)payloadLen;
    Bytef *src = new Bytef[payloadLen];
    memset(src, 0, payloadLen);
    memcpy(src, payload, payloadLen);
    const char *version = zlibVersion();
    uLong bound = compressBound(srcLen);
    uLongf compLen = bound ? bound : 1;
    Bytef *compBuf = new Bytef[(size_t)(bound ? bound : 1)];
    memset(compBuf, 0, (size_t)(bound ? bound : 1));

    // step 2: Setup (compress to produce valid compressed data; init inflate)
    int rc_compress = compress2(compBuf, &compLen, src, srcLen, 6);
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

    // step 3: Core operations (inflate, then reset windowBits and inflate again; use crc table and crc calculation)
    int rc_inflate_first = inflate(&inf, 0);
    int rc_inflate_reset2 = inflateReset2(&inf, 15);
    int rc_inflate_second = inflate(&inf, 0);
    const z_crc_t *crc_tbl = get_crc_table();
    z_crc_t crc_first_entry = crc_tbl[0];
    uLong computed_crc = crc32_z(0, outBuf, (z_size_t)payloadLen);
    (void)crc_first_entry;

    // step 4: Validate and Cleanup
    int rc_inflate_end = inflateEnd(&inf);
    delete [] src;
    delete [] compBuf;
    delete [] outBuf;
    (void)payloadLen;
    (void)srcLen;
    (void)version;
    (void)bound;
    (void)compLen;
    (void)rc_compress;
    (void)rc_inflate_init;
    (void)rc_inflate_first;
    (void)rc_inflate_reset2;
    (void)rc_inflate_second;
    (void)computed_crc;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}