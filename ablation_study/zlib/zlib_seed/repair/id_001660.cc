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
//<ID> 1660
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: deflate -> gzopen/gzwrite/gzflush -> gzopen/gzgetc_ -> inflate -> inflateSyncPoint";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *ver = zlibVersion();

    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    def_strm.next_in = (Bytef *)src;
    def_strm.avail_in = (uInt)src_len;

    uLong comp_bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_bound);
    memset(comp_buf, 0, (int)comp_bound);
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)comp_bound;

    // step 2: Operate (deflate -> write compressed bytes to a gz file and flush)
    int def_ret = deflate(&def_strm, 4);
    uLong comp_len = def_strm.total_out;
    gzFile gzf_w = gzopen("test_zlib_api_sequence.gz", "wb");
    int gw_ret = gzwrite(gzf_w, comp_buf, (unsigned int)comp_len);
    int gf_ret = gzflush(gzf_w, 2);
    int gz_close_w = gzclose(gzf_w);

    // step 3: Read back one byte with gzgetc_ and then inflate the compressed buffer
    gzFile gzf_r = gzopen("test_zlib_api_sequence.gz", "rb");
    int first_byte = gzgetc_(gzf_r);

    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)comp_len;

    Bytef *decomp_buf = (Bytef *)malloc((size_t)src_len);
    memset(decomp_buf, 0, (int)src_len);
    inf_strm.next_out = decomp_buf;
    inf_strm.avail_out = (uInt)src_len;

    int inf_ret = inflate(&inf_strm, 0);
    int inf_sync = inflateSyncPoint(&inf_strm);

    // step 4: Validate & Cleanup
    int gz_close_r = gzclose(gzf_r);
    int def_end_ret = deflateEnd(&def_strm);
    int inf_end_ret = inflateEnd(&inf_strm);
    free(comp_buf);
    free(decomp_buf);

    (void)def_init_ret;
    (void)def_ret;
    (void)comp_bound;
    (void)gw_ret;
    (void)gf_ret;
    (void)gz_close_w;
    (void)first_byte;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)inf_sync;
    (void)gz_close_r;
    (void)def_end_ret;
    (void)inf_end_ret;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}