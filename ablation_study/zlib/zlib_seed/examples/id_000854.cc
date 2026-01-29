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
//<ID> 854
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and CRC computations
    const char src[] = "zlib api sequence payload for crc32_z and crc32_combine";
    const uLong srcLen = (uLong)(sizeof(src) - 1);
    const Bytef *source = (const Bytef *)src;
    const uLong firstLen = srcLen / 2;
    const uLong secondLen = srcLen - firstLen;
    uLong crc_part1 = crc32_z(0L, source, (z_size_t)firstLen);
    uLong crc_part2 = crc32_z(0L, source + firstLen, (z_size_t)secondLen);
    uLong crc_combined = crc32_combine(crc_part1, crc_part2, (off_t)secondLen);

    // step 2: Initialize inflate stream
    z_stream strm;
    memset(&strm, 0, (size_t)sizeof(z_stream));
    strm.next_in = (Bytef *)source;
    strm.avail_in = (uInt)srcLen;
    int rc_inflate_init = inflateInit_(&strm, zlibVersion(), (int)sizeof(z_stream));

    // step 3: gzFile operations and error handling
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_printf = gzprintf(gf, "zlib api sequence output\n");
    gzclearerr(gf);
    int rc_close = gzclose(gf);

    // step 4: Finalize stream and cleanup
    strm.data_type = (int)(crc_combined & 0xFF);
    int rc_inflate_end = inflateEnd(&strm);

    (void)crc_part1;
    (void)crc_part2;
    (void)crc_combined;
    (void)rc_inflate_init;
    (void)rc_printf;
    (void)rc_close;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}