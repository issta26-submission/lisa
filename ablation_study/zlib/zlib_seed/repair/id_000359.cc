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
//<ID> 359
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char *filename = "test_zlib_api_sequence.tmp.gz";
    Bytef write_buf[] = "Zlib API sequence payload for gz write/read/flush/offset/getc.";
    unsigned int write_len = (unsigned int)(sizeof(write_buf) - 1);
    gzFile gz_wr;
    gzFile gz_rd;
    int write_ret;
    int flush_ret;
    off64_t wr_offset;
    z_size_t read_items;
    Bytef read_buf[128];
    int getc_ret;
    off64_t rd_offset;
    int close_wr_ret;
    int close_rd_ret;

    // step 2: Setup / write
    memset(read_buf, 0, sizeof(read_buf));
    gz_wr = gzopen(filename, "wb+");
    write_ret = gzwrite(gz_wr, (voidpc)write_buf, write_len);
    flush_ret = gzflush(gz_wr, 0);
    wr_offset = gzoffset64(gz_wr);
    close_wr_ret = gzclose(gz_wr);

    // step 3: Core operations - read and inspect
    gz_rd = gzopen(filename, "rb");
    read_items = gzfread((voidp)read_buf, (z_size_t)1, (z_size_t)sizeof(read_buf), gz_rd);
    getc_ret = gzgetc_(gz_rd);
    rd_offset = gzoffset64(gz_rd);
    close_rd_ret = gzclose_r(gz_rd);

    // step 4: Cleanup / validation
    (void)filename;
    (void)write_len;
    (void)write_ret;
    (void)flush_ret;
    (void)wr_offset;
    (void)read_items;
    (void)getc_ret;
    (void)rd_offset;
    (void)close_wr_ret;
    (void)close_rd_ret;
    (void)read_buf;
    // API sequence test completed successfully
    return 66;
}