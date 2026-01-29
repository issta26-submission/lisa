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
//<ID> 872
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const char payload[] = "zlib payload for compress2 and adler32";
    const z_size_t payload_len = (z_size_t)(sizeof(payload) - 1);
    const Bytef *src = (const Bytef *)payload;
    uLong adler_src = adler32_z(0UL, src, payload_len);
    uLong bound = compressBound((uLong)payload_len);
    uLongf destLen = bound ? bound : 1UL;
    Bytef *dest = new Bytef[(size_t)destLen];

    // step 2: Compress the source buffer into dest
    int rc_compress = compress2(dest, &destLen, src, (uLong)payload_len, 6);

    // step 3: Compute adler of compressed data, combine, and write via gzdopen
    uLong adler_comp = adler32_z(0UL, dest, (z_size_t)destLen);
    uLong combined = adler32_combine(adler_src, adler_comp, (off_t)destLen);
    gzFile gf = gzdopen(1, "wb");
    int rc_gz_write = gzwrite(gf, (voidpc)dest, (unsigned int)destLen);
    int rc_gz_close = gzclose(gf);

    // step 4: Validate and cleanup
    (void)rc_compress;
    (void)rc_gz_write;
    (void)rc_gz_close;
    (void)adler_src;
    (void)adler_comp;
    (void)combined;
    delete [] dest;
    // API sequence test completed successfully
    return 66;
}