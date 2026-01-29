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
//<ID> 879
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initial checksum
    const char payload[] = "example payload for zlib compress2 and adler32";
    const z_size_t payload_len = (z_size_t)(sizeof(payload) - 1);
    const Bytef *src_buf = (const Bytef *)payload;
    uLong adler_initial = adler32_z(0UL, src_buf, payload_len);

    // step 2: Prepare destination buffer and compress
    uLong bound = compressBound((uLong)payload_len);
    Bytef *dest_buf = new Bytef[(bound ? bound : 1)];
    uLongf dest_len = (uLongf)bound;
    int rc_compress = compress2(dest_buf, &dest_len, src_buf, (uLong)payload_len, 6);

    // step 3: Compute checksum of compressed data, combine, and write via gzdopen
    uLong adler_compressed = adler32_z(0UL, dest_buf, (z_size_t)dest_len);
    uLong combined_adler = adler32_combine(adler_initial, adler_compressed, (off_t)payload_len);
    gzFile gf = gzdopen(1, "wb");
    unsigned int write_len = (unsigned int)(dest_len <= (uLong)~0u ? dest_len : (uLong)~0u);
    int rc_gzwrite = gzwrite(gf, dest_buf, write_len);
    int rc_gzflush = gzflush(gf, 0);
    int rc_gzclose = gzclose(gf);

    // step 4: Cleanup and finalization
    delete [] dest_buf;
    (void)rc_compress;
    (void)adler_initial;
    (void)adler_compressed;
    (void)combined_adler;
    (void)rc_gzwrite;
    (void)rc_gzflush;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}