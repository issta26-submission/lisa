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
//<ID> 708
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    Bytef plain[400];
    memset(plain, 'Z', sizeof(plain));
    uLong plain_len = (uLong)sizeof(plain);
    uLong half_len = plain_len / 2;
    uLong a1 = adler32(0UL, plain, (uInt)half_len);
    uLong a2 = adler32(0UL, plain + half_len, (uInt)(plain_len - half_len));
    off64_t part_len = (off64_t)(plain_len - half_len);
    uLong combined_adler = adler32_combine64(a1, a2, part_len);

    // step 2: Configure
    z_stream def_src;
    memset(&def_src, 0, sizeof(def_src));
    int def_init_ret = deflateInit_(&def_src, 6, version, (int)sizeof(z_stream));
    def_src.next_in = plain;
    def_src.avail_in = (uInt)plain_len;
    uLong out_bound = deflateBound(&def_src, plain_len);
    Bytef *def_out_buf = (Bytef *)malloc((size_t)out_bound);
    memset(def_out_buf, 0, (size_t)out_bound);
    def_src.next_out = def_out_buf;
    def_src.avail_out = (uInt)out_bound;
    int def_ret = deflate(&def_src, 4);
    uLong produced = def_src.total_out;

    z_stream def_copy;
    memset(&def_copy, 0, sizeof(def_copy));
    int def_copy_ret = deflateCopy(&def_copy, &def_src);
    Bytef *copy_out_buf = (Bytef *)malloc((size_t)out_bound);
    memset(copy_out_buf, 0, (size_t)out_bound);
    def_copy.next_out = copy_out_buf;
    def_copy.avail_out = (uInt)out_bound;
    int def_copy_run = deflate(&def_copy, 4);

    // step 3: Operate
    gzFile gz = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gz_w = gzwrite(gz, def_out_buf, (unsigned int)produced);
    int gz_close_w = gzclose(gz);
    gz = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    int c1 = gzgetc(gz);
    int gu_ret = gzungetc(c1, gz);
    int c2 = gzgetc(gz);
    int eof_flag = gzeof(gz);
    int gz_close_r = gzclose(gz);

    // step 4: Validate / Cleanup
    int def_end_ret_copy = deflateEnd(&def_copy);
    int def_end_ret_src = deflateEnd(&def_src);
    free(def_out_buf);
    free(copy_out_buf);
    (void)version;
    (void)combined_adler;
    (void)def_init_ret;
    (void)def_ret;
    (void)produced;
    (void)def_copy_ret;
    (void)def_copy_run;
    (void)gz_w;
    (void)gz_close_w;
    (void)c1;
    (void)gu_ret;
    (void)c2;
    (void)eof_flag;
    (void)gz_close_r;
    // API sequence test completed successfully
    return 66;
}