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
//<ID> 1024
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const uLong ORIG_LEN = 512UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'Z', (size_t)ORIG_LEN);
    uLong flags = zlibCompileFlags();
    uLong crc_orig = crc32_z(0L, (const Bytef *)orig, (z_size_t)ORIG_LEN);

    // step 2: Write original data to a gzip file using gzopen/gzwrite and finalize writer
    gzFile gw = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gw, orig, (unsigned int)ORIG_LEN);
    gzflush(gw, 0);
    gzclose_w(gw);

    // step 3: Open the gzip file via file descriptor and use gzdopen + gzungetc, then read some bytes
    int fd = open("test_zlib_api_sequence.gz", 0);
    gzFile gr = gzdopen(fd, "rb");
    int pushed = gzungetc((int)orig[0], gr);
    char buf[128];
    memset(buf, 0, sizeof(buf));
    int got = gzread(gr, buf, (unsigned int)sizeof(buf));
    uLong crc_buf = crc32_z(0L, (const Bytef *)buf, (z_size_t)got);
    gzclose(gr);

    // step 4: Cleanup all resources and finalize
    free(orig);
    (void)flags;
    (void)crc_orig;
    (void)crc_buf;
    (void)got;
    (void)pushed;
    // API sequence test completed successfully
    return 66;
}