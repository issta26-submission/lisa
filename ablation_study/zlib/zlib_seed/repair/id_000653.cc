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
//<ID> 653
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    Bytef plain_in[1024];
    memset(plain_in, 'A', sizeof(plain_in));
    uLong plain_len = (uLong)sizeof(plain_in);
    Bytef comp_buf1[4096];
    memset(comp_buf1, 0, sizeof(comp_buf1));
    uLongf comp_len1 = (uLongf)sizeof(comp_buf1);
    int comp_ret = compress(comp_buf1, &comp_len1, plain_in, plain_len);

    // step 2: Configure
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    def_strm.next_in = comp_buf1;
    def_strm.avail_in = (uInt)comp_len1;
    Bytef comp_buf2[8192];
    memset(comp_buf2, 0, sizeof(comp_buf2));
    def_strm.next_out = comp_buf2;
    def_strm.avail_out = (uInt)sizeof(comp_buf2);

    // step 3: Operate
    int def_ret = deflate(&def_strm, 4);
    uLong def_out_len = def_strm.total_out;
    int def_end_ret = deflateEnd(&def_strm);
    gzFile gz_w = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gz_write_ret = gzwrite(gz_w, plain_in, (unsigned int)plain_len);
    int gz_close_w = gzclose(gz_w);
    gzFile gz_r = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    int gz_char = gzgetc(gz_r);
    int gz_close_r = gzclose(gz_r);

    // step 4: Validate / Cleanup
    (void)version;
    (void)comp_ret;
    (void)comp_len1;
    (void)def_init_ret;
    (void)def_ret;
    (void)def_out_len;
    (void)def_end_ret;
    (void)gz_write_ret;
    (void)gz_close_w;
    (void)gz_char;
    (void)gz_close_r;
    // API sequence test completed successfully
    return 66;
}