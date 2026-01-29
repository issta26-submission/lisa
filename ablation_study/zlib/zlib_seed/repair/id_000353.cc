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
//<ID> 353
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char *ver;
    Bytef src[] = "Zlib API sequence payload demonstrating gz read/write, gzfread, gzgetc_, gzflush and gzoffset64 usage.";
    uLong src_len = (uLong)(sizeof(src) - 1);
    Bytef comp_buf[4096];
    uLongf comp_len_f;
    int ret_compress;
    Bytef decomp_buf[4096];
    uLongf decomp_len_f;
    int ret_uncompress;
    gzFile gzw;
    gzFile gzr;
    int ret_gzwrite;
    int ret_gzflush;
    off64_t offset_after_write;
    z_size_t read_count;
    Bytef read_buf[4096];
    int ret_gzgetc;
    off64_t offset_after_read;
    int ret_gzclose_w;
    int ret_gzclose_r;

    // step 2: Setup / initialization
    ver = zlibVersion();
    memset(comp_buf, 0, sizeof(comp_buf));
    memset(decomp_buf, 0, sizeof(decomp_buf));
    memset(read_buf, 0, sizeof(read_buf));
    comp_len_f = (uLongf)sizeof(comp_buf);
    ret_compress = compress2(comp_buf, &comp_len_f, (const Bytef *)src, src_len, 6);

    // step 3: Core operations - write gz, flush, inspect offset, read with gzfread and gzgetc_
    gzw = gzopen64("test_zlib_api_sequence.gz", "wb");
    ret_gzwrite = gzwrite(gzw, src, (unsigned int)src_len);
    ret_gzflush = gzflush(gzw, 1);
    offset_after_write = gzoffset64(gzw);
    ret_gzclose_w = gzclose_w(gzw);
    gzr = gzopen64("test_zlib_api_sequence.gz", "rb");
    read_count = gzfread((voidp)read_buf, (z_size_t)1, (z_size_t)sizeof(read_buf), gzr);
    ret_gzgetc = gzgetc_(gzr);
    offset_after_read = gzoffset64(gzr);
    ret_gzclose_r = gzclose_r(gzr);

    // step 4: Validation / cleanup
    decomp_len_f = (uLongf)sizeof(decomp_buf);
    ret_uncompress = uncompress(decomp_buf, &decomp_len_f, comp_buf, (uLong)comp_len_f);
    (void)ver;
    (void)ret_compress;
    (void)ret_uncompress;
    (void)read_count;
    (void)ret_gzwrite;
    (void)ret_gzflush;
    (void)offset_after_write;
    (void)ret_gzclose_w;
    (void)ret_gzgetc;
    (void)offset_after_read;
    (void)ret_gzclose_r;
    // API sequence test completed successfully
    return 66;
}