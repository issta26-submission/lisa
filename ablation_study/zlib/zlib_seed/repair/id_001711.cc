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
//<ID> 1711
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: deflateInit_ -> deflateParams -> deflateReset -> deflate -> gzoffset64/gzdirect";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *ver = zlibVersion();
    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    int def_init_ret = deflateInit_(&strm, 6, ver, (int)sizeof(z_stream));

    // step 2: Configure
    uLong bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    strm.next_in = (Bytef *)src;
    strm.avail_in = (uInt)src_len;
    strm.next_out = comp_buf;
    strm.avail_out = (uInt)bound;
    int def_params_ret = deflateParams(&strm, 5, 0);
    int def_reset_ret = deflateReset(&strm);

    // step 3: Operate
    int def_ret = deflate(&strm, 4);
    uLong comp_len = strm.total_out;
    int def_end_ret = deflateEnd(&strm);
    uLong checksum = adler32(1UL, src, (uInt)src_len);

    // step 4: Validate & Cleanup
    gzFile gzf_w = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gw_ret = gzwrite(gzf_w, comp_buf, (unsigned int)comp_len);
    off64_t offset_after_write = gzoffset64(gzf_w);
    int is_direct = gzdirect(gzf_w);
    int gzclose_w = gzclose(gzf_w);
    free(comp_buf);
    (void)def_init_ret;
    (void)def_params_ret;
    (void)def_reset_ret;
    (void)def_ret;
    (void)comp_len;
    (void)def_end_ret;
    (void)checksum;
    (void)gw_ret;
    (void)offset_after_write;
    (void)is_direct;
    (void)gzclose_w;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}