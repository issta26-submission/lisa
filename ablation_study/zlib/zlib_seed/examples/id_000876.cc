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
//<ID> 876
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const char payload[] = "zlib example payload for compress and adler32";
    const uLong src_len = (uLong)(sizeof(payload) - 1);
    const Bytef *src_buf = (const Bytef *)payload;
    uLong adler_initial = adler32_z(0UL, src_buf, (z_size_t)src_len);

    // step 2: Compress the payload
    uLongf dest_len = (uLongf)(src_len + 128);
    Bytef *dest_buf = new Bytef[dest_len ? dest_len : 1];
    int rc_compress = compress2(dest_buf, &dest_len, src_buf, src_len, 6);
    uLong adler_compressed = adler32_z(0UL, dest_buf, (z_size_t)dest_len);

    // step 3: Combine checksums and write original payload to a gz stream on fd=1
    uLong combined_adler = adler32_combine(adler_initial, adler_compressed, (off_t)dest_len);
    gzFile gz_out = gzdopen(1, "wb");
    int rc_gz_puts = gzputs(gz_out, payload);
    int rc_gz_close = gzclose(gz_out);

    // step 4: Cleanup and finalization
    delete [] dest_buf;
    (void)rc_compress;
    (void)rc_gz_puts;
    (void)rc_gz_close;
    (void)adler_initial;
    (void)adler_compressed;
    (void)combined_adler;
    // API sequence test completed successfully
    return 66;
}