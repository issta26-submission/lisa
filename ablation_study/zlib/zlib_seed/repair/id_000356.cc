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
//<ID> 356
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char *ver;
    Bytef src_buf[] = "Example payload for zlib gz operations demonstrating compress, write, read and offsets.";
    uLong src_len = (uLong)(sizeof(src_buf) - 1);
    Bytef comp_buf[8192];
    uLongf comp_len_f;
    int ret_compress;
    gzFile gz_write;
    int ret_gzwrite;
    int ret_gzflush;
    off64_t write_offset64;
    gzFile gz_read;
    z_size_t items_read;
    Bytef read_buf[8192];
    int ch;
    off64_t read_offset64;
    int ret_gzclose_write;
    int ret_gzclose_read;

    // step 2: Setup / initialization
    ver = zlibVersion();
    memset(comp_buf, 0, sizeof(comp_buf));
    memset(read_buf, 0, sizeof(read_buf));
    comp_len_f = (uLongf)sizeof(comp_buf);
    ret_compress = compress(comp_buf, &comp_len_f, (const Bytef *)src_buf, src_len);

    // step 3: Core operations - gz write, flush, get offset, reopen and read using gzfread and gzgetc_
    gz_write = gzopen("test_zlib_api_sequence.gz", "wb");
    ret_gzwrite = gzwrite(gz_write, (voidpc)comp_buf, (unsigned int)comp_len_f);
    ret_gzflush = gzflush(gz_write, 0);
    write_offset64 = gzoffset64(gz_write);
    ret_gzclose_write = gzclose(gz_write);
    gz_read = gzopen("test_zlib_api_sequence.gz", "rb");
    items_read = gzfread((voidp)read_buf, (z_size_t)1, (z_size_t)comp_len_f, gz_read);
    ch = gzgetc_(gz_read);
    read_offset64 = gzoffset64(gz_read);
    ret_gzclose_read = gzclose(gz_read);

    // step 4: Cleanup / validation
    (void)ver;
    (void)src_len;
    (void)comp_len_f;
    (void)ret_compress;
    (void)ret_gzwrite;
    (void)ret_gzflush;
    (void)write_offset64;
    (void)items_read;
    (void)ch;
    (void)read_offset64;
    (void)ret_gzclose_write;
    (void)ret_gzclose_read;
    // API sequence test completed successfully
    return 66;
}