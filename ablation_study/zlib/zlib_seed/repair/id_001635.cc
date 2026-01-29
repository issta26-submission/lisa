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
//<ID> 1635
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: gzopen64 + deflatePending + deflateEnd + inflate usage";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *ver = zlibVersion();
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    uLong comp_bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_bound);
    memset(comp_buf, 0, (int)comp_bound);
    def_strm.next_in = (Bytef *)src;
    def_strm.avail_in = (uInt)src_len;
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)comp_bound;

    // step 2: Configure & Operate (compress)
    int deflate_ret = deflate(&def_strm, 4);
    unsigned int pending_bytes = 0;
    int pending_bits = 0;
    int pending_ret = deflatePending(&def_strm, &pending_bytes, &pending_bits);

    // step 3: Persist compressed data via gzopen64 and prepare inflate
    gzFile gzf = gzopen64("test_zlib_api_sequence.gz", "wb");
    unsigned int write_len = (unsigned int)def_strm.total_out;
    int gzwrite_ret = 0;
    gzwrite_ret = gzwrite(gzf, comp_buf, write_len);
    int gzclose_ret = gzclose(gzf);
    z_stream infl_strm;
    memset(&infl_strm, 0, (int)sizeof(infl_strm));
    int infl_init_ret = inflateInit_(&infl_strm, ver, (int)sizeof(z_stream));
    infl_strm.next_in = comp_buf;
    infl_strm.avail_in = (uInt)write_len;
    Bytef *decomp_buf = (Bytef *)malloc((size_t)src_len);
    memset(decomp_buf, 0, (int)src_len);
    infl_strm.next_out = decomp_buf;
    infl_strm.avail_out = (uInt)src_len;

    // step 4: Inflate, cleanup and validate
    int inflate_ret = inflate(&infl_strm, 4);
    int def_end_ret = deflateEnd(&def_strm);
    int infl_end_ret = inflateEnd(&infl_strm);
    free(comp_buf);
    free(decomp_buf);
    (void)ver;
    (void)def_init_ret;
    (void)deflate_ret;
    (void)pending_bytes;
    (void)pending_bits;
    (void)pending_ret;
    (void)write_len;
    (void)gzwrite_ret;
    (void)gzclose_ret;
    (void)infl_init_ret;
    (void)inflate_ret;
    (void)def_end_ret;
    (void)infl_end_ret;
    // API sequence test completed successfully
    return 66;
}