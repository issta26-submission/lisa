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
//<ID> 1529
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations (Initialize)
    const char base[] = "zlib api sequence payload: ";
    const char *version = zlibVersion();
    const size_t baseLen = (size_t)(sizeof(base) - 1);
    const size_t verLen = strlen(version);
    const size_t totalLen = baseLen + verLen;
    uLong srcLen = (uLong)totalLen;
    Bytef *src = new Bytef[totalLen];
    memset(src, 0, totalLen);
    memcpy(src, base, baseLen);
    memcpy(src + baseLen, version, verLen);
    uLong crc_before = crc32_z(0UL, src, (z_size_t)srcLen);

    // step 2: Configure (open gz file)
    const char *fname = "test_zlib_api_sequence_output.gz";
    gzFile gzf = gzopen(fname, "wb");

    // step 3: Operate (write payload to gz file)
    int written = gzwrite(gzf, src, (unsigned int)srcLen);
    int valid_write = (written == (int)srcLen);

    // step 4: Validate and Cleanup
    int rc_close = gzclose(gzf);
    delete [] src;
    (void)crc_before;
    (void)version;
    (void)valid_write;
    (void)rc_close;
    (void)written;
    // API sequence test completed successfully
    return 66;
}