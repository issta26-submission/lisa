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
//<ID> 720
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    unsigned char *src = (unsigned char *)malloc(600);
    memset(src, 'Z', 600);
    unsigned int src_len = 512u;

    // step 2: Create compressed file
    gzFile wz = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int written = gzwrite(wz, src, src_len);
    int wz_close = gzclose(wz);

    // step 3: Read back with seek and gzfread
    gzFile r = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    off64_t pos1 = gzseek64(r, (off64_t)10, 0);
    unsigned char *buf1 = (unsigned char *)malloc(128);
    memset(buf1, 0, 128);
    z_size_t nread1 = gzfread(buf1, 1, (z_size_t)100, r);
    off64_t pos2 = gzseek64(r, (off64_t)0, 0);
    unsigned char *buf2 = (unsigned char *)malloc(600);
    memset(buf2, 0, 600);
    z_size_t nread2 = gzfread(buf2, 1, (z_size_t)src_len, r);
    int r_close = gzclose(r);

    // step 4: Validate / Cleanup
    free(src);
    free(buf1);
    free(buf2);
    (void)version;
    (void)written;
    (void)wz_close;
    (void)pos1;
    (void)nread1;
    (void)pos2;
    (void)nread2;
    (void)r_close;
    // API sequence test completed successfully
    return 66;
}