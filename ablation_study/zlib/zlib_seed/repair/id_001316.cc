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
//<ID> 1316
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src_data[] = "zlib API sequence payload for compress/inflate/gzread/deflateEnd test.";
    uLong src_len = (uLong)(sizeof(src_data) - 1);
    uLong bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    uLongf comp_len = (uLongf)bound;
    int comp_ret = compress(comp_buf, &comp_len, src_data, src_len);
    const char *version = zlibVersion();

    // step 2: Configure
    gzFile gzf_w = gzopen("test_zlib_api_sequence.gz", "wb");
    int gw = gzwrite(gzf_w, comp_buf, (unsigned int)comp_len);
    int gzw_close_w = gzclose(gzf_w);
    gzFile gzf_r = gzopen("test_zlib_api_sequence.gz", "rb");
    uLong read_buf_len = (uLong)comp_len + 16UL;
    Bytef *read_buf = (Bytef *)malloc((size_t)read_buf_len);
    memset(read_buf, 0, (int)read_buf_len);
    int gr = gzread(gzf_r, read_buf, (unsigned int)read_buf_len);
    int gzr_close_r = gzclose(gzf_r);

    // step 3: Operate
    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)comp_len;
    uLong out_len = src_len + 64UL;
    Bytef *out_buf = (Bytef *)malloc((size_t)out_len);
    memset(out_buf, 0, (int)out_len);
    inf_strm.next_out = out_buf;
    inf_strm.avail_out = (uInt)out_len;
    int inf_ret = inflate(&inf_strm, 4);
    unsigned long codes_used = inflateCodesUsed(&inf_strm);
    int inf_end_ret = inflateEnd(&inf_strm);

    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    int def_end_ret = deflateEnd(&def_strm);

    // step 4: Validate and cleanup
    free(comp_buf);
    free(read_buf);
    free(out_buf);
    (void)comp_ret;
    (void)version;
    (void)gw;
    (void)gzw_close_w;
    (void)gr;
    (void)gzr_close_r;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)codes_used;
    (void)inf_end_ret;
    (void)def_init_ret;
    (void)def_end_ret;
    // API sequence test completed successfully
    return 66;
}