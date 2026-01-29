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
//<ID> 1737
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize - prepare source and compress it
    const Bytef src[] = "zlib sequence test: compress2 -> write gz -> inflateCopy -> inflate";
    uLong src_len = (uLong)(sizeof(src) - 1);
    uLong bound = compressBound(src_len);
    uLongf comp_len_f = (uLongf)bound;
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    int comp_ret = compress2(comp_buf, &comp_len_f, src, src_len, 6);
    uLong comp_len = (uLong)comp_len_f;

    // step 2: Configure - write compressed data to a gzFile and flush
    gzFile gzf = gzopen("tmp_zlib_api_sequence.gz", "wb");
    int gw_ret = gzwrite(gzf, comp_buf, (unsigned int)comp_len);
    int gz_flush_ret = gzflush(gzf, 2);
    int gz_eof_flag = gzeof(gzf);
    int gzclose_w_ret = gzclose(gzf);

    // step 3: Setup inflate stream and create a copy of its state
    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    const char *ver = zlibVersion();
    int inf_init_ret = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)comp_len;
    Bytef *decomp_buf = (Bytef *)malloc((size_t)(src_len + 1));
    memset(decomp_buf, 0, (int)(src_len + 1));
    inf_strm.next_out = decomp_buf;
    inf_strm.avail_out = (uInt)(src_len + 1);
    z_stream copy_strm;
    memset(&copy_strm, 0, (int)sizeof(copy_strm));
    int copy_ret = inflateCopy(&copy_strm, &inf_strm);

    // step 4: Operate (inflate) and Cleanup
    int inf_ret = inflate(&inf_strm, 0);
    uLong decompressed = inf_strm.total_out;
    int inf_end_ret1 = inflateEnd(&inf_strm);
    int inf_end_ret2 = inflateEnd(&copy_strm);
    free(comp_buf);
    free(decomp_buf);
    (void)comp_ret;
    (void)gw_ret;
    (void)gz_flush_ret;
    (void)gz_eof_flag;
    (void)gzclose_w_ret;
    (void)inf_init_ret;
    (void)copy_ret;
    (void)inf_ret;
    (void)decompressed;
    (void)inf_end_ret1;
    (void)inf_end_ret2;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}