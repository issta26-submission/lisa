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
//<ID> 650
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    Bytef src[128];
    memset(src, 'A', sizeof(src));
    uLong src_len = (uLong)sizeof(src);
    Bytef comp1[256];
    memset(comp1, 0, sizeof(comp1));
    uLongf comp1_len = (uLongf)sizeof(comp1);
    int comp_ret = compress(comp1, &comp1_len, src, src_len);

    // step 2: Configure deflate stream
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    int def_init_ret = deflateInit_(&strm, 6, version, (int)sizeof(z_stream));
    strm.next_in = comp1;
    strm.avail_in = (uInt)comp1_len;
    Bytef def_out[512];
    memset(def_out, 0, sizeof(def_out));
    strm.next_out = def_out;
    strm.avail_out = (uInt)sizeof(def_out);

    // step 3: Operate (deflate)
    int def_ret = deflate(&strm, 4); /* 4 == Z_FINISH */
    uLong def_total_out = strm.total_out;
    int def_end_ret = deflateEnd(&strm);

    // step 4: Validate / Cleanup using gz APIs
    gzFile gw = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gw_write = gzwrite(gw, src, (unsigned int)src_len);
    int gw_close = gzclose(gw);
    gzFile gr = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    int got_ch = gzgetc(gr);
    int gr_close = gzclose(gr);

    (void)version;
    (void)comp_ret;
    (void)comp1_len;
    (void)def_init_ret;
    (void)def_ret;
    (void)def_total_out;
    (void)def_end_ret;
    (void)gw_write;
    (void)gw_close;
    (void)got_ch;
    (void)gr_close;

    // API sequence test completed successfully
    return 66;
}