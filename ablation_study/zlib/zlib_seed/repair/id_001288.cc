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
//<ID> 1288
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const Bytef src_data[] = "zlib API sequence payload: compress with deflate, then inflate the result, and exercise gzseek/gzclose variants";
    uLong src_len = (uLong)(sizeof(src_data) - 1);
    const char *version = zlibVersion();
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    uLong bound = deflateBound(&def_strm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);

    // step 2: Configure and compress with deflate
    def_strm.next_in = (Bytef *)src_data;
    def_strm.avail_in = (uInt)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)src_len);
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)(bound > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)bound);
    int def_ret = deflate(&def_strm, 4);
    uLong comp_size = def_strm.total_out;
    int def_end_ret = deflateEnd(&def_strm);

    // step 3: Initialize inflate, decompress the in-memory compressed data, query inflateCodesUsed
    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    Bytef *out_buf = (Bytef *)malloc((size_t)src_len + 16);
    memset(out_buf, 0, (int)(src_len + 16));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)(comp_size > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)comp_size);
    inf_strm.next_out = out_buf;
    inf_strm.avail_out = (uInt)((src_len + 16) > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)(src_len + 16));
    int inf_ret = inflate(&inf_strm, 4);
    unsigned long codes_used = inflateCodesUsed(&inf_strm);
    int inf_end_ret = inflateEnd(&inf_strm);

    // step 4: File-level API usage and cleanup — exercise gzseek, gzclose_w and gzclose_r
    gzFile gw = gzopen("test_zlib_api_sequence.gz", "wb");
    off_t gw_pos_before = gzseek(gw, 0, 0);
    int gw_close_ret = gzclose_w(gw);
    gzFile gr = gzopen("test_zlib_api_sequence.gz", "rb");
    off_t gr_pos = gzseek(gr, 0, 0);
    int gr_close_ret = gzclose_r(gr);

    // cleanup allocated buffers and silence unused-variable warnings
    free(comp_buf);
    free(out_buf);
    (void)version;
    (void)def_init_ret;
    (void)bound;
    (void)def_ret;
    (void)comp_size;
    (void)def_end_ret;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)codes_used;
    (void)inf_end_ret;
    (void)gw_pos_before;
    (void)gw_close_ret;
    (void)gr_pos;
    (void)gr_close_ret;
    // API sequence test completed successfully
    return 66;
}