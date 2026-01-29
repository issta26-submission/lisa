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
//<ID> 20
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char src[] = "Hello zlib world";
    const Bytef *data = (const Bytef *)src;
    z_size_t data_len = (z_size_t)(sizeof(src) - 1);
    uLong crc_init = 0;
    uLong crc = 0;
    gzFile gz1;
    gzFile gz2;
    int written1;
    int written2;
    off64_t off1;
    off64_t off2;
    uLong combined;
    int rc_close1;
    int rc_close2;

    // step 2: Setup
    crc = crc32_z(crc_init, data, data_len);
    gz1 = gzopen("test_zlib_api_sequence.gz", "wb");
    gz2 = gzdopen(1, "wb");

    // step 3: Operate -> Validate
    written1 = gzwrite(gz1, (const void *)data, (unsigned int)data_len);
    off1 = gzoffset64(gz1);
    combined = crc + (uLong)written1 + (uLong)off1;
    gzwrite(gz1, (const void *)&combined, (unsigned int)sizeof(combined));
    written2 = gzwrite(gz2, (const void *)&crc, (unsigned int)sizeof(crc));
    off2 = gzoffset64(gz2);

    // step 4: Cleanup
    rc_close1 = gzclose(gz1);
    rc_close2 = gzclose(gz2);
    (void)rc_close1;
    (void)rc_close2;

    // API sequence test completed successfully
    return 66;
}