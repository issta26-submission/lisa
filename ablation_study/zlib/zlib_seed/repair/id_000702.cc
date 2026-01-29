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
//<ID> 702
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    Bytef plain_in[512];
    memset(plain_in, 'Z', sizeof(plain_in));
    uLong plain_len = (uLong)sizeof(plain_in);
    uInt half = (uInt)(plain_len / 2);
    uLong a1 = adler32(0UL, plain_in, half);
    uLong a2 = adler32(0UL, plain_in + half, (uInt)(plain_len - half));
    off64_t half_off = (off64_t)half;
    uLong combined_adler = adler32_combine64(a1, a2, half_off);

    // step 2: Configure
    z_stream def;
    memset(&def, 0, sizeof(def));
    int def_init_ret = deflateInit_(&def, 6, version, (int)sizeof(z_stream));
    def.next_in = plain_in;
    def.avail_in = (uInt)plain_len;
    uLong def_out_bound = deflateBound(&def, plain_len);
    Bytef *def_out_buf = (Bytef *)malloc((size_t)def_out_bound);
    memset(def_out_buf, 0, (size_t)def_out_bound);
    def.next_out = def_out_buf;
    def.avail_out = (uInt)def_out_bound;
    int def_ret = deflate(&def, 4);

    z_stream def_copy;
    memset(&def_copy, 0, sizeof(def_copy));
    int def_copy_ret = deflateCopy(&def_copy, &def);
    uLong def_total_out = def.total_out;

    // step 3: Operate
    gzFile gz_w = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gz_w_ret = gzwrite(gz_w, def_out_buf, (unsigned int)def_total_out);
    int gz_close_w = gzclose(gz_w);
    gzFile gz_r = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    int c1 = gzgetc(gz_r);
    int gu_ret = gzungetc(c1, gz_r);
    int c2 = gzgetc(gz_r);
    int eof_flag = gzeof(gz_r);
    int gz_close_r = gzclose(gz_r);

    // step 4: Validate / Cleanup
    int def_end_copy = deflateEnd(&def_copy);
    int def_end = deflateEnd(&def);
    free(def_out_buf);
    (void)version;
    (void)a1;
    (void)a2;
    (void)half_off;
    (void)combined_adler;
    (void)def_init_ret;
    (void)def_ret;
    (void)def_copy_ret;
    (void)def_total_out;
    (void)gz_w_ret;
    (void)gz_close_w;
    (void)c1;
    (void)gu_ret;
    (void)c2;
    (void)eof_flag;
    (void)gz_close_r;
    (void)def_end_copy;
    (void)def_end;
    // API sequence test completed successfully
    return 66;
}