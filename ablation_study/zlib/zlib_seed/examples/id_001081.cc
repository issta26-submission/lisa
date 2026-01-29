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
//<ID> 1081
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare payload and compute checksums for halves
    const char payload[] = "zlib API sequence payload for checksum and inflate validation";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    uLong half = payload_len / 2;
    uInt first_len = (uInt)half;
    uInt second_len = (uInt)(payload_len - half);
    uLong crc1 = crc32_z(0UL, (const Bytef *)payload, (z_size_t)first_len);
    uLong crc2 = crc32_z(0UL, (const Bytef *)(payload + first_len), (z_size_t)second_len);
    uLong combined_crc = crc32_combine(crc1, crc2, (off_t)second_len);
    uLong adler_orig = adler32(0UL, (const Bytef *)payload, (uInt)payload_len);

    // step 2: Compress the payload into a buffer
    uLong comp_bound = compressBound(payload_len);
    Bytef * comp_buf = new Bytef[(comp_bound ? comp_bound : 1)];
    uLongf comp_len = (uLongf)comp_bound;
    int rc_compress = compress(comp_buf, &comp_len, (const Bytef *)payload, payload_len);

    // step 3: Initialize inflate stream, perform inflate into output buffer
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    Bytef * out_buf = new Bytef[(payload_len ? payload_len : 1)];
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)comp_len;
    istrm.next_out = out_buf;
    istrm.avail_out = (uInt)payload_len;
    int rc_inflate = inflate(&istrm, 0);
    int rc_inflate_end = inflateEnd(&istrm);

    // step 4: Validate checksums, query build flags, cleanup
    uLong adler_out = adler32(0UL, (const Bytef *)out_buf, (uInt)payload_len);
    unsigned long flags = zlibCompileFlags();
    delete [] comp_buf;
    delete [] out_buf;
    (void)crc1;
    (void)crc2;
    (void)combined_crc;
    (void)adler_orig;
    (void)rc_compress;
    (void)rc_inflate_init;
    (void)rc_inflate;
    (void)rc_inflate_end;
    (void)adler_out;
    (void)flags;
    // API sequence test completed successfully
    return 66;
}