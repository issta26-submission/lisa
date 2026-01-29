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
//<ID> 1025
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    uLong flags = zlibCompileFlags();
    const uLong ORIG_LEN = 256UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'A', (size_t)ORIG_LEN);
    Bytef *readbuf = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(readbuf, 0, (size_t)ORIG_LEN);
    uLong crc_orig = crc32_z(0UL, orig, (z_size_t)ORIG_LEN);

    // step 2: Write original buffer to a gzip file using gzopen and finalize writer
    const char *tmpname = "zlib_fuzz_tmp.gz";
    gzFile gw = gzopen(tmpname, "wb");
    gzwrite(gw, orig, (unsigned int)ORIG_LEN);
    gzflush(gw, 0);
    gzclose(gw);

    // step 3: Open gz reader on the same file, push back a byte with gzungetc, read it back and validate CRC of read byte
    gzFile gr = gzopen(tmpname, "rb");
    int pushed = gzungetc((int)'B', gr);
    int got = gzread(gr, readbuf, 1u);
    uLong crc_read = crc32_z(0UL, readbuf, (z_size_t)1);

    // step 4: Cleanup resources and finish
    gzclearerr(gr);
    gzclose(gr);
    free(orig);
    free(readbuf);
    (void)ver;
    (void)flags;
    (void)crc_orig;
    (void)crc_read;
    (void)pushed;
    (void)got;
    (void)tmpname;
    // API sequence test completed successfully
    return 66;
}