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
//<ID> 21
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize & Configure
    const char src[] = "Example data for zlib API sequence";
    const Bytef *source = (const Bytef *)src;
    z_size_t src_len = (z_size_t)(sizeof(src) - 1);
    uLong crc = 0;
    Bytef crcbuf[sizeof(uLong)];
    memset(crcbuf, 0, sizeof(crcbuf));
    gzFile gz_primary = gzopen("test_zlib_api_sequence.gz", "wb");
    gzFile gz_stdout = gzdopen(1, "wb");

    // step 2: Operate (compute CRC and write data)
    crc = crc32_z(0UL, source, src_len);
    memcpy(crcbuf, &crc, sizeof(uLong));
    (void)gzwrite(gz_primary, (voidp)source, (unsigned int)src_len);
    (void)gzwrite(gz_primary, (voidp)crcbuf, (unsigned int)sizeof(crcbuf));
    (void)gzwrite(gz_stdout, (voidp)source, (unsigned int)src_len);

    // step 3: Validate (obtain offsets to validate writes)
    off64_t offset_primary = gzoffset64(gz_primary);
    off64_t offset_stdout = gzoffset64(gz_stdout);
    Bytef offsets_buf[2 * sizeof(off64_t)];
    memset(offsets_buf, 0, sizeof(offsets_buf));
    memcpy(offsets_buf, &offset_primary, sizeof(off64_t));
    memcpy(offsets_buf + sizeof(off64_t), &offset_stdout, sizeof(off64_t));
    (void)gzwrite(gz_primary, (voidp)offsets_buf, (unsigned int)sizeof(offsets_buf));

    // step 4: Cleanup
    (void)gzclose(gz_primary);
    (void)gzclose(gz_stdout);
    // API sequence test completed successfully
    return 66;
}