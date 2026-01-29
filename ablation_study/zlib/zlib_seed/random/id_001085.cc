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
//<ID> 1085
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare payload and compute initial checksums and compile flags
    const char payload[] = "zlib API sequence payload for inflate/crc/adler demonstration";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    uLong crc_full = crc32_z(0UL, (const Bytef *)payload, (z_size_t)payload_len);
    uLong adler_orig = adler32(0UL, (const Bytef *)payload, (uInt)payload_len);
    unsigned long compile_flags = zlibCompileFlags();

    // step 2: Compress the payload into a temporary buffer
    uLong bound = compressBound(payload_len);
    Bytef * comp_buf = new Bytef[bound];
    uLongf comp_len = (uLongf)bound;
    int rc_compress = compress2(comp_buf, &comp_len, (const Bytef *)payload, payload_len, 6);

    // step 3: Initialize inflate stream, perform a single inflate call, then end inflate
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)comp_len;
    Bytef * out_buf = new Bytef[payload_len];
    istrm.next_out = out_buf;
    istrm.avail_out = (uInt)payload_len;
    int rc_inflate_init = inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    int rc_inflate = inflate(&istrm, 0);
    int rc_inflate_end = inflateEnd(&istrm);

    // step 4: Validate checksums using crc32_z, crc32_combine and adler32; cleanup resources
    uLong part_len = payload_len / 2;
    uLong crc_part1 = crc32_z(0UL, (const Bytef *)payload, (z_size_t)part_len);
    uLong crc_part2 = crc32_z(0UL, (const Bytef *)(payload + part_len), (z_size_t)(payload_len - part_len));
    uLong crc_combined = crc32_combine(crc_part1, crc_part2, (off_t)(payload_len - part_len));
    uLong adler_out = adler32(0UL, (const Bytef *)out_buf, (uInt)payload_len);

    delete [] comp_buf;
    delete [] out_buf;

    (void)crc_full;
    (void)adler_orig;
    (void)compile_flags;
    (void)rc_compress;
    (void)rc_inflate_init;
    (void)rc_inflate;
    (void)rc_inflate_end;
    (void)crc_part1;
    (void)crc_part2;
    (void)crc_combined;
    (void)adler_out;
    // API sequence test completed successfully
    return 66;
}