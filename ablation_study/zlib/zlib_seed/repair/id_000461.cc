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
//<ID> 461
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
    int init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));

    // step 2: Configure
    Bytef src[256];
    memset(src, 'A', sizeof(src));
    def_strm.next_in = src;
    def_strm.avail_in = (uInt)sizeof(src);

    Bytef outbuf[2048];
    memset(outbuf, 0, sizeof(outbuf));
    def_strm.next_out = outbuf;
    def_strm.avail_out = (uInt)sizeof(outbuf);

    // step 3: Operate
    int reset_ret = deflateReset(&def_strm);
    unsigned int pending = 0;
    int bits = 0;
    int pending_ret = deflatePending(&def_strm, &pending, &bits);
    int deflate_ret = deflate(&def_strm, 4); /* 4 == Z_FINISH */
    uLong produced = def_strm.total_out;
    unsigned long codes_used = inflateCodesUsed(&def_strm);

    const char *filename = "test_zlib_api_sequence.tmp.gz";
    gzFile gzw = gzopen(filename, "wb");
    int gzwrite_ret = gzwrite(gzw, outbuf, (unsigned int)produced);
    int gzclose_w_ret = gzclose(gzw);

    // step 4: Validate / Cleanup
    gzFile gzr = gzopen(filename, "rb");
    char readbuf[512];
    memset(readbuf, 0, sizeof(readbuf));
    char *gets_ret = gzgets(gzr, readbuf, (int)sizeof(readbuf));
    int gzclose_r_ret = gzclose(gzr);
    int deflate_end_ret = deflateEnd(&def_strm);

    (void)ver;
    (void)init_ret;
    (void)reset_ret;
    (void)pending_ret;
    (void)pending;
    (void)bits;
    (void)deflate_ret;
    (void)produced;
    (void)codes_used;
    (void)gzwrite_ret;
    (void)gzclose_w_ret;
    (void)gets_ret;
    (void)gzclose_r_ret;
    (void)deflate_end_ret;
    // API sequence test completed successfully
    return 66;
}