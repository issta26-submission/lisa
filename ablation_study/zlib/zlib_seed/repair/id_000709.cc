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
//<ID> 709
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    Bytef plain[256];
    memset(plain, 'X', sizeof(plain));
    uLong plain_len = (uLong)sizeof(plain);
    uLong out_bound = compressBound(plain_len);
    Bytef *out_buf = (Bytef *)malloc((size_t)out_bound);
    memset(out_buf, 0, (size_t)out_bound);

    z_stream def;
    memset(&def, 0, sizeof(def));
    int def_init_ret = deflateInit_(&def, 6, version, (int)sizeof(z_stream));
    def.next_in = plain;
    def.avail_in = (uInt)plain_len;
    def.next_out = out_buf;
    def.avail_out = (uInt)out_bound;

    // step 2: Configure
    z_stream def_copy;
    memset(&def_copy, 0, sizeof(def_copy));
    int copy_ret = deflateCopy(&def_copy, &def);
    Bytef *out_buf_copy = (Bytef *)malloc((size_t)out_bound);
    memset(out_buf_copy, 0, (size_t)out_bound);
    def_copy.next_out = out_buf_copy;
    def_copy.avail_out = (uInt)out_bound;
    def_copy.next_in = plain;
    def_copy.avail_in = (uInt)plain_len;

    // step 3: Operate
    int def_ret = deflate(&def, 4);
    int def_copy_ret = deflate(&def_copy, 4);
    uLong def_total = def.total_out;
    uLong def_copy_total = def_copy.total_out;
    int def_end_ret = deflateEnd(&def);
    int def_copy_end_ret = deflateEnd(&def_copy);

    gzFile gz = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gz_w = gzwrite(gz, out_buf, (unsigned int)def_total);
    int gz_close_ret = gzclose(gz);

    gzFile gzr = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    int c1 = gzgetc(gzr);
    int unget_ret = gzungetc(c1, gzr);
    int c2 = gzgetc(gzr);
    int eof_flag = gzeof(gzr);
    int gzr_close_ret = gzclose(gzr);

    // step 4: Validate / Cleanup
    uLong a1 = adler32(0UL, plain, 128);
    uLong a2 = adler32(0UL, plain + 128, 128);
    off64_t part_len = (off64_t)128;
    uLong combined = adler32_combine64(a1, a2, part_len);

    free(out_buf);
    free(out_buf_copy);
    (void)version;
    (void)plain_len;
    (void)out_bound;
    (void)def_init_ret;
    (void)copy_ret;
    (void)def_ret;
    (void)def_copy_ret;
    (void)def_total;
    (void)def_copy_total;
    (void)def_end_ret;
    (void)def_copy_end_ret;
    (void)gz_w;
    (void)gz_close_ret;
    (void)c1;
    (void)unget_ret;
    (void)c2;
    (void)eof_flag;
    (void)gzr_close_ret;
    (void)a1;
    (void)a2;
    (void)part_len;
    (void)combined;
    // API sequence test completed successfully
    return 66;
}