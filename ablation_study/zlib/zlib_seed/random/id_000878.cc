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
//<ID> 878
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const char payload[] = "Example payload for compress2 and adler32_z usage";
    const z_size_t payload_len = (z_size_t)(sizeof(payload) - 1);
    const Bytef *src_buf = (const Bytef *)payload;
    uLong adler_src = adler32_z(0UL, src_buf, payload_len);

    // step 2: Setup compression buffers and perform compress2
    uLong bound = compressBound((uLong)payload_len);
    Bytef *comp_buf = new Bytef[bound ? bound : 1];
    uLongf comp_len = (uLongf)bound;
    int rc_compress = compress2(comp_buf, &comp_len, src_buf, (uLong)payload_len, 6);

    // step 3: Compute adler on compressed output and combine with source adler
    uLong adler_comp = adler32_z(0UL, comp_buf, (z_size_t)comp_len);
    uLong adler_combined = adler32_combine(adler_src, adler_comp, (off_t)comp_len);

    // step 4: Write compressed data to a gz stream (wrapping stdout) and cleanup
    gzFile gf = gzdopen(1, "wb");
    int wrote = gzwrite(gf, (voidpc)comp_buf, (unsigned int)comp_len);
    int rc_flush = gzflush(gf,  Z_FINISH ? Z_FINISH : 2); // use numeric fallback if Z_FINISH not available
    int rc_gz_close = gzclose(gf);
    delete [] comp_buf;
    (void)rc_compress;
    (void)adler_src;
    (void)adler_comp;
    (void)adler_combined;
    (void)wrote;
    (void)rc_flush;
    (void)rc_gz_close;
    // API sequence test completed successfully
    return 66;
}