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
//<ID> 1734
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize and produce compressed payload
    const Bytef src[] = "zlib API sequence test: compress -> write gz -> read gz -> inflateCopy -> inflate";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *ver = zlibVersion();
    uLong bound = compressBound(src_len);
    uLongf comp_len = (uLongf)bound;
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    int comp_ret = compress(comp_buf, &comp_len, src, src_len);
    gzFile gzf_w = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gw_ret = gzwrite(gzf_w, comp_buf, (unsigned int)comp_len);
    int gzf_flush_ret = gzflush(gzf_w, 2);
    int gzclose_w = gzclose(gzf_w);

    // step 2: Read compressed payload back from gz file
    gzFile gzf_r = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    Bytef *comp_buf2 = (Bytef *)malloc((size_t)comp_len);
    memset(comp_buf2, 0, (int)comp_len);
    int read_bytes = gzread(gzf_r, comp_buf2, (unsigned int)comp_len);
    int eof_flag = gzeof(gzf_r);
    int gzclose_r = gzclose(gzf_r);

    // step 3: Inflate using inflateInit_, inflateCopy and inflate
    z_stream inf_src;
    memset(&inf_src, 0, (int)sizeof(inf_src));
    int inf_init_ret = inflateInit_(&inf_src, ver, (int)sizeof(z_stream));
    inf_src.next_in = comp_buf2;
    inf_src.avail_in = (uInt)read_bytes;
    Bytef *decomp_buf = (Bytef *)malloc((size_t)(src_len + 1));
    memset(decomp_buf, 0, (int)(src_len + 1));
    inf_src.next_out = decomp_buf;
    inf_src.avail_out = (uInt)(src_len + 1);
    z_stream inf_copy;
    memset(&inf_copy, 0, (int)sizeof(inf_copy));
    int copy_ret = inflateCopy(&inf_copy, &inf_src);
    int inf_ret = inflate(&inf_copy, 0);
    int inf_end_copy = inflateEnd(&inf_copy);
    int inf_end_src = inflateEnd(&inf_src);

    // step 4: Cleanup
    free(comp_buf);
    free(comp_buf2);
    free(decomp_buf);
    (void)ver;
    (void)comp_ret;
    (void)gw_ret;
    (void)gzf_flush_ret;
    (void)gzclose_w;
    (void)read_bytes;
    (void)eof_flag;
    (void)gzclose_r;
    (void)inf_init_ret;
    (void)copy_ret;
    (void)inf_ret;
    (void)inf_end_copy;
    (void)inf_end_src;
    // API sequence test completed successfully
    return 66;
}