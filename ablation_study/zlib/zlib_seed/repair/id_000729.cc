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
//<ID> 729
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *fname = "test_zlib_api_sequence_tmp.gz";
    unsigned char plain[256];
    memset(plain, 'Z', sizeof(plain));
    size_t plain_len = sizeof(plain);
    gzFile gzw = gzopen(fname, "wb");
    int written = gzwrite(gzw, plain, (unsigned int)plain_len);
    int close_w = gzclose(gzw);

    // step 2: Configure
    gzFile gzr = gzopen(fname, "rb");
    off64_t seek_off = gzseek64(gzr, (off64_t)100, 0);

    // step 3: Operate
    z_size_t to_read = (z_size_t)128;
    voidp read_buf = malloc((size_t)to_read);
    memset(read_buf, 0, (size_t)to_read);
    z_size_t read_items = gzfread(read_buf, 1, to_read, gzr);
    off64_t new_pos = gzseek64(gzr, (off64_t)0, 1);

    // step 4: Validate / Cleanup
    int close_r = gzclose(gzr);
    free(read_buf);
    (void)written;
    (void)close_w;
    (void)seek_off;
    (void)read_items;
    (void)new_pos;
    (void)plain_len;
    // API sequence test completed successfully
    return 66;
}