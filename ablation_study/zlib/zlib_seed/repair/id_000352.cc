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
//<ID> 352
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char *ver;
    gzFile gzw;
    gzFile gzr;
    char write_buf[] = "Zlib fuzz harness payload for gz operations and gzfread/gzgetc_/gzoffset64/gzflush tests";
    unsigned int write_len = (unsigned int)(sizeof(write_buf) - 1);
    int gzwrite_ret;
    int gzflush_ret;
    off64_t off_after_write;
    char read_buf[128];
    z_size_t gzfread_ret;
    int gzgetc_ret;
    off64_t off_after_read;
    int gzclose_ret1;
    int gzclose_ret2;

    // step 2: Setup / initialization
    ver = zlibVersion();
    memset(read_buf, 0, sizeof(read_buf));
    gzw = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    gzwrite_ret = gzwrite(gzw, (voidpc)write_buf, write_len);
    gzflush_ret = gzflush(gzw, 2);
    off_after_write = gzoffset64(gzw);
    gzclose_ret1 = gzclose(gzw);

    // step 3: Core operations - read back, validate offsets and single-char access
    gzr = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    gzfread_ret = gzfread((voidp)read_buf, (z_size_t)1, (z_size_t)sizeof(read_buf), gzr);
    gzgetc_ret = gzgetc_(gzr);
    off_after_read = gzoffset64(gzr);
    gzclose_ret2 = gzclose(gzr);

    // step 4: Cleanup / validation
    (void)ver;
    (void)gzwrite_ret;
    (void)gzflush_ret;
    (void)off_after_write;
    (void)gzfread_ret;
    (void)gzgetc_ret;
    (void)off_after_read;
    (void)gzclose_ret1;
    (void)gzclose_ret2;
    // API sequence test completed successfully
    return 66;
}