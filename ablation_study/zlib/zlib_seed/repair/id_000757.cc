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
//<ID> 757
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    Bytef plain[128];
    memset(plain, 'X', sizeof(plain));
    uLong plain_len = (uLong)sizeof(plain);

    uLong bound_for_compress = compressBound(plain_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound_for_compress);
    memset(comp_buf, 0, (size_t)bound_for_compress);
    uLongf comp_len = (uLongf)bound_for_compress;
    int compress_ret = compress(comp_buf, &comp_len, plain, plain_len);

    // step 2: Configure
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    int init_ret = deflateInit_(&strm, 6, version, (int)sizeof(z_stream));
    strm.next_in = comp_buf;
    strm.avail_in = (uInt)comp_len;
    uLong out_bound = deflateBound(&strm, (uLong)comp_len);
    Bytef *out_buf = (Bytef *)malloc((size_t)out_bound);
    memset(out_buf, 0, (size_t)out_bound);
    strm.next_out = out_buf;
    strm.avail_out = (uInt)out_bound;

    z_stream copystrm;
    memset(&copystrm, 0, sizeof(copystrm));
    int copy_ret = deflateCopy(&copystrm, &strm);
    int reset_ret = deflateReset(&copystrm);

    // step 3: Operate
    int deflate_ret = deflate(&strm, 4);
    uLong final_len = strm.total_out;

    const char *fname = "test_zlib_api_sequence_tmp.gz";
    gzFile gzw = gzopen(fname, "wb");
    int gz_wr = gzwrite(gzw, out_buf, (unsigned int)final_len);
    int gz_close_w = gzclose(gzw);

    gzFile gzr = gzopen(fname, "rb");
    Bytef *read_back = (Bytef *)malloc((size_t)final_len + 16);
    memset(read_back, 0, (size_t)final_len + 16);
    z_size_t items_read = gzfread(read_back, 1, (z_size_t)final_len, gzr);
    int gz_close_r = gzclose(gzr);

    // step 4: Validate / Cleanup
    int def_end_ret = deflateEnd(&strm);
    int def_end_copy_ret = deflateEnd(&copystrm);
    free(comp_buf);
    free(out_buf);
    free(read_back);

    (void)version;
    (void)plain_len;
    (void)bound_for_compress;
    (void)compress_ret;
    (void)comp_len;
    (void)init_ret;
    (void)out_bound;
    (void)copy_ret;
    (void)reset_ret;
    (void)deflate_ret;
    (void)final_len;
    (void)gz_wr;
    (void)gz_close_w;
    (void)items_read;
    (void)gz_close_r;
    (void)def_end_ret;
    (void)def_end_copy_ret;
    // API sequence test completed successfully
    return 66;
}