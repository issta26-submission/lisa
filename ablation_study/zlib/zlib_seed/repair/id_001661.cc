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
//<ID> 1661
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence: deflate -> write gz -> read gz -> inflateSyncPoint";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *ver = zlibVersion();
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    int dinit_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int iinit_ret = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));

    // step 2: Configure
    def_strm.next_in = (Bytef *)src;
    def_strm.avail_in = (uInt)src_len;
    uLong comp_bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_bound);
    memset(comp_buf, 0, (int)comp_bound);
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)comp_bound;

    // step 3: Operate
    int def_ret = deflate(&def_strm, 4);
    uLong comp_len = def_strm.total_out;
    gzFile gzf_out = gzopen("test_zlib_api_sequence.gz", "wb");
    int gw_ret = gzwrite(gzf_out, comp_buf, (unsigned int)comp_len);
    int gf_ret = gzflush(gzf_out, 0);
    int gclose_out_ret = gzclose(gzf_out);
    gzFile gzf_in = gzopen("test_zlib_api_sequence.gz", "rb");
    int first_byte = gzgetc_(gzf_in);
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)comp_len;
    int sync_ret = inflateSyncPoint(&inf_strm);

    // step 4: Validate & Cleanup
    int dend_ret = deflateEnd(&def_strm);
    int iend_ret = inflateEnd(&inf_strm);
    int gclose_in_ret = gzclose(gzf_in);
    free(comp_buf);
    (void)dinit_ret;
    (void)iinit_ret;
    (void)def_ret;
    (void)comp_bound;
    (void)comp_len;
    (void)gw_ret;
    (void)gf_ret;
    (void)gclose_out_ret;
    (void)first_byte;
    (void)sync_ret;
    (void)dend_ret;
    (void)iend_ret;
    (void)gclose_in_ret;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}