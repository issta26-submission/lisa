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
//<ID> 1313
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src_data[] = "zlib API sequence payload: compress, gz write/read, inflate and inspect codes used";
    uLong src_len = (uLong)(sizeof(src_data) - 1);
    const char *version = zlibVersion();
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    uLong est_def_bound = deflateBound(&def_strm, src_len);
    uLong comp_bound = compressBound(src_len);
    uLongf comp_len = (uLongf)comp_bound;
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_bound);
    memset(comp_buf, 0, (int)comp_bound);
    int comp_ret = compress(comp_buf, &comp_len, src_data, src_len);

    // step 2: Configure
    gzFile gz_w = gzopen("test_zlib_api_sequence.gz", "wb");
    int gw = gzwrite(gz_w, src_data, (unsigned int)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)src_len));
    int gw_close = gzclose(gz_w);

    // step 3: Operate
    gzFile gz_r = gzopen("test_zlib_api_sequence.gz", "rb");
    uLong read_buf_len = src_len + 16;
    Bytef *read_buf = (Bytef *)malloc((size_t)read_buf_len);
    memset(read_buf, 0, (int)read_buf_len);
    int gr = gzread(gz_r, (voidp)read_buf, (unsigned int)(read_buf_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)read_buf_len));
    int gr_close = gzclose(gz_r);

    // step 4: Validate and Cleanup
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
    int inf_end = inflateEnd(&inf_strm);
    int def_end = deflateEnd(&def_strm);

    free(comp_buf);
    free(read_buf);
    free(out_buf);

    (void)def_init_ret;
    (void)est_def_bound;
    (void)comp_bound;
    (void)comp_len;
    (void)comp_ret;
    (void)gw;
    (void)gw_close;
    (void)gr;
    (void)gr_close;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)codes_used;
    (void)inf_end;
    (void)def_end;
    (void)version;

    // API sequence test completed successfully
    return 66;
}