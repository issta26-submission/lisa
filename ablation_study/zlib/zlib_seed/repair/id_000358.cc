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
//<ID> 358
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char *ver = zlibVersion();
    Bytef src_buf[] = "Zlib API sequence payload: compress, write to gz, flush, offset, read back, single-get.";
    uLong src_len = (uLong)(sizeof(src_buf) - 1);
    Bytef comp_buf[8192];
    Bytef read_buf[8192];
    uLongf comp_len_f = (uLongf)sizeof(comp_buf);
    int ret_compress;
    gzFile gz = 0;
    int ret_gzwrite;
    int ret_gzflush;
    off64_t off64;
    z_size_t items_read;
    int single_char;
    int ret_gzclose;

    // step 2: Setup / initialization
    memset(comp_buf, 0, sizeof(comp_buf));
    memset(read_buf, 0, sizeof(read_buf));
    ret_compress = compress2(comp_buf, &comp_len_f, (const Bytef *)src_buf, src_len, 6);
    gz = gzopen("test_zlib_api_sequence_stream.gz", "wb+");

    // step 3: Core operations - write compressed data, flush, get offset, rewind, read, single-get
    ret_gzwrite = gzwrite(gz, (voidpc)comp_buf, (unsigned int)comp_len_f);
    ret_gzflush = gzflush(gz, 2);
    off64 = gzoffset64(gz);
    gzrewind(gz);
    items_read = gzfread((voidp)read_buf, (z_size_t)1, (z_size_t)sizeof(read_buf), gz);
    single_char = gzgetc_(gz);

    // step 4: Cleanup / validation
    ret_gzclose = gzclose(gz);
    (void)ver;
    (void)src_len;
    (void)ret_compress;
    (void)ret_gzwrite;
    (void)ret_gzflush;
    (void)off64;
    (void)items_read;
    (void)single_char;
    (void)ret_gzclose;
    // API sequence test completed successfully
    return 66;
}