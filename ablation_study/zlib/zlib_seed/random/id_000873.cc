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
//<ID> 873
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const char payload[] = "example zlib payload for compress2 and adler checks";
    const z_size_t payload_len = (z_size_t)(sizeof(payload) - 1);
    const Bytef *source_buf = (const Bytef *)payload;
    uLong source_len = (uLong)payload_len;
    uLongf dest_len = (uLongf)(source_len + 1024UL);
    Bytef *dest_buf = new Bytef[(size_t)dest_len];
    memset(dest_buf, 0, (size_t)dest_len);

    // step 2: Configure and compress
    int rc_compress = compress2(dest_buf, &dest_len, source_buf, source_len, 6);

    // step 3: Compute adler checksums, combine and write compressed data to a gz stream
    uLong adler_src = adler32_z(1UL, source_buf, (z_size_t)source_len);
    uLong adler_dest = adler32_z(1UL, dest_buf, (z_size_t)dest_len);
    uLong combined_adler = adler32_combine(adler_src, adler_dest, (off_t)dest_len);
    gzFile gf = gzdopen(1, "wb");
    int rc_gz_write = gzwrite(gf, (voidpc)dest_buf, (unsigned int)dest_len);

    // step 4: Validate and cleanup
    int rc_gz_close = gzclose(gf);
    delete [] dest_buf;
    (void)rc_compress;
    (void)rc_gz_write;
    (void)rc_gz_close;
    (void)adler_src;
    (void)adler_dest;
    (void)combined_adler;
    // API sequence test completed successfully
    return 66;
}