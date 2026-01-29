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
//<ID> 877
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const char payload[] = "zlib payload for compress and adler checks";
    const z_size_t payload_len = (z_size_t)(sizeof(payload) - 1);
    const Bytef *src = (const Bytef *)payload;
    uLongf dest_len = (uLongf)(payload_len + 64);
    Bytef *dest = new Bytef[dest_len ? dest_len : 1];

    // step 2: Configure and compress
    int rc_compress = compress2(dest, &dest_len, src, (uLong)payload_len, 6);
    uLong ad_original = adler32_z(1UL, src, payload_len);
    uLong ad_compressed = adler32_z(0UL, dest, (z_size_t)dest_len);

    // step 3: Operate with gzFile and combine adlers
    gzFile gf = gzdopen(1, "wb");
    int rc_gz_puts = gzputs(gf, payload);
    uLong combined_adler = adler32_combine(ad_original, ad_compressed, (off_t)dest_len);

    // step 4: Validate and cleanup
    int rc_gz_close = gzclose(gf);
    delete [] dest;
    (void)rc_compress;
    (void)ad_original;
    (void)ad_compressed;
    (void)combined_adler;
    (void)rc_gz_puts;
    (void)rc_gz_close;
    // API sequence test completed successfully
    return 66;
}