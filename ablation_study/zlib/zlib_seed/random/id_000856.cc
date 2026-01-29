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
//<ID> 856
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and CRC calculations
    const char src[] = "zlib api payload part A";
    const char extra[] = " and payload part B";
    const uLong len_src = (uLong)(sizeof(src) - 1);
    const uLong len_extra = (uLong)(sizeof(extra) - 1);
    const Bytef *src_buf = (const Bytef *)src;
    const Bytef *extra_buf = (const Bytef *)extra;
    uLong crc_src = crc32_z(0L, src_buf, (z_size_t)len_src);
    uLong crc_extra = crc32_z(0L, extra_buf, (z_size_t)len_extra);

    // step 2: Combine CRCs and write a summary to a gz file
    uLong crc_combined = crc32_combine(crc_src, crc_extra, (off_t)len_extra);
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    gzprintf(gf, "%lu %lu %lu\n", (unsigned long)crc_src, (unsigned long)crc_extra, (unsigned long)crc_combined);
    gzclearerr(gf);

    // step 3: Initialize and configure an inflate stream using source data
    z_stream strm;
    memset(&strm, 0, (size_t)sizeof(z_stream));
    strm.next_in = (Bytef *)src_buf;
    strm.avail_in = (uInt)len_src;
    int rc_inflate_init = inflateInit_(&strm, zlibVersion(), (int)sizeof(z_stream));
    strm.data_type = (int)(crc_combined & 3UL); // use combined CRC bits to influence stream data_type

    // step 4: Cleanup and finalize
    int rc_inflate_end = inflateEnd(&strm);
    int rc_gz_close = gzclose(gf);
    (void)rc_inflate_init;
    (void)rc_inflate_end;
    (void)rc_gz_close;
    // API sequence test completed successfully
    return 66;
}