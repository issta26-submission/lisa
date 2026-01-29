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
//<ID> 1733
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize & Configure
    const Bytef src[] = "zlib API sequence test: prepare data -> deflate -> write gz -> inflate via copy";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *ver = zlibVersion();
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    uLong bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    def_strm.next_in = (Bytef *)src;
    def_strm.avail_in = (uInt)src_len;
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)bound;

    // step 2: Operate (deflate -> write gz -> flush)
    int def_ret = deflate(&def_strm, 4);
    uLong comp_len = def_strm.total_out;
    int def_end_ret = deflateEnd(&def_strm);
    gzFile gzf_w = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gw_ret = gzwrite(gzf_w, src, (unsigned int)src_len);
    int gzflush_ret = gzflush(gzf_w, 2);
    int gzeof_after_write = gzeof(gzf_w);
    int gzclose_w = gzclose(gzf_w);

    // step 3: Validate (inflate using inflateCopy)
    Bytef *decomp_buf = (Bytef *)malloc((size_t)(src_len + 1));
    memset(decomp_buf, 0, (int)(src_len + 1));
    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)comp_len;
    inf_strm.next_out = decomp_buf;
    inf_strm.avail_out = (uInt)(src_len + 1);
    z_stream inf_copy;
    memset(&inf_copy, 0, (int)sizeof(inf_copy));
    int copy_ret = inflateCopy(&inf_copy, &inf_strm);
    int inf_copy_ret = inflate(&inf_copy, 0);
    int inf_copy_end = inflateEnd(&inf_copy);
    int inf_end_ret = inflateEnd(&inf_strm);

    // step 4: Cleanup & final validation
    gzFile gzf_r = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    Bytef *read_buf = (Bytef *)malloc((size_t)(src_len + 1));
    memset(read_buf, 0, (int)(src_len + 1));
    int read_ret = gzread(gzf_r, read_buf, (unsigned int)(src_len + 1));
    int eof_flag = gzeof(gzf_r);
    int gzclose_r = gzclose(gzf_r);
    free(comp_buf);
    free(decomp_buf);
    free(read_buf);
    (void)ver;
    (void)def_init_ret;
    (void)bound;
    (void)def_ret;
    (void)comp_len;
    (void)def_end_ret;
    (void)gw_ret;
    (void)gzflush_ret;
    (void)gzeof_after_write;
    (void)gzclose_w;
    (void)inf_init_ret;
    (void)copy_ret;
    (void)inf_copy_ret;
    (void)inf_copy_end;
    (void)inf_end_ret;
    (void)read_ret;
    (void)eof_flag;
    (void)gzclose_r;
    // API sequence test completed successfully
    return 66;
}