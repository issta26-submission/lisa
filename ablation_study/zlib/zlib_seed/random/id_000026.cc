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
//<ID> 26
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char src[] = "Example data for zlib crc and gzip";
    const Bytef *buf = (const Bytef *)src;
    z_size_t len = (z_size_t)(sizeof(src) - 1);
    uLong crc = 0;
    off64_t off1 = 0;
    off64_t off2 = 0;
    gzFile gz1 = NULL;
    gzFile gz2 = NULL;
    int write_rc = 0;
    const char *file_name = "test_zlib_api_sequence.gz";

    // step 2: Setup
    crc = crc32_z(0UL, buf, len);
    gz1 = gzopen(file_name, "wb");
    gz2 = gzdopen(1, "wb");

    // step 3: Core operations and validation
    write_rc = gzwrite(gz1, buf, (unsigned int)len);
    off1 = gzoffset64(gz1);
    off2 = gzoffset64(gz2);
    gzprintf(gz2, "crc=%lu off1=%lld off2=%lld write_rc=%d\n", (unsigned long)crc, (long long)off1, (long long)off2, write_rc);

    // step 4: Cleanup
    gzclose(gz1);
    gzclose(gz2);

    // API sequence test completed successfully
    return 66;
}