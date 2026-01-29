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
//<ID> 27
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & initialization
    const char src[] = "Hello zlib test data";
    const Bytef *buffer = (const Bytef *)src;
    z_size_t buflen = (z_size_t)(sizeof(src) - 1);
    uLong crc = 0UL;
    gzFile gz1 = gzopen("test_zlib_api_sequence.gz", "wb");
    gzFile gz2 = gzdopen(1, "wb");
    off64_t off1 = 0;
    off64_t off2 = 0;
    int wrote1 = 0;
    int wrote2 = 0;
    int rc_flush1 = 0;
    int rc_flush2 = 0;

    // step 2: Configure -> compute checksum for the source data
    crc = crc32_z(0UL, buffer, buflen);

    // step 3: Operate -> write data to two gzip streams and obtain offsets
    wrote1 = gzwrite(gz1, buffer, (unsigned int)buflen);
    wrote2 = gzwrite(gz2, buffer, (unsigned int)buflen);
    off1 = gzoffset64(gz1);
    off2 = gzoffset64(gz2);

    // step 4: Validate -> emit summary, flush and cleanup
    gzprintf(gz1, "crc=%lu off1=%lld off2=%lld wrote1=%d wrote2=%d\n", (unsigned long)crc, (long long)off1, (long long)off2, wrote1, wrote2);
    rc_flush1 = gzflush(gz1, 1);
    rc_flush2 = gzflush(gz2, 1);
    gzclose(gz1);
    gzclose(gz2);
    (void)rc_flush1;
    (void)rc_flush2;
    return 66;
    // API sequence test completed successfully
}