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
//<ID> 706
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    Bytef plain_in[300];
    memset(plain_in, 'Z', sizeof(plain_in));
    uLong plain_len = (uLong)sizeof(plain_in);
    uLong half_len = plain_len / 2UL;
    uLong a1 = adler32(0UL, plain_in, (uInt)half_len);
    uLong a2 = adler32(0UL, plain_in + half_len, (uInt)(plain_len - half_len));
    off64_t part_len64 = (off64_t)half_len;
    uLong combined_adler = adler32_combine64(a1, a2, part_len64);

    // step 2: Configure
    uLong out_bound = compressBound(plain_len);
    Bytef *def_out_buf = (Bytef *)malloc((size_t)out_bound);
    memset(def_out_buf, 0, (size_t)out_bound);
    z_stream def;
    memset(&def, 0, sizeof(def));
    int def_init_ret = deflateInit_(&def, 6, version, (int)sizeof(z_stream));
    def.next_in = plain_in;
    def.avail_in = (uInt)plain_len;
    def.next_out = def_out_buf;
    def.avail_out = (uInt)out_bound;

    // step 3: Operate
    int def_ret = deflate(&def, 4);
    uLong def_total_out = def.total_out;
    z_stream def_copy;
    memset(&def_copy, 0, sizeof(def_copy));
    int copy_ret = deflateCopy(&def_copy, &def);
    int def_end_ret = deflateEnd(&def);
    int def_copy_end_ret = deflateEnd(&def_copy);

    gzFile gz_w = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gz_w_ret = gzwrite(gz_w, def_out_buf, (unsigned int)def_total_out);
    int gz_close_w = gzclose(gz_w);

    gzFile gz_r = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    int first_char = gzgetc(gz_r);
    int unget_ret = gzungetc(first_char, gz_r);
    int second_char = gzgetc(gz_r);
    int eof_flag = gzeof(gz_r);
    int gz_close_r = gzclose(gz_r);

    // step 4: Validate / Cleanup
    free(def_out_buf);
    (void)version;
    (void)a1;
    (void)a2;
    (void)part_len64;
    (void)combined_adler;
    (void)out_bound;
    (void)def_init_ret;
    (void)def_ret;
    (void)def_total_out;
    (void)copy_ret;
    (void)def_end_ret;
    (void)def_copy_end_ret;
    (void)gz_w_ret;
    (void)gz_close_w;
    (void)first_char;
    (void)unget_ret;
    (void)second_char;
    (void)eof_flag;
    (void)gz_close_r;
    // API sequence test completed successfully
    return 66;
}