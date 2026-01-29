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
//<ID> 1504
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib api sequence payload for gzopen + gzbuffer + crc32_z + gzclose";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *buf = new Bytef[(size_t)srcLen];
    memset(buf, 0, (size_t)srcLen);
    memcpy(buf, payload, (size_t)srcLen);
    const char *version = zlibVersion();

    // step 2: Setup (compute checksum and open gz file with buffer)
    uLong checksum = crc32_z(0UL, buf, (z_size_t)srcLen);
    gzFile gzf = gzopen("test_zlib_api_sequence_output.gz", "wb");
    int rc_buffer = gzbuffer(gzf, 16384);

    // step 3: Core operations (write payload and close gz file)
    int rc_write = gzwrite(gzf, buf, (unsigned int)srcLen);
    int rc_close = gzclose(gzf);

    // step 4: Cleanup and finalize
    delete [] buf;
    (void)version;
    (void)checksum;
    (void)rc_buffer;
    (void)rc_write;
    (void)rc_close;
    // API sequence test completed successfully
    return 66;
}