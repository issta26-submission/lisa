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
//<ID> 1713
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: initialize -> deflateParams -> deflateReset -> deflate -> gzoffset64/gzdirect";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *ver = zlibVersion();
    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    int def_init_ret = deflateInit_(&strm, 6, ver, (int)sizeof(z_stream));
    strm.next_in = (Bytef *)src;
    strm.avail_in = (uInt)src_len;
    uLong bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    strm.next_out = comp_buf;
    strm.avail_out = (uInt)bound;

    // step 2: Configure
    int dp_ret = deflateParams(&strm, 6, 0);
    int dr_ret = deflateReset(&strm);
    uLong src_adler = adler32(1L, src, (uInt)src_len);
    strm.adler = src_adler;

    // step 3: Operate
    int def_ret = deflate(&strm, 4);
    uLong comp_len = strm.total_out;
    uLong comp_adler = adler32(1L, comp_buf, (uInt)comp_len);
    int def_end_ret = deflateEnd(&strm);
    gzFile gzf_w = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gw_ret = gzwrite(gzf_w, src, (unsigned int)src_len);
    int gw_direct = gzdirect(gzf_w);
    off64_t gw_offset = gzoffset64(gzf_w);
    int gw_flush = gzflush(gzf_w, 2);
    int gzclose_w = gzclose(gzf_w);

    // step 4: Validate & Cleanup
    gzFile gzf_r = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    int gr_direct = gzdirect(gzf_r);
    off64_t gr_offset = gzoffset64(gzf_r);
    Bytef *read_buf = (Bytef *)malloc((size_t)(src_len + 1));
    memset(read_buf, 0, (int)(src_len + 1));
    int gr_ret = gzread(gzf_r, read_buf, (unsigned int)src_len);
    int gzclose_r = gzclose(gzf_r);
    free(comp_buf);
    free(read_buf);
    (void)ver;
    (void)def_init_ret;
    (void)dp_ret;
    (void)dr_ret;
    (void)src_adler;
    (void)def_ret;
    (void)comp_len;
    (void)comp_adler;
    (void)def_end_ret;
    (void)gw_ret;
    (void)gw_direct;
    (void)gw_offset;
    (void)gw_flush;
    (void)gzclose_w;
    (void)gr_direct;
    (void)gr_offset;
    (void)gr_ret;
    (void)gzclose_r;
    // API sequence test completed successfully
    return 66;
}