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
//<ID> 23
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations / Initialize
    const Bytef src[] = "Example data for crc and gzip";
    z_size_t src_len = (z_size_t)(sizeof(src) - 1);
    uLong crc_seed = 0UL;
    uLong crc = crc32_z(crc_seed, src, src_len);
    gzFile gz1;
    gzFile gz2;
    int write_rc1;
    int write_rc2;
    off64_t offset1;
    int close_rc1;
    int close_rc2;

    // step 2: Setup / Configure
    gz1 = gzopen("test_zlib_api_sequence.gz", "wb");
    gz2 = gzdopen(1, "wb");

    // step 3: Operate / Validate
    write_rc1 = gzwrite(gz1, src, (unsigned)src_len);
    write_rc2 = gzwrite(gz2, src, (unsigned)src_len);
    offset1 = gzoffset64(gz1);
    gzprintf(gz2, "crc=%lu offset=%lld wrote1=%d wrote2=%d\n", (unsigned long)crc, (long long)offset1, write_rc1, write_rc2);

    // step 4: Cleanup
    close_rc1 = gzclose(gz1);
    close_rc2 = gzclose(gz2);

    // API sequence test completed successfully
    (void)close_rc1;
    (void)close_rc2;
    (void)write_rc1;
    (void)write_rc2;
    (void)offset1;
    (void)crc;

    return 66;
}