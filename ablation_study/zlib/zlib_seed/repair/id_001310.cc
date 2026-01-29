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
//<ID> 1310
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src_data[] = "zlib API sequence payload: compress, write/read gz, inflate and inspect codes used";
    uLong src_len = (uLong)(sizeof(src_data) - 1);
    const char *version = zlibVersion();
    uLong bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    uLongf comp_len = (uLongf)bound;
    int comp_ret = compress(comp_buf, &comp_len, src_data, src_len);

    // step 2: Setup deflater (to demonstrate deflateInit_/deflateEnd usage)
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));

    // step 3: Operate - write original data to a gz file, then read it back with gzread
    gzFile gz_w = gzopen("test_zlib_api_sequence.gz", "wb");
    int gw = gzwrite(gz_w, src_data, (unsigned int)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)src_len));
    int gzclose_w_ret = gzclose(gz_w);
    gzFile gz_r = gzopen("test_zlib_api_sequence.gz", "rb");
    Bytef *read_back = (Bytef *)malloc((size_t)src_len);
    memset(read_back, 0, (int)src_len);
    int gr = gzread(gz_r, read_back, (unsigned int)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)src_len));
    int gzclose_r_ret = gzclose(gz_r);

    // step 4: Validate & Cleanup - inflate compressed buffer, inspect codes used, and cleanup deflater
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
    int def_end_ret = deflateEnd(&def_strm);

    free(comp_buf);
    free(read_back);
    free(out_buf);

    (void)comp_ret;
    (void)bound;
    (void)def_init_ret;
    (void)gw;
    (void)gzclose_w_ret;
    (void)gr;
    (void)gzclose_r_ret;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)codes_used;
    (void)inf_end_ret;
    (void)def_end_ret;
    (void)version;
    // API sequence test completed successfully
    return 66;
}