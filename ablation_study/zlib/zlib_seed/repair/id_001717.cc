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
//<ID> 1717
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: deflateInit_ -> deflateParams -> deflate -> gzwrite -> gzoffset64";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *ver = zlibVersion();
    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    int init_ret = deflateInit_(&strm, 6, ver, (int)sizeof(z_stream));
    strm.next_in = (Bytef *)src;
    strm.avail_in = (uInt)src_len;

    // step 2: Configure
    uLong bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    strm.next_out = comp_buf;
    strm.avail_out = (uInt)bound;
    int params_ret = deflateParams(&strm, 9, 0);
    int reset_ret = deflateReset(&strm);

    // step 3: Operate
    int def_ret = deflate(&strm, 4);
    uLong comp_len = strm.total_out;
    int end_ret = deflateEnd(&strm);
    uLong checksum_src = adler32(0UL, src, (uInt)src_len);

    gzFile gzf_w = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gw_ret = gzwrite(gzf_w, src, (unsigned int)src_len);
    int direct_w = gzdirect(gzf_w);
    off64_t offset_w = gzoffset64(gzf_w);
    int gzclose_w = gzclose(gzf_w);

    // step 4: Validate & Cleanup
    gzFile gzf_r = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    int direct_r = gzdirect(gzf_r);
    off64_t offset_r_before = gzoffset64(gzf_r);
    Bytef *decomp_buf = (Bytef *)malloc((size_t)(src_len + 8));
    memset(decomp_buf, 0, (int)(src_len + 8));
    int gr_ret = gzread(gzf_r, decomp_buf, (unsigned int)src_len);
    off64_t offset_r_after = gzoffset64(gzf_r);
    uLong checksum_read = adler32(0UL, decomp_buf, (uInt)((gr_ret > 0) ? gr_ret : 0));
    int gzclose_r = gzclose(gzf_r);
    free(comp_buf);
    free(decomp_buf);

    (void)init_ret;
    (void)params_ret;
    (void)reset_ret;
    (void)def_ret;
    (void)comp_len;
    (void)end_ret;
    (void)checksum_src;
    (void)gw_ret;
    (void)direct_w;
    (void)offset_w;
    (void)gzclose_w;
    (void)direct_r;
    (void)offset_r_before;
    (void)gr_ret;
    (void)offset_r_after;
    (void)checksum_read;
    (void)gzclose_r;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}