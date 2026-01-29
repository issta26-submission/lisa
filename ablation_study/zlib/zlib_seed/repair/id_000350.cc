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
//<ID> 350
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char *ver = zlibVersion();
    Bytef src_buf[] = "Zlib API sequence payload for gz operations, reading, offset and flush testing.";
    uLong src_len = (uLong)(sizeof(src_buf) - 1);
    gzFile gz_write;
    gzFile gz_read;
    int ret_gzwrite;
    int ret_gzflush;
    int ret_close_write;
    int ret_close_read;
    z_size_t items_read;
    Bytef read_buf[256];
    off64_t offset_before;
    off64_t offset_after;
    int next_char;

    // step 2: Setup / initialization
    memset(read_buf, 0, sizeof(read_buf));
    gz_write = gzopen("test_zlib_api_sequence_stream.gz", "wb");
    ret_gzwrite = gzwrite(gz_write, (voidpc)src_buf, (unsigned int)src_len);
    ret_gzflush = gzflush(gz_write, 2);
    ret_close_write = gzclose_w(gz_write);

    // step 3: Core operations - reopen for read, check offset, read with gzfread and gzgetc_
    gz_read = gzopen("test_zlib_api_sequence_stream.gz", "rb");
    offset_before = gzoffset64(gz_read);
    items_read = gzfread((voidp)read_buf, (z_size_t)1, (z_size_t)sizeof(read_buf), gz_read);
    next_char = gzgetc_(gz_read);
    offset_after = gzoffset64(gz_read);

    // step 4: Cleanup / validation
    ret_close_read = gzclose_r(gz_read);
    (void)ver;
    (void)src_len;
    (void)ret_gzwrite;
    (void)ret_gzflush;
    (void)ret_close_write;
    (void)items_read;
    (void)next_char;
    (void)offset_before;
    (void)offset_after;
    (void)ret_close_read;
    // API sequence test completed successfully
    return 66;
}