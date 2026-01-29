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
//<ID> 753
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
    memset(plain, 'Z', sizeof(plain));
    uLong plain_len = (uLong)sizeof(plain);

    uLong def_bound_tmp = compressBound(plain_len);
    uLongf comp_len = (uLongf)def_bound_tmp;
    Bytef *comp_buf = (Bytef *)malloc((size_t)def_bound_tmp);
    memset(comp_buf, 0, (size_t)def_bound_tmp);
    int comp_ret = compress(comp_buf, &comp_len, plain, plain_len);

    // step 2: Configure
    z_stream zs;
    memset(&zs, 0, sizeof(zs));
    int init_ret = deflateInit_(&zs, 6, version, (int)sizeof(z_stream));
    zs.next_in = plain;
    zs.avail_in = (uInt)plain_len;
    uLong def_out_bound = deflateBound(&zs, plain_len);
    Bytef *def_out = (Bytef *)malloc((size_t)def_out_bound);
    memset(def_out, 0, (size_t)def_out_bound);
    zs.next_out = def_out;
    zs.avail_out = (uInt)def_out_bound;
    int reset_ret = deflateReset(&zs);

    z_stream zs_copy;
    memset(&zs_copy, 0, sizeof(zs_copy));
    int copy_ret = deflateCopy(&zs_copy, &zs);

    // step 3: Operate
    int def_ret = deflate(&zs, 4);
    uLong def_comp_len = zs.total_out;
    int end_ret = deflateEnd(&zs);
    int copy_end_ret = deflateEnd(&zs_copy);

    const char *fname = "test_zlib_api_sequence_tmp.gz";
    gzFile gw = gzopen(fname, "wb");
    int gz_wr = gzwrite(gw, def_out, (unsigned int)def_comp_len);
    int gz_close_w = gzclose(gw);

    gzFile gr = gzopen(fname, "rb");
    Bytef *read_buf = (Bytef *)malloc((size_t)(def_comp_len + 1));
    memset(read_buf, 0, (size_t)(def_comp_len + 1));
    z_size_t items_read = gzfread(read_buf, 1, (z_size_t)def_comp_len, gr);
    int gz_close_r = gzclose(gr);

    // step 4: Validate / Cleanup
    free(comp_buf);
    free(def_out);
    free(read_buf);

    (void)version;
    (void)plain_len;
    (void)def_bound_tmp;
    (void)comp_len;
    (void)comp_ret;
    (void)init_ret;
    (void)reset_ret;
    (void)copy_ret;
    (void)def_ret;
    (void)def_comp_len;
    (void)end_ret;
    (void)copy_end_ret;
    (void)fname;
    (void)gz_wr;
    (void)gz_close_w;
    (void)items_read;
    (void)gz_close_r;
    // API sequence test completed successfully
    return 66;
}