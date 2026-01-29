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
//<ID> 29
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    const char src_c[] = "Zlib API test data";
    const Bytef *source = (const Bytef *)src_c;
    z_size_t src_len = (z_size_t)(sizeof(src_c) - 1);
    uLong crc = crc32_z(0UL, source, src_len);
    gzFile gz1 = gzopen("test_zlib_api_sequence.gz", "wb");
    gzFile gz2 = gzdopen(1, "wb");
    int rc_buf1 = gzbuffer(gz1, 8192);
    int rc_buf2 = gzbuffer(gz2, 8192);

    // step 2: Configure
    int wrote1 = gzwrite(gz1, source, (unsigned int)src_len);
    int wrote2 = gzwrite(gz2, source, (unsigned int)src_len);

    // step 3: Operate -> Validate
    off64_t off1 = gzoffset64(gz1);
    off64_t off2 = gzoffset64(gz2);
    int rc_printf = gzprintf(gz1, "crc=%lu off1=%lld off2=%lld\n", (unsigned long)crc, (long long)off1, (long long)off2);

    // step 4: Cleanup
    int rc_close1 = gzclose(gz1);
    int rc_close2 = gzclose(gz2);
    (void)rc_buf1; (void)rc_buf2; (void)wrote1; (void)wrote2; (void)rc_printf; (void)rc_close1; (void)rc_close2;

    // API sequence test completed successfully
    return 66;
}