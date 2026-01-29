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
//<ID> 857
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initial CRC computation
    const char src[] = "zlib api sequence payload for gzprintf, crc32_z, inflateInit_, crc32_combine";
    const uLong srcLen = (uLong)(sizeof(src) - 1);
    const Bytef *source = (const Bytef *)src;
    uLong crc_a = crc32_z(0UL, source, (z_size_t)srcLen);
    uLong crc_b = crc32_z(crc_a, source, (z_size_t)srcLen);
    uLong combined_crc = crc32_combine(crc_a, crc_b, (off_t)srcLen);
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");

    // step 2: Write payload and computed CRC into the gz file and clear any error
    int rc_write_payload = gzprintf(gf, "%s\n", src);
    int rc_write_crc = gzprintf(gf, "combined_crc=%lu\n", combined_crc);
    gzclearerr(gf);

    // step 3: Initialize inflate stream using zlibVersion and attach input buffer
    z_stream strm;
    memset(&strm, 0, (size_t)sizeof(z_stream));
    strm.next_in = (Bytef *)source;
    strm.avail_in = (uInt)srcLen;
    int rc_inflate_init = inflateInit_(&strm, zlibVersion(), (int)sizeof(z_stream));

    // step 4: Cleanup streams and close file
    int rc_inflate_end = inflateEnd(&strm);
    int rc_close = gzclose(gf);

    (void)crc_a;
    (void)crc_b;
    (void)combined_crc;
    (void)rc_write_payload;
    (void)rc_write_crc;
    (void)rc_inflate_init;
    (void)rc_inflate_end;
    (void)rc_close;
    (void)source;

    // API sequence test completed successfully
    return 66;
}