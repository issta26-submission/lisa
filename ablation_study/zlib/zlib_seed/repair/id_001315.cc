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
//<ID> 1315
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src_data[] = "zlib API sequence payload: initialize compressor, use compress(), write gz, read gz, inflate codes used";
    uLong src_len = (uLong)(sizeof(src_data) - 1);
    uLong bound_for_compress = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound_for_compress);
    memset(comp_buf, 0, (int)bound_for_compress);
    uLongf comp_len = (uLongf)bound_for_compress;
    int comp_ret = compress(comp_buf, &comp_len, src_data, src_len);
    const char *version = zlibVersion();

    // step 2: Configure
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    uLong def_bound = deflateBound(&def_strm, src_len);
    Bytef *def_out = (Bytef *)malloc((size_t)def_bound);
    memset(def_out, 0, (int)def_bound);
    def_strm.next_in = (Bytef *)src_data;
    def_strm.avail_in = (uInt)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)src_len);
    def_strm.next_out = def_out;
    def_strm.avail_out = (uInt)(def_bound > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)def_bound);

    // step 3: Operate
    int def_ret = deflate(&def_strm, 4);
    uLong deflated_size = def_strm.total_out;
    int def_end_ret = deflateEnd(&def_strm);
    gzFile gz_w = gzopen("test_zlib_api_sequence.gz", "wb");
    int gw = gzwrite(gz_w, src_data, (unsigned int)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)src_len));
    int close_w = gzclose(gz_w);
    gzFile gz_r = gzopen("test_zlib_api_sequence.gz", "rb");
    uLong out_buf_len = src_len + 64;
    Bytef *out_buf = (Bytef *)malloc((size_t)out_buf_len);
    memset(out_buf, 0, (int)out_buf_len);
    int gz_read_ret = gzread(gz_r, (voidp)out_buf, (unsigned int)(out_buf_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)out_buf_len));

    // step 4: Validate & Cleanup
    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)(comp_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)comp_len);
    uLong out2_len = src_len + 128;
    Bytef *out2 = (Bytef *)malloc((size_t)out2_len);
    memset(out2, 0, (int)out2_len);
    inf_strm.next_out = out2;
    inf_strm.avail_out = (uInt)(out2_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)out2_len);
    int inf_ret = inflate(&inf_strm, 4);
    unsigned long codes_used = inflateCodesUsed(&inf_strm);
    int inf_end_ret = inflateEnd(&inf_strm);
    int close_r = gzclose(gz_r);

    free(comp_buf);
    free(def_out);
    free(out_buf);
    free(out2);

    (void)comp_ret;
    (void)version;
    (void)def_init_ret;
    (void)def_ret;
    (void)deflated_size;
    (void)def_end_ret;
    (void)gw;
    (void)close_w;
    (void)gz_read_ret;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)codes_used;
    (void)inf_end_ret;
    (void)close_r;
    // API sequence test completed successfully
    return 66;
}