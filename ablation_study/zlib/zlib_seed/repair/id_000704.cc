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
//<ID> 704
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    Bytef plain_in[256];
    memset(plain_in, 'A', sizeof(plain_in));
    uLong plain_len = (uLong)sizeof(plain_in);
    uLong comp_bound1 = compressBound(plain_len);
    Bytef *comp_buf1 = (Bytef *)malloc((size_t)comp_bound1);
    memset(comp_buf1, 0, (size_t)comp_bound1);
    uLongf comp_len1 = (uLongf)comp_bound1;
    int comp_ret = compress2(comp_buf1, &comp_len1, plain_in, plain_len, 6);

    z_stream def_src;
    memset(&def_src, 0, sizeof(def_src));
    int def_init_ret = deflateInit_(&def_src, 6, version, (int)sizeof(z_stream));
    def_src.next_in = plain_in;
    def_src.avail_in = (uInt)plain_len;
    uLong def_bound = deflateBound(&def_src, plain_len);
    Bytef *def_out = (Bytef *)malloc((size_t)def_bound);
    memset(def_out, 0, (size_t)def_bound);
    def_src.next_out = def_out;
    def_src.avail_out = (uInt)def_bound;
    int def_ret1 = deflate(&def_src, 4);

    // step 2: Configure
    z_stream def_copy;
    memset(&def_copy, 0, sizeof(def_copy));
    int copy_ret = deflateCopy(&def_copy, &def_src);
    Bytef *def_copy_out = (Bytef *)malloc((size_t)def_bound);
    memset(def_copy_out, 0, (size_t)def_bound);
    def_copy.next_out = def_copy_out;
    def_copy.avail_out = (uInt)def_bound;
    int def_ret2 = deflate(&def_copy, 4);
    int def_end_copy = deflateEnd(&def_copy);
    int def_end_src = deflateEnd(&def_src);

    uLong a1 = adler32(0UL, plain_in, 128);
    uLong a2 = adler32(0UL, plain_in + 128, 128);
    off64_t part_len = (off64_t)128;
    uLong combined_adler = adler32_combine64(a1, a2, part_len);

    // step 3: Operate
    gzFile gz_w = gzopen64("test_zlib_api_sequence_tmp.gz", "wb");
    int gz_w_ret = gzwrite(gz_w, def_out, (unsigned int)def_src.total_out);
    int gz_close_w = gzclose(gz_w);

    gzFile gz_r = gzopen64("test_zlib_api_sequence_tmp.gz", "rb");
    int c1 = gzgetc(gz_r);
    int unget_ret = gzungetc(c1, gz_r);
    int c2 = gzgetc(gz_r);
    int eof_flag = gzeof(gz_r);
    int gz_close_r = gzclose(gz_r);

    // step 4: Validate / Cleanup
    free(comp_buf1);
    free(def_out);
    free(def_copy_out);
    (void)version;
    (void)plain_len;
    (void)comp_bound1;
    (void)comp_len1;
    (void)comp_ret;
    (void)def_init_ret;
    (void)def_bound;
    (void)def_ret1;
    (void)copy_ret;
    (void)def_ret2;
    (void)def_end_copy;
    (void)def_end_src;
    (void)a1;
    (void)a2;
    (void)part_len;
    (void)combined_adler;
    (void)gz_w_ret;
    (void)gz_close_w;
    (void)c1;
    (void)unget_ret;
    (void)c2;
    (void)eof_flag;
    (void)gz_close_r;
    // API sequence test completed successfully
    return 66;
}