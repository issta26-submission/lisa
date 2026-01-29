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
//<ID> 852
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char src[] = "zlib api sequence payload to exercise crc32_z, gzprintf, inflateInit_, crc32_combine and gzclearerr";
    const uLong srcLen = (uLong)(sizeof(src) - 1);
    const Bytef *source = (const Bytef *)src;
    uLong crc_initial = crc32_z(0UL, source, (z_size_t)srcLen);

    // step 2: Setup resources
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gz_printf = gzprintf(gf, "zlib api sequence entry\n");
    gzclearerr(gf);

    // step 3: Initialize inflate and compute CRC combination
    z_stream strm;
    memset(&strm, 0, (size_t)sizeof(z_stream));
    strm.next_in = (Bytef *)source;
    strm.avail_in = (uInt)srcLen;
    int rc_inflate_init = inflateInit_(&strm, zlibVersion(), (int)sizeof(z_stream));
    uLong crc_secondary = crc32_z(crc_initial, source, (z_size_t)srcLen);
    uLong crc_combined = crc32_combine(crc_initial, crc_secondary, (off_t)srcLen);

    // step 4: Cleanup
    int rc_inflate_end = inflateEnd(&strm);
    int rc_gz_close = gzclose(gf);

    (void)crc_initial;
    (void)rc_gz_printf;
    (void)rc_inflate_init;
    (void)crc_secondary;
    (void)crc_combined;
    (void)rc_inflate_end;
    (void)rc_gz_close;

    // API sequence test completed successfully
    return 66;
}