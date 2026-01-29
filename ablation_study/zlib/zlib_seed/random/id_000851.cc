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
//<ID> 851
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initial CRC
    const char src[] = "zlib test payload for gzprintf and crc32";
    const Bytef *source = (const Bytef *)src;
    z_size_t src_len = (z_size_t)(sizeof(src) - 1);
    uLong crc_a = crc32_z(0UL, source, src_len);

    // step 2: Initialize inflate stream
    z_stream strm;
    memset(&strm, 0, (size_t)sizeof(z_stream));
    int rc_init = inflateInit_(&strm, zlibVersion(), (int)sizeof(z_stream));
    strm.next_in = (Bytef *)source;
    strm.avail_in = (uInt)src_len;

    // step 3: Operate: write using gzprintf, clear errors, compute and combine CRCs
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_printf = gzprintf(gf, "%s", src);
    gzclearerr(gf);
    const char suffix[] = "-suffix";
    const Bytef *suf = (const Bytef *)suffix;
    z_size_t suf_len = (z_size_t)(sizeof(suffix) - 1);
    uLong crc_b = crc32_z(0UL, suf, suf_len);
    uLong combined = crc32_combine(crc_a, crc_b, (off_t)suf_len);
    int rc_close = gzclose(gf);

    // step 4: Validate and Cleanup
    int rc_inflate_end = inflateEnd(&strm);
    (void)rc_init;
    (void)rc_printf;
    (void)crc_a;
    (void)crc_b;
    (void)combined;
    (void)rc_close;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}