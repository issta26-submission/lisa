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
//<ID> 1667
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: deflate -> write gz -> reopen -> inspect -> inflateSyncPoint -> cleanup";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *ver = zlibVersion();
    z_stream def_strm;
    z_stream inf_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    def_strm.next_in = (Bytef *)src;
    def_strm.avail_in = (uInt)src_len;

    // step 2: Configure
    uLong comp_bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_bound);
    memset(comp_buf, 0, (int)comp_bound);
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)comp_bound;

    // step 3: Operate
    int def_ret1 = deflate(&def_strm, 0);
    int def_ret2 = deflate(&def_strm, 4);
    uLong comp_len = def_strm.total_out;
    gzFile gzf = gzopen("test_zlib_api_sequence.gz", "wb");
    int gw_ret = 0;
    if (gzf) gw_ret = gzwrite(gzf, comp_buf, (unsigned int)comp_len);
    int gf_ret = 0;
    if (gzf) gf_ret = gzflush(gzf, 2);
    int gzclose_ret1 = 0;
    if (gzf) gzclose_ret1 = gzclose(gzf);
    gzFile gzf2 = gzopen("test_zlib_api_sequence.gz", "rb");
    int gc_ret = 0;
    if (gzf2) gc_ret = gzgetc_(gzf2);
    int gzclose_ret2 = 0;
    if (gzf2) gzclose_ret2 = gzclose(gzf2);

    // step 4: Validate & Cleanup
    int inf_init_ret = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)comp_len;
    int sync_point = inflateSyncPoint(&inf_strm);
    int def_end_ret = deflateEnd(&def_strm);
    int inf_end_ret = inflateEnd(&inf_strm);
    free(comp_buf);
    (void)def_init_ret;
    (void)def_ret1;
    (void)def_ret2;
    (void)comp_bound;
    (void)gw_ret;
    (void)gf_ret;
    (void)gzclose_ret1;
    (void)gc_ret;
    (void)gzclose_ret2;
    (void)inf_init_ret;
    (void)sync_point;
    (void)def_end_ret;
    (void)inf_end_ret;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}