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
//<ID> 700
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    Bytef plain[300];
    memset(plain, 'A', sizeof(plain));
    uLong plain_len = (uLong)sizeof(plain);

    // step 2: Configure
    z_stream def_src;
    memset(&def_src, 0, sizeof(def_src));
    int def_src_init = deflateInit_(&def_src, 6, version, (int)sizeof(z_stream));
    def_src.next_in = plain;
    def_src.avail_in = (uInt)plain_len;
    uLong def_bound = deflateBound(&def_src, plain_len);
    Bytef *def_out1 = (Bytef *)malloc((size_t)def_bound);
    Bytef *def_out2 = (Bytef *)malloc((size_t)def_bound);
    memset(def_out1, 0, (size_t)def_bound);
    memset(def_out2, 0, (size_t)def_bound);
    def_src.next_out = def_out1;
    def_src.avail_out = (uInt)def_bound;

    z_stream def_dest;
    memset(&def_dest, 0, sizeof(def_dest));
    int def_copy_ret = deflateCopy(&def_dest, &def_src);
    def_dest.next_out = def_out2;
    def_dest.avail_out = (uInt)def_bound;

    // step 3: Operate
    int def_ret_src = deflate(&def_src, 4);
    int def_ret_dest = deflate(&def_dest, 4);
    uLong def_total_src = def_src.total_out;
    uLong def_total_dest = def_dest.total_out;
    int def_end_src = deflateEnd(&def_src);
    int def_end_dest = deflateEnd(&def_dest);

    uLong a1 = adler32(0UL, plain, (uInt)(plain_len / 2));
    uLong a2 = adler32(0UL, plain + (plain_len / 2), (uInt)(plain_len - (plain_len / 2)));
    off64_t part_len = (off64_t)(plain_len - (plain_len / 2));
    uLong combined_adler = adler32_combine64(a1, a2, part_len);

    gzFile gzw = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gz_w = gzwrite(gzw, plain, (unsigned int)plain_len);
    int gz_close_w = gzclose(gzw);

    gzFile gzr = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    int c1 = gzgetc(gzr);
    int unret = gzungetc(c1, gzr);
    int c2 = gzgetc(gzr);
    int eof_flag = gzeof(gzr);
    int gz_close_r = gzclose(gzr);

    // step 4: Validate / Cleanup
    free(def_out1);
    free(def_out2);
    (void)version;
    (void)def_src_init;
    (void)def_copy_ret;
    (void)def_ret_src;
    (void)def_ret_dest;
    (void)def_total_src;
    (void)def_total_dest;
    (void)def_end_src;
    (void)def_end_dest;
    (void)a1;
    (void)a2;
    (void)part_len;
    (void)combined_adler;
    (void)gz_w;
    (void)gz_close_w;
    (void)c1;
    (void)unret;
    (void)c2;
    (void)eof_flag;
    (void)gz_close_r;
    // API sequence test completed successfully
    return 66;
}