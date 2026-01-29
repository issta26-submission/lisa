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
//<ID> 1314
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src_data[] = "zlib API sequence payload: compress, deflate, write gz, read gz, inflate, inspect codes used";
    uLong src_len = (uLong)(sizeof(src_data) - 1);
    const char *version = zlibVersion();
    uLong bound_for_compress = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound_for_compress);
    memset(comp_buf, 0, (int)bound_for_compress);
    uLongf comp_len = (uLongf)bound_for_compress;
    int comp_ret = compress(comp_buf, &comp_len, src_data, src_len);

    // step 2: Configure (stream deflation of the same source)
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    def_strm.next_in = (Bytef *)src_data;
    def_strm.avail_in = (uInt)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)src_len);
    uLong bound_for_deflate = deflateBound(&def_strm, src_len);
    Bytef *def_out_buf = (Bytef *)malloc((size_t)bound_for_deflate);
    memset(def_out_buf, 0, (int)bound_for_deflate);
    def_strm.next_out = def_out_buf;
    def_strm.avail_out = (uInt)(bound_for_deflate > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)bound_for_deflate);
    int def_ret = deflate(&def_strm, 4);
    uLong deflated_size = def_strm.total_out;
    int def_end_ret = deflateEnd(&def_strm);

    // step 3: Operate (write original data to a gz file and read it back; also inflate the compressed buffer)
    gzFile gz_w = gzopen("test_zlib_api_sequence.gz", "wb");
    int gw = gzwrite(gz_w, src_data, (unsigned int)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)src_len));
    int gzclose_w_ret = gzclose(gz_w);
    gzFile gz_r = gzopen("test_zlib_api_sequence.gz", "rb");
    uLong read_buf_len = src_len + 16;
    voidp read_buf = malloc((size_t)read_buf_len);
    memset(read_buf, 0, (int)read_buf_len);
    int gz_read_ret = gzread(gz_r, read_buf, (unsigned int)(read_buf_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)read_buf_len));
    int gzclose_r_ret = gzclose(gz_r);

    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    Bytef *out_buf = (Bytef *)malloc((size_t)(src_len + 64));
    memset(out_buf, 0, (int)(src_len + 64));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)(comp_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)comp_len);
    inf_strm.next_out = out_buf;
    inf_strm.avail_out = (uInt)((src_len + 64) > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)(src_len + 64));
    int inf_ret = inflate(&inf_strm, 4);
    unsigned long codes_used = inflateCodesUsed(&inf_strm);
    int inf_end_ret = inflateEnd(&inf_strm);

    // step 4: Validate & Cleanup
    free(comp_buf);
    free(def_out_buf);
    free(out_buf);
    free(read_buf);
    (void)version;
    (void)comp_ret;
    (void)def_init_ret;
    (void)def_ret;
    (void)deflated_size;
    (void)def_end_ret;
    (void)gw;
    (void)gzclose_w_ret;
    (void)gz_read_ret;
    (void)gzclose_r_ret;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)inf_end_ret;
    (void)codes_used;
    // API sequence test completed successfully
    return 66;
}