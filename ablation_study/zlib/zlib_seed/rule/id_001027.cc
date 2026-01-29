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
//<ID> 1027
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const uLong ORIG_LEN = 256UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'X', (size_t)ORIG_LEN);
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);
    uLong flags = zlibCompileFlags();
    uLong crc_before = crc32_z(0L, orig, (z_size_t)ORIG_LEN);

    // step 2: Write compressed data to a gzip file using gzopen/gzclose_w and ensure file exists
    gzFile gw = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gw, orig, (unsigned int)ORIG_LEN);
    gzclose_w(gw);

    // step 3: Re-open the file via FILE*, duplicate its descriptor and use gzdopen for reading, then use gzungetc and gzread
    FILE *f = fopen("test_zlib_api_sequence.gz", "rb");
    int fd = fileno(f);
    int fd_dup = dup(fd);
    fclose(f);
    gzFile gr = gzdopen(fd_dup, "rb");
    int push_ret = gzungetc('A', gr);
    int read_ret = gzread(gr, decomp, (unsigned int)ORIG_LEN);
    uLong crc_after = crc32_z(0L, decomp, (z_size_t)(read_ret > 0 ? read_ret : 0));
    gzclose_r(gr);

    // step 4: Cleanup and finalize
    free(orig);
    free(decomp);
    (void)flags;
    (void)crc_before;
    (void)crc_after;
    (void)push_ret;
    (void)read_ret;
    // API sequence test completed successfully
    return 66;
}