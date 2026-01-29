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
//<ID> 464
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations / Initialize
    const char *ver = zlibVersion();
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    Bytef src[128];
    memset(src, 'A', sizeof(src));
    Bytef comp[65536];
    memset(comp, 0, sizeof(comp));

    // step 2: Configure
    def_strm.next_in = src;
    def_strm.avail_in = (uInt)sizeof(src);
    def_strm.next_out = comp;
    def_strm.avail_out = (uInt)sizeof(comp);

    // step 3: Operate
    int def_ret = deflate(&def_strm, 4); // 4 == Z_FINISH
    uLong produced = def_strm.total_out;
    gzFile gzw = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    int gzwrite_ret = gzwrite(gzw, comp, (unsigned int)produced);
    int gzclose_w_ret = gzclose(gzw);

    int def_reset_ret = deflateReset(&def_strm);
    unsigned int pending = 0;
    int bits = 0;
    int def_pending_ret = deflatePending(&def_strm, &pending, &bits);

    z_stream inf_strm;
    memset(&inf_strm, 0, sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    unsigned long codes_used = inflateCodesUsed(&inf_strm);

    gzFile gzr = gzopen("test_zlib_api_sequence.tmp.gz", "rb");
    char linebuf[256];
    memset(linebuf, 0, sizeof(linebuf));
    char *gzgets_ret = gzgets(gzr, linebuf, (int)sizeof(linebuf));
    int gzclose_r_ret = gzclose(gzr);

    // step 4: Validate / Cleanup
    int def_end_ret = deflateEnd(&def_strm);
    int inf_end_ret = inflateEnd(&inf_strm);

    (void)ver;
    (void)def_init_ret;
    (void)def_ret;
    (void)produced;
    (void)gzwrite_ret;
    (void)gzclose_w_ret;
    (void)def_reset_ret;
    (void)pending;
    (void)bits;
    (void)def_pending_ret;
    (void)inf_init_ret;
    (void)codes_used;
    (void)gzgets_ret;
    (void)gzclose_r_ret;
    (void)def_end_ret;
    (void)inf_end_ret;
    // API sequence test completed successfully
    return 66;
}