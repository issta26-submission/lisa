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
//<ID> 469
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
    z_stream inf_strm;
    memset(&inf_strm, 0, sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));

    // step 2: Configure
    Bytef input[512];
    memset(input, 'A', sizeof(input));
    def_strm.next_in = input;
    def_strm.avail_in = (uInt)sizeof(input);
    Bytef compbuf[1024];
    memset(compbuf, 0, sizeof(compbuf));
    def_strm.next_out = compbuf;
    def_strm.avail_out = (uInt)sizeof(compbuf);

    // step 3: Operate
    int def_ret = deflate(&def_strm, 4); /* 4 == Z_FINISH */
    uLong produced = def_strm.total_out;
    unsigned int pending;
    int bits;
    int pending_ret = deflatePending(&def_strm, &pending, &bits);
    int reset_ret = deflateReset(&def_strm);
    const char *filename = "test_zlib_api_sequence.tmp.gz";
    gzFile gzw = gzopen(filename, "wb");
    int gzwrite_ret = gzwrite(gzw, compbuf, (unsigned int)produced);
    int gzclose_w_ret = gzclose(gzw);

    // step 4: Validate / Cleanup
    inf_strm.next_in = compbuf;
    inf_strm.avail_in = (uInt)produced;
    Bytef decomp[512];
    memset(decomp, 0, sizeof(decomp));
    inf_strm.next_out = decomp;
    inf_strm.avail_out = (uInt)sizeof(decomp);
    int inf_ret = inflate(&inf_strm, 4); /* 4 == Z_FINISH */
    unsigned long codes_used = inflateCodesUsed(&inf_strm);
    gzFile gzr = gzopen(filename, "rb");
    char linebuf[128];
    memset(linebuf, 0, sizeof(linebuf));
    char *ggets_ret = gzgets(gzr, linebuf, (int)sizeof(linebuf));
    int gzclose_r_ret = gzclose(gzr);
    int def_end_ret = deflateEnd(&def_strm);
    int inf_end_ret = inflateEnd(&inf_strm);

    (void)ver;
    (void)def_init_ret;
    (void)inf_init_ret;
    (void)def_ret;
    (void)produced;
    (void)pending;
    (void)bits;
    (void)pending_ret;
    (void)reset_ret;
    (void)filename;
    (void)gzwrite_ret;
    (void)gzclose_w_ret;
    (void)inf_ret;
    (void)codes_used;
    (void)ggets_ret;
    (void)gzclose_r_ret;
    (void)def_end_ret;
    (void)inf_end_ret;
    // API sequence test completed successfully
    return 66;
}