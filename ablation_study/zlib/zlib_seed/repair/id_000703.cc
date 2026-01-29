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
//<ID> 703
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
    uLong comp_bound = compressBound(plain_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_bound);
    memset(comp_buf, 0, (size_t)comp_bound);
    uLongf comp_len = (uLongf)comp_bound;
    int comp_ret = compress2(comp_buf, &comp_len, plain_in, plain_len, 6);

    // step 2: Configure
    z_stream def;
    memset(&def, 0, sizeof(def));
    int def_init_ret = deflateInit_(&def, 6, version, (int)sizeof(z_stream));
    def.next_in = comp_buf;
    def.avail_in = (uInt)comp_len;
    uLong def_out_bound = deflateBound(&def, plain_len);
    Bytef *def_out_buf = (Bytef *)malloc((size_t)def_out_bound);
    memset(def_out_buf, 0, (size_t)def_out_bound);
    def.next_out = def_out_buf;
    def.avail_out = (uInt)def_out_bound;
    z_stream def_copy;
    memset(&def_copy, 0, sizeof(def_copy));
    int copy_ret = deflateCopy(&def_copy, &def);

    // step 3: Operate
    int def_ret = deflate(&def, 4);
    uLong def_total_out = def.total_out;
    int def_end_ret = deflateEnd(&def);
    int def_copy_end_ret = deflateEnd(&def_copy);

    gzFile gzw = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gz_w = gzwrite(gzw, comp_buf, (unsigned int)comp_len);
    int gz_close_w = gzclose(gzw);

    gzFile gzr = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    int first_ch = gzgetc(gzr);
    int push_ret = gzungetc(first_ch, gzr);
    int second_ch = gzgetc(gzr);
    int at_eof = gzeof(gzr);
    int gz_close_r = gzclose(gzr);

    // step 4: Validate / Cleanup
    uLong a1 = adler32(0UL, plain_in, 150);
    uLong a2 = adler32(0UL, plain_in + 150, 150);
    off64_t part_len = (off64_t)150;
    uLong combined = adler32_combine64(a1, a2, part_len);

    free(comp_buf);
    free(def_out_buf);

    (void)version;
    (void)plain_len;
    (void)comp_bound;
    (void)comp_ret;
    (void)comp_len;
    (void)def_init_ret;
    (void)def_out_bound;
    (void)copy_ret;
    (void)def_ret;
    (void)def_total_out;
    (void)def_end_ret;
    (void)def_copy_end_ret;
    (void)gzw;
    (void)gz_w;
    (void)gz_close_w;
    (void)gzr;
    (void)first_ch;
    (void)push_ret;
    (void)second_ch;
    (void)at_eof;
    (void)gz_close_r;
    (void)a1;
    (void)a2;
    (void)part_len;
    (void)combined;
    // API sequence test completed successfully
    return 66;
}