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
//<ID> 850
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initial I/O
    const char src[] = "zlib sequence payload for crc and gzprintf";
    const uLong srcLen = (uLong)(sizeof(src) - 1);
    const Bytef *source = (const Bytef *)src;
    uLong crc1 = crc32_z(0L, source, (z_size_t)srcLen);
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gz_printf = gzprintf(gf, "gzprintf wrote a fixed test message");
    gzclearerr(gf);

    // step 2: Initialize stream (Configure)
    z_stream strm;
    memset(&strm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&strm, zlibVersion(), (int)sizeof(z_stream));
    strm.next_in = (Bytef *)source;
    strm.avail_in = (uInt)srcLen;

    // step 3: Operate - compute secondary CRC and combine (Validate)
    uLong crc2 = crc32_z(1L, source, (z_size_t)srcLen);
    uLong crc_combined = crc32_combine(crc1, crc2, (off_t)srcLen);

    // step 4: Cleanup
    int rc_inflate_end = inflateEnd(&strm);
    int rc_gz_close = gzclose(gf);
    (void)rc_gz_printf;
    (void)rc_inflate_init;
    (void)crc_combined;
    (void)rc_inflate_end;
    (void)rc_gz_close;
    // API sequence test completed successfully
    return 66;
}