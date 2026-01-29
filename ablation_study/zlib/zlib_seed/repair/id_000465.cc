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
//<ID> 465
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
    z_stream inf_strm;
    memset(&inf_strm, 0, sizeof(inf_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    int inf_init_ret = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));

    // step 2: Configure
    Bytef src[512];
    memset(src, 'A', sizeof(src));
    def_strm.next_in = src;
    def_strm.avail_in = (uInt)sizeof(src);
    Bytef outbuf[16384];
    memset(outbuf, 0, sizeof(outbuf));
    def_strm.next_out = outbuf;
    def_strm.avail_out = (uInt)sizeof(outbuf);
    uLong bound = deflateBound(&def_strm, (uLong)def_strm.avail_in);
    (void)bound;

    // step 3: Operate
    int deflate_ret = deflate(&def_strm, 4); // Z_FINISH
    unsigned int pending = 0;
    int bits = 0;
    int pending_ret = deflatePending(&def_strm, &pending, &bits);
    int reset_ret = deflateReset(&def_strm);
    unsigned long codes_used = inflateCodesUsed(&inf_strm);

    const char *filename = "test_zlib_api_sequence.tmp.gz";
    gzFile gzw = gzopen(filename, "wb");
    int gzwrite_ret = gzwrite(gzw, outbuf, (unsigned int)def_strm.total_out);
    int gzclose_w_ret = gzclose(gzw);
    gzFile gzr = gzopen(filename, "rb");
    char readbuf[512];
    memset(readbuf, 0, sizeof(readbuf));
    char *got = gzgets(gzr, readbuf, (int)sizeof(readbuf));
    int gzclose_r_ret = gzclose(gzr);

    // step 4: Validate / Cleanup
    int deflate_end_ret = deflateEnd(&def_strm);
    int inflate_end_ret = inflateEnd(&inf_strm);

    (void)ver;
    (void)def_init_ret;
    (void)inf_init_ret;
    (void)src;
    (void)outbuf;
    (void)deflate_ret;
    (void)pending_ret;
    (void)pending;
    (void)bits;
    (void)reset_ret;
    (void)codes_used;
    (void)filename;
    (void)gzw;
    (void)gzwrite_ret;
    (void)gzclose_w_ret;
    (void)gzr;
    (void)got;
    (void)readbuf;
    (void)gzclose_r_ret;
    (void)deflate_end_ret;
    (void)inflate_end_ret;
    // API sequence test completed successfully
    return 66;
}