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
//<ID> 1621
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations (Initialize)
    const char payload[] = "zlib API sequence payload for inflate and CRC checks";
    const size_t payloadLen = (size_t)(sizeof(payload) - 1);
    uLong srcLen = (uLong)payloadLen;
    Bytef *src = new Bytef[payloadLen];
    memset(src, 0, payloadLen);
    memcpy(src, payload, payloadLen);
    const char *version = zlibVersion();
    uLong bound = compressBound(srcLen);
    Bytef *comp = new Bytef[(size_t)(bound ? bound : 1)];
    memset(comp, 0, (size_t)(bound ? bound : 1));
    uLongf compLen = bound ? bound : 1;
    int rc_compress = compress2(comp, &compLen, src, srcLen, 6);

    // step 2: Setup (inflate stream to decompress the compressed data)
    z_stream inf;
    memset(&inf, 0, (size_t)sizeof(z_stream));
    inf.next_in = comp;
    inf.avail_in = (uInt)compLen;
    uLong outBufLen = srcLen + 64;
    Bytef *out = new Bytef[(size_t)outBufLen];
    memset(out, 0, (size_t)outBufLen);
    inf.next_out = out;
    inf.avail_out = (uInt)outBufLen;
    int rc_inflate_init = inflateInit_(&inf, version, (int)sizeof(z_stream));

    // step 3: Core operations (inflate once, compute CRC and CRC table usage, reset window and inflate again)
    int rc_inflate_first = inflate(&inf, 0);
    z_size_t produced_first = (z_size_t)(outBufLen - (uLong)inf.avail_out);
    uLong crc_first = crc32_z(0UL, out, produced_first);
    const z_crc_t *crc_table = get_crc_table();
    z_crc_t crc_table_first = crc_table[0];
    int rc_inflate_reset2 = inflateReset2(&inf, 15);
    inf.next_in = comp;
    inf.avail_in = (uInt)compLen;
    inf.next_out = out;
    inf.avail_out = (uInt)outBufLen;
    int rc_inflate_second = inflate(&inf, 0);
    z_size_t produced_second = (z_size_t)(outBufLen - (uLong)inf.avail_out);
    uLong crc_second = crc32_z(0UL, out, produced_second);

    // step 4: Validate and Cleanup
    int rc_inflate_end = inflateEnd(&inf);
    delete [] src;
    delete [] comp;
    delete [] out;
    (void)payloadLen;
    (void)srcLen;
    (void)version;
    (void)bound;
    (void)compLen;
    (void)rc_compress;
    (void)rc_inflate_init;
    (void)rc_inflate_first;
    (void)produced_first;
    (void)crc_first;
    (void)crc_table_first;
    (void)rc_inflate_reset2;
    (void)rc_inflate_second;
    (void)produced_second;
    (void)crc_second;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}