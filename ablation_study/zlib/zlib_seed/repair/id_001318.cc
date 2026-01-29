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
//<ID> 1318
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src_data[] = "zlib API sequence payload for compression and gzip IO";
    uLong src_len = (uLong)(sizeof(src_data) - 1);
    const char *version = zlibVersion();
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    uLong bound = deflateBound(&def_strm, src_len);
    Bytef *def_comp_buf = (Bytef *)malloc((size_t)bound);
    memset(def_comp_buf, 0, (int)bound);
    def_strm.next_in = (Bytef *)src_data;
    def_strm.avail_in = (uInt)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)src_len);
    def_strm.next_out = def_comp_buf;
    def_strm.avail_out = (uInt)(bound > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)bound);
    int def_ret = deflate(&def_strm, 4);
    uLong def_out_size = def_strm.total_out;
    int def_end_ret = deflateEnd(&def_strm);

    // step 2: Configure (single-shot compress)
    uLongf comp_len = (uLongf)compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_len);
    memset(comp_buf, 0, (int)comp_len);
    int comp_ret = compress(comp_buf, &comp_len, src_data, src_len);

    // step 3: Operate (write original data to a gz file and read it back)
    gzFile gz_w = gzopen("test_zlib_api_sequence.gz", "wb");
    int gw = gzwrite(gz_w, src_data, (unsigned int)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)src_len));
    int close_w_ret = gzclose(gz_w);
    gzFile gz_r = gzopen("test_zlib_api_sequence.gz", "rb");
    uLong read_buf_len = src_len + 16;
    Bytef *read_buf = (Bytef *)malloc((size_t)read_buf_len);
    memset(read_buf, 0, (int)read_buf_len);
    int gr = gzread(gz_r, read_buf, (unsigned int)(read_buf_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)read_buf_len));
    int close_r_ret = gzclose(gz_r);

    // step 4: Validate & Cleanup (inflate the single-shot compressed buffer and query codes used)
    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)(comp_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)comp_len);
    uLong out_len = src_len + 64;
    Bytef *out_buf = (Bytef *)malloc((size_t)out_len);
    memset(out_buf, 0, (int)out_len);
    inf_strm.next_out = out_buf;
    inf_strm.avail_out = (uInt)(out_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)out_len);
    int inf_ret = inflate(&inf_strm, 4);
    unsigned long codes_used = inflateCodesUsed(&inf_strm);
    int inf_end_ret = inflateEnd(&inf_strm);

    free(def_comp_buf);
    free(comp_buf);
    free(read_buf);
    free(out_buf);

    (void)def_init_ret;
    (void)bound;
    (void)def_ret;
    (void)def_out_size;
    (void)def_end_ret;
    (void)comp_ret;
    (void)gw;
    (void)close_w_ret;
    (void)gr;
    (void)close_r_ret;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)inf_end_ret;
    (void)codes_used;
    (void)version;
    // API sequence test completed successfully
    return 66;
}