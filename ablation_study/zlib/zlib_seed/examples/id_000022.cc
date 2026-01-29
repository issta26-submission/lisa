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
//<ID> 22
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    const char data[] = "Example zlib data";
    const Bytef *buf = (const Bytef *)data;
    uLong dataLen = (uLong)(sizeof(data) - 1);
    Bytef scratch[64];
    memset(scratch, 0, sizeof(scratch));
    uLong checksum = crc32_z(0UL, buf, (z_size_t)dataLen);
    gzFile gz1 = gzopen("test_zlib_api_sequence.gz", "wb");
    gzFile gz2 = gzdopen(1, "wb");
    int rc_write1;
    int rc_write2;
    off64_t off1;
    off64_t off2;

    // step 2: Configure
    gzbuffer(gz1, 8192);
    gzsetparams(gz1, 6, 0);
    gzbuffer(gz2, 4096);

    // step 3: Operate
    rc_write1 = gzwrite(gz1, buf, (unsigned int)dataLen);
    rc_write2 = gzwrite(gz2, buf, (unsigned int)dataLen);
    gzprintf(gz1, " checksum=%lu\n", (unsigned long)checksum);

    // step 4: Validate & Cleanup
    off1 = gzoffset64(gz1);
    off2 = gzoffset64(gz2);
    (void)off1;
    (void)off2;
    gzclose(gz1);
    gzclose(gz2);

    // API sequence test completed successfully
    return 66;
}