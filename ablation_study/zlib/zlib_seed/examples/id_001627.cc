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
//<ID> 1627
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
    uLong crc = crc32_z(0UL, src, (z_size_t)srcLen);
    const z_crc_t *crc_table = get_crc_table();
    z_crc_t first_table_entry = crc_table ? crc_table[0] : (z_crc_t)0;

    uLong bound = compressBound(srcLen);
    Bytef *compBuf = new Bytef[(size_t)(bound ? bound : 1)];
    memset(compBuf, 0, (size_t)(bound ? bound : 1));
    uLongf compLen = bound ? bound : 1;
    int rc_compress = compress2(compBuf, &compLen, src, srcLen, 6);

    // step 2: Setup (initialize inflate stream)
    z_stream infstrm;
    memset(&infstrm, 0, (size_t)sizeof(z_stream));
    infstrm.next_in = compBuf;
    infstrm.avail_in = (uInt)compLen;
    uLong outBufLen = srcLen + 64;
    Bytef *outBuf = new Bytef[(size_t)outBufLen];
    memset(outBuf, 0, (size_t)outBufLen);
    infstrm.next_out = outBuf;
    infstrm.avail_out = (uInt)outBufLen;
    int rc_inflate_init = inflateInit_(&infstrm, version, (int)sizeof(z_stream));

    // step 3: Core operations (inflate, reset window bits, re-inflate)
    int rc_inflate_first = inflate(&infstrm, 0);
    int rc_inflate_reset2 = inflateReset2(&infstrm, 15);
    infstrm.next_in = compBuf;
    infstrm.avail_in = (uInt)compLen;
    infstrm.next_out = outBuf;
    infstrm.avail_out = (uInt)outBufLen;
    int rc_inflate_second = inflate(&infstrm, 0);
    int rc_inflate_end = inflateEnd(&infstrm);

    // step 4: Validate and Cleanup
    delete [] src;
    delete [] compBuf;
    delete [] outBuf;
    (void)payloadLen;
    (void)srcLen;
    (void)version;
    (void)crc;
    (void)first_table_entry;
    (void)bound;
    (void)compLen;
    (void)rc_compress;
    (void)rc_inflate_init;
    (void)rc_inflate_first;
    (void)rc_inflate_reset2;
    (void)rc_inflate_second;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}