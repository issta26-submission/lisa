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
//<ID> 853
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and CRC computation
    const char src1[] = "Hello zlib";
    const char src2[] = " More data";
    uLong len1 = (uLong)(sizeof(src1) - 1);
    uLong len2 = (uLong)(sizeof(src2) - 1);
    const Bytef *buf1 = (const Bytef *)src1;
    const Bytef *buf2 = (const Bytef *)src2;
    uLong crc1 = crc32_z(0L, buf1, (z_size_t)len1);
    uLong crc2 = crc32_z(0L, buf2, (z_size_t)len2);
    uLong crc_combined = crc32_combine(crc1, crc2, (off_t)len2);

    // step 2: Open gz file and write formatted data, then clear any errors
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_printf = gzprintf(gf, "%s%s", src1, src2);
    gzclearerr(gf);

    // step 3: Initialize inflate stream using combined input
    z_stream strm;
    memset(&strm, 0, (size_t)sizeof(z_stream));
    Bytef *combined = new Bytef[len1 + len2 ? (len1 + len2) : 1];
    memcpy(combined, buf1, (size_t)len1);
    memcpy(combined + len1, buf2, (size_t)len2);
    strm.next_in = combined;
    strm.avail_in = (uInt)(len1 + len2);
    unsigned char outbuf[128];
    strm.next_out = outbuf;
    strm.avail_out = (uInt)sizeof(outbuf);
    int rc_inflate_init = inflateInit_(&strm, zlibVersion(), (int)sizeof(z_stream));
    int rc_inflate_end = inflateEnd(&strm);

    // step 4: Cleanup gz file and buffers
    int rc_close = gzclose(gf);
    delete [] combined;
    (void)crc1;
    (void)crc2;
    (void)crc_combined;
    (void)rc_printf;
    (void)rc_inflate_init;
    (void)rc_inflate_end;
    (void)rc_close;
    // API sequence test completed successfully
    return 66;
}