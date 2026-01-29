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
//<ID> 658
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    Bytef plain_in[1024];
    memset(plain_in, 'A', sizeof(plain_in));
    uLong plain_len = (uLong)sizeof(plain_in);
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));

    // step 2: Configure
    uLong bound_comp = compressBound(plain_len);
    uLongf comp1_len = (uLongf)bound_comp;
    Bytef *comp1 = (Bytef *)malloc((size_t)bound_comp);
    memset(comp1, 0, (size_t)bound_comp);
    int comp_ret = compress(comp1, &comp1_len, plain_in, plain_len);

    def_strm.next_in = plain_in;
    def_strm.avail_in = (uInt)plain_len;
    uLong def_bound = deflateBound(&def_strm, plain_len);
    Bytef *comp2 = (Bytef *)malloc((size_t)def_bound);
    memset(comp2, 0, (size_t)def_bound);
    def_strm.next_out = comp2;
    def_strm.avail_out = (uInt)def_bound;

    // step 3: Operate
    int def_ret = deflate(&def_strm, 4); /* 4 == Z_FINISH */
    uLong def_comp_len = def_strm.total_out;
    int def_end_ret = deflateEnd(&def_strm);

    gzFile gw = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gw_write = gzwrite(gw, plain_in, (unsigned int)plain_len);
    int gw_close = gzclose(gw);
    gzFile gr = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    int got_char = gzgetc(gr);
    int gr_close = gzclose(gr);

    // step 4: Validate / Cleanup
    uLong crc = crc32(0L, plain_in, (uInt)plain_len);
    (void)version;
    (void)def_init_ret;
    (void)bound_comp;
    (void)comp1_len;
    (void)comp_ret;
    (void)def_bound;
    (void)def_ret;
    (void)def_comp_len;
    (void)def_end_ret;
    (void)gw_write;
    (void)gw_close;
    (void)got_char;
    (void)gr_close;
    (void)crc;
    free(comp1);
    free(comp2);
    // API sequence test completed successfully
    return 66;
}