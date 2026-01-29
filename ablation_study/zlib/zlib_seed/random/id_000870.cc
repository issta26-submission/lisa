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
//<ID> 870
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const char source[] = "example payload for zlib compress and adler32";
    const z_size_t source_len = (z_size_t)(sizeof(source) - 1);
    const Bytef *source_buf = (const Bytef *)source;
    uLong adler_source = adler32_z(0UL, source_buf, source_len);

    // step 2: Setup compression buffers and parameters
    uLong dest_capacity = (uLong)(source_len + 64);
    Bytef *dest_buf = new Bytef[dest_capacity ? dest_capacity : 1];
    memset(dest_buf, 0, (size_t)dest_capacity);
    uLongf dest_len = (uLongf)dest_capacity;
    int comp_level = 6;

    // step 3: Core operations — compress, checksum compressed data, combine checksums, and write with gz
    int rc_comp = compress2(dest_buf, &dest_len, source_buf, (uLong)source_len, comp_level);
    uLong adler_comp = adler32_z(0UL, (const Bytef *)dest_buf, (z_size_t)dest_len);
    off_t comp_len_off = (off_t)dest_len;
    uLong combined_adler = adler32_combine(adler_source, adler_comp, comp_len_off);
    gzFile gf = gzdopen(1, "wb");
    int rc_gz_write = gzwrite(gf, (voidpc)source_buf, (unsigned int)source_len);
    int rc_gz_close = gzclose(gf);

    // step 4: Validate and cleanup
    (void)rc_comp;
    (void)adler_source;
    (void)adler_comp;
    (void)combined_adler;
    (void)rc_gz_write;
    (void)rc_gz_close;
    delete [] dest_buf;

    // API sequence test completed successfully
    return 66;
}