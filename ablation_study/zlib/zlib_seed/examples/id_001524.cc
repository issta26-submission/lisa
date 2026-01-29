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
//<ID> 1524
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations (Initialize)
    const char payload[] = "zlib api sequence payload: gzopen + gzwrite + crc32_z + zlibVersion";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    const char *version = zlibVersion();
    uLong crc_before = crc32_z(0UL, src, (z_size_t)srcLen);

    // step 2: Configure (open gz file)
    const char *fname = "test_zlib_api_sequence_output.gz";
    gzFile gzf = gzopen(fname, "wb");

    // step 3: Operate (write payload to gz file)
    int rc_write = gzwrite(gzf, (voidpc)src, (unsigned int)srcLen);

    // step 4: Validate and Cleanup (close and free resources)
    int rc_close = gzclose(gzf);
    delete [] src;
    (void)version;
    (void)crc_before;
    (void)rc_write;
    (void)rc_close;
    // API sequence test completed successfully
    return 66;
}