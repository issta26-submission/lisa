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
//<ID> 1312
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src_data[] = "zlib API sequence payload demonstrating compress, deflate, gzread and inflateCodesUsed";
    uLong src_len = (uLong)(sizeof(src_data) - 1);
    const char *version = zlibVersion();
    uLong bound_comp = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound_comp);
    memset(comp_buf, 0, (int)bound_comp);
    uLongf comp_len = (uLongf)bound_comp;
    int comp_ret = compress(comp_buf, &comp_len, src_data, src_len);

    // step 2: Configure (deflater using deflateInit_, deflate and deflateEnd)
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    def_strm.next_in = (Bytef *)src_data;
    def_strm.avail_in = (uInt)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)src_len);
    uLong def_bound = deflateBound(&def_strm, src_len);
    Bytef *def_out = (Bytef *)malloc((size_t)def_bound);
    memset(def_out, 0, (int)def_bound);
    def_strm.next_out = def_out;
    def_strm.avail_out = (uInt)(def_bound > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)def_bound);
    int def_ret = deflate(&def_strm, 4);
    uLong def_comp_size = def_strm.total_out;
    int def_end_ret = deflateEnd(&def_strm);

    // step 3: Operate (write compressed data to gz and read it back using gzread)
    gzFile gz_w = gzopen("test_zlib_api_sequence.gz", "wb");
    int gw = gzwrite(gz_w, comp_buf, (unsigned int)(comp_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)comp_len));
    int close_w = gzclose(gz_w);
    gzFile gz_r = gzopen("test_zlib_api_sequence.gz", "rb");
    Bytef *read_buf = (Bytef *)malloc((size_t)comp_len);
    memset(read_buf, 0, (int)comp_len);
    int gr = gzread(gz_r, (voidp)read_buf, (unsigned int)(comp_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)comp_len));
    int close_r = gzclose(gz_r);

    // step 4: Validate and Cleanup (inflate read data, query inflateCodesUsed, free resources)
    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    inf_strm.next_in = read_buf;
    inf_strm.avail_in = (uInt)(gr > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)gr);
    uLong out_len = src_len + 64;
    Bytef *out_buf = (Bytef *)malloc((size_t)out_len);
    memset(out_buf, 0, (int)out_len);
    inf_strm.next_out = out_buf;
    inf_strm.avail_out = (uInt)(out_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)out_len);
    int inf_ret = inflate(&inf_strm, 4);
    unsigned long codes_used = inflateCodesUsed(&inf_strm);
    int inf_end = inflateEnd(&inf_strm);

    free(comp_buf);
    free(def_out);
    free(read_buf);
    free(out_buf);

    (void)version;
    (void)comp_ret;
    (void)def_init_ret;
    (void)def_ret;
    (void)def_comp_size;
    (void)def_end_ret;
    (void)gw;
    (void)close_w;
    (void)gr;
    (void)close_r;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)inf_end;
    (void)codes_used;
    // API sequence test completed successfully
    return 66;
}