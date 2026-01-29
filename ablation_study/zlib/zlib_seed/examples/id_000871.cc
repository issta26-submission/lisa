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
//<ID> 871
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const char source[] = "zlib sample payload for compress2 and adler32 operations";
    const z_size_t source_len = (z_size_t)(sizeof(source) - 1);
    const Bytef *source_buf = (const Bytef *)source;
    uLong adler_src = adler32_z(0UL, source_buf, source_len);
    uLong bound = compressBound((uLong)source_len);
    uLongf dest_len = (uLongf)bound;
    Bytef *dest = new Bytef[ dest_len ? dest_len : 1u ];
    memset(dest, 0, (size_t)dest_len);

    // step 2: Setup - compress the source buffer
    int rc_compress = compress2(dest, &dest_len, source_buf, (uLong)source_len, 6);

    // step 3: Operate - compute checksum of compressed data and combine with source checksum, write to gz using fd 1
    uLong adler_comp = adler32_z(0UL, dest, (z_size_t)dest_len);
    uLong combined_adler = adler32_combine(adler_src, adler_comp, (off_t)dest_len);
    gzFile gzf = gzdopen(1, "wb");
    int rc_gz_write = gzwrite(gzf, dest, (unsigned int)dest_len);
    int rc_gz_close = gzclose(gzf);

    // step 4: Validate and Cleanup
    (void)rc_compress;
    (void)adler_src;
    (void)adler_comp;
    (void)combined_adler;
    (void)rc_gz_write;
    (void)rc_gz_close;
    delete [] dest;

    // API sequence test completed successfully
    return 66;
}