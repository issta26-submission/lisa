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
//<ID> 1507
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib api sequence payload: gzopen + gzbuffer + crc32_z + gzclose";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);

    // step 2: Setup (compute checksum and derive buffer size from it)
    uLong checksum = crc32_z(0UL, src, (z_size_t)srcLen);
    unsigned int buffer_size = (unsigned int)((checksum % 4096u) + 1u);

    // step 3: Core operations (open gz file, set buffer, write payload, close)
    gzFile gzf = gzopen("test_zlib_api_sequence_output.gz", "wb");
    int rc_gzbuffer = gzbuffer(gzf, buffer_size);
    int rc_write = gzwrite(gzf, src, (unsigned int)srcLen);
    int rc_close = gzclose(gzf);

    // step 4: Cleanup and finalize
    delete [] src;
    (void)checksum;
    (void)buffer_size;
    (void)rc_gzbuffer;
    (void)rc_write;
    (void)rc_close;
    // API sequence test completed successfully
    return 66;
}