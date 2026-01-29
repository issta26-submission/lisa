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
//<ID> 1526
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations (Initialize)
    const char payload[] = "zlib api sequence payload: include version and write via gzwrite";
    const char *version = zlibVersion();
    size_t payloadLen = (size_t)(sizeof(payload) - 1);
    size_t versionLen = strlen(version);
    uLong totalLen = (uLong)(payloadLen + 1 + versionLen);
    Bytef *buf = new Bytef[(size_t)totalLen];
    memset(buf, 0, (size_t)totalLen);
    memcpy(buf, payload, payloadLen);
    buf[payloadLen] = '|';
    memcpy(buf + payloadLen + 1, version, versionLen);
    uLong crc_before = crc32_z(0UL, buf, (z_size_t)totalLen);

    // step 2: Configure (open gz file)
    const char *fname = "test_zlib_api_sequence_output.gz";
    gzFile gzf = gzopen(fname, "wb");

    // step 3: Operate (write payload to gz file)
    int rc_write = gzwrite(gzf, (voidpc)buf, (unsigned int)totalLen);
    int valid_write = (rc_write == (int)totalLen);

    // step 4: Validate and Cleanup (close and free resources)
    int rc_close = gzclose(gzf);
    delete [] buf;
    (void)crc_before;
    (void)valid_write;
    (void)rc_write;
    (void)rc_close;
    // API sequence test completed successfully
    return 66;
}