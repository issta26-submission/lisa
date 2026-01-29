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
//<ID> 1628
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations (Initialize)
    const char payload[] = "zlib api sequence payload for inflateReset2, inflateInit_, crc32_z and get_crc_table";
    const size_t payloadLen = (size_t)(sizeof(payload) - 1);
    uLong srcLen = (uLong)payloadLen;
    Bytef *src = new Bytef[payloadLen];
    memset(src, 0, payloadLen);
    memcpy(src, payload, payloadLen);
    const char *version = zlibVersion();
    uLong crc_src = crc32_z(0, src, (z_size_t)srcLen);
    const z_crc_t *crc_table = get_crc_table();
    z_crc_t table_first = crc_table[0];
    uLong bound = compressBound(srcLen);
    Bytef *compBuf = new Bytef[(size_t)(bound ? bound : 1)];
    memset(compBuf, 0, (size_t)(bound ? bound : 1));
    uLongf compLen = bound ? bound : 1;
    int rc_compress = compress2(compBuf, &compLen, src, srcLen, 6);

    // step 2: Setup (inflate stream to decompress the compressed data)
    z_stream inf;
    memset(&inf, 0, (size_t)sizeof(z_stream));
    inf.next_in = compBuf;
    inf.avail_in = (uInt)compLen;
    uLong outBufLen = srcLen + 32;
    Bytef *outBuf = new Bytef[(size_t)outBufLen];
    memset(outBuf, 0, (size_t)outBufLen);
    inf.next_out = outBuf;
    inf.avail_out = (uInt)outBufLen;
    int rc_inflate_init = inflateInit_(&inf, version, (int)sizeof(z_stream));

    // step 3: Core operations (inflate, adjust windowBits with inflateReset2, inflate again)
    int rc_inflate_first = inflate(&inf, 0);
    int rc_inflate_reset2 = inflateReset2(&inf, 15);
    int rc_inflate_second = inflate(&inf, 0);

    // step 4: Validate and Cleanup
    uLong crc_out = crc32_z(0, outBuf, (z_size_t)srcLen);
    int rc_inflate_end = inflateEnd(&inf);
    delete [] src;
    delete [] compBuf;
    delete [] outBuf;
    (void)payloadLen;
    (void)srcLen;
    (void)version;
    (void)crc_src;
    (void)table_first;
    (void)bound;
    (void)compLen;
    (void)rc_compress;
    (void)rc_inflate_init;
    (void)rc_inflate_first;
    (void)rc_inflate_reset2;
    (void)rc_inflate_second;
    (void)crc_out;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}