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
//<ID> 354
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char input_data[] = "Zlib API sequence: write via gzwrite, flush, read back with gzfread and inspect with gzgetc_.";
    unsigned char read_buf[512];
    gzFile gz_write = (gzFile)0;
    gzFile gz_read = (gzFile)0;
    int gzwrite_ret;
    int gzflush_ret;
    int gzclose_ret_write;
    int gzclose_ret_read;
    z_size_t items_read;
    int single_char;
    off64_t offset_after_write;
    off64_t offset_after_read;

    // step 2: Setup / initialization
    memset((void *)read_buf, 0, sizeof(read_buf));
    gz_write = gzopen64("test_zlib_api_sequence.gz", "wb");
    gz_read = (gzFile)0;
    gzwrite_ret = 0;
    gzflush_ret = 0;
    gzclose_ret_write = 0;
    gzclose_ret_read = 0;
    items_read = (z_size_t)0;
    single_char = 0;
    offset_after_write = (off64_t)0;
    offset_after_read = (off64_t)0;

    // step 3: Core operations - write, flush, get write offset, close, then open for read
    gzwrite_ret = gzwrite(gz_write, (voidpc)input_data, (unsigned int)(sizeof(input_data) - 1));
    offset_after_write = gzoffset64(gz_write);
    gzflush_ret = gzflush(gz_write, 0);
    gzclose_ret_write = gzclose(gz_write);
    gz_read = gzopen64("test_zlib_api_sequence.gz", "rb");

    // step 4: Core operations - read, inspect next char, get read offset, cleanup
    items_read = gzfread((voidp)read_buf, (z_size_t)1, (z_size_t)sizeof(read_buf), gz_read);
    single_char = gzgetc_(gz_read);
    offset_after_read = gzoffset64(gz_read);
    gzclose_ret_read = gzclose(gz_read);

    (void)gzwrite_ret;
    (void)gzflush_ret;
    (void)gzclose_ret_write;
    (void)gzclose_ret_read;
    (void)items_read;
    (void)single_char;
    (void)offset_after_write;
    (void)offset_after_read;
    // API sequence test completed successfully
    return 66;
}