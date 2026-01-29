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
//<ID> 1714
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: deflateParams -> deflateReset -> deflate -> gzoffset64/gzdirect -> adler32";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *ver = zlibVersion();
    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    int init_ret = deflateInit_(&strm, 6, ver, (int)sizeof(z_stream));
    uLong bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    strm.next_in = (Bytef *)src;
    strm.avail_in = (uInt)src_len;
    strm.next_out = comp_buf;
    strm.avail_out = (uInt)bound;

    // step 2: Configure
    int params_ret = deflateParams(&strm, 6, 0);
    int reset_ret = deflateReset(&strm);

    // step 3: Operate
    int def_ret = deflate(&strm, 4);
    uLong comp_len = strm.total_out;
    uLong checksum = adler32(0UL, src, (uInt)src_len);
    gzFile gzf_w = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gw_ret = gzwrite(gzf_w, comp_buf, (unsigned int)comp_len);
    int direct_w = gzdirect(gzf_w);
    off64_t off_w = gzoffset64(gzf_w);
    int gzclose_w = gzclose(gzf_w);

    // step 4: Validate & Cleanup
    gzFile gzf_r = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    int direct_r = gzdirect(gzf_r);
    off64_t off_r = gzoffset64(gzf_r);
    int gzclose_r = gzclose(gzf_r);
    int def_end = deflateEnd(&strm);
    free(comp_buf);
    (void)ver;
    (void)init_ret;
    (void)params_ret;
    (void)reset_ret;
    (void)def_ret;
    (void)comp_len;
    (void)checksum;
    (void)gw_ret;
    (void)direct_w;
    (void)off_w;
    (void)gzclose_w;
    (void)direct_r;
    (void)off_r;
    (void)gzclose_r;
    (void)def_end;
    (void)bound;
    (void)src_len;
    // API sequence test completed successfully
    return 66;
}