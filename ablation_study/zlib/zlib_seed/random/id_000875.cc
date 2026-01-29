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
//<ID> 875
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const char payload[] = "zlib example payload for compress2 and adler32";
    const uLong sourceLen = (uLong)(sizeof(payload) - 1);
    const Bytef * src_buf = (const Bytef *)payload;
    uLong adler_src = adler32_z(0UL, src_buf, (z_size_t)sourceLen);

    uLongf dest_capacity = (uLongf)(sourceLen + 1024);
    Bytef * dest_buf = new Bytef[dest_capacity ? dest_capacity : 1];
    uLongf dest_len = dest_capacity;

    // step 2: Compress the source into dest and compute adler of compressed data
    int rc_compress = compress2(dest_buf, &dest_len, src_buf, sourceLen, 6);
    uLong adler_comp = adler32_z(0UL, dest_buf, (z_size_t)dest_len);

    // step 3: Write compressed data to a gz stream opened from fd 1 and combine checksums
    gzFile gf = gzdopen(1, "wb");
    int rc_gz_write = gzwrite(gf, (voidpc)dest_buf, (unsigned int)dest_len);
    int rc_gz_flush = gzflush(gf, 2);
    uLong combined_adler = adler32_combine(adler_src, adler_comp, (off_t)dest_len);

    // step 4: Cleanup and validation
    int rc_gz_close = gzclose(gf);
    delete [] dest_buf;
    (void)rc_compress;
    (void)rc_gz_write;
    (void)rc_gz_flush;
    (void)rc_gz_close;
    (void)adler_src;
    (void)adler_comp;
    (void)combined_adler;
    // API sequence test completed successfully
    return 66;
}