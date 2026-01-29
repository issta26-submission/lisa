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
//<ID> 1503
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations / Initialize
    const char payload[] = "zlib api sequence payload: open gz, buffer, write, crc32_z, close";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    const z_size_t zlen = (z_size_t)srcLen;
    uLong crc = crc32_z(0UL, src, zlen);

    // step 2: Configure
    gzFile gzf = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_buf = gzbuffer(gzf, 8192);

    // step 3: Operate
    int rc_write1 = gzwrite(gzf, src, (unsigned int)srcLen);
    Bytef crcbuf[sizeof(uLong)];
    memcpy(crcbuf, &crc, sizeof(uLong));
    int rc_write2 = gzwrite(gzf, crcbuf, (unsigned int)sizeof(uLong));

    // step 4: Validate & Cleanup
    int rc_close = gzclose(gzf);
    delete [] src;
    (void)rc_buf;
    (void)rc_write1;
    (void)rc_write2;
    (void)rc_close;
    // API sequence test completed successfully
    return 66;
}