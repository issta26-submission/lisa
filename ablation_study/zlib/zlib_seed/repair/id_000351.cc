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
//<ID> 351
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char *filename = "test_zlib_api_sequence_stream.gz";
    const char write_data[] = "Zlib stream payload for gz* API sequence testing.";
    const unsigned int write_len = (unsigned int)(sizeof(write_data) - 1);
    char read_buf[128];
    memset((void *)read_buf, 0, sizeof(read_buf));
    gzFile gz_write = (gzFile)0;
    gzFile gz_read = (gzFile)0;
    int ret_gzwrite;
    int ret_gzflush;
    off64_t offset_after_write;
    z_size_t items_read;
    int ch_after_read;
    int ret_gzclose_write;
    int ret_gzclose_read;

    // step 2: Setup / open and write
    gz_write = gzopen(filename, "wb");
    ret_gzwrite = gzwrite(gz_write, write_data, write_len);
    ret_gzflush = gzflush(gz_write, 0);
    offset_after_write = gzoffset64(gz_write);
    ret_gzclose_write = gzclose(gz_write);

    // step 3: Open for read and read using gzfread and gzgetc_
    gz_read = gzopen(filename, "rb");
    items_read = gzfread((voidp)read_buf, (z_size_t)1, (z_size_t)sizeof(read_buf), gz_read);
    ch_after_read = gzgetc_(gz_read);
    offset_after_write = gzoffset64(gz_read);
    ret_gzclose_read = gzclose(gz_read);

    // step 4: Cleanup / validation
    (void)filename;
    (void)write_len;
    (void)ret_gzwrite;
    (void)ret_gzflush;
    (void)offset_after_write;
    (void)items_read;
    (void)ch_after_read;
    (void)ret_gzclose_write;
    (void)ret_gzclose_read;
    // API sequence test completed successfully
    return 66;
}