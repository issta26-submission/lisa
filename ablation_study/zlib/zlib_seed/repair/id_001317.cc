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
//<ID> 1317
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src_data[] = "zlib API sequence payload for compress/inflate and gz read test";
    uLong src_len = (uLong)(sizeof(src_data) - 1);
    const char *version = zlibVersion();
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));

    // step 2: Configure (compress in one-shot and prepare buffers)
    uLong bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    uLongf comp_len = (uLongf)bound;
    int comp_ret = compress(comp_buf, &comp_len, src_data, src_len);

    // step 3: Operate (write original data to a gz file and read it back; also end deflater)
    int def_end_ret = deflateEnd(&def_strm);
    gzFile gz_w = gzopen("test_zlib_api_sequence.gz", "wb");
    int gw = gzwrite(gz_w, src_data, (unsigned int)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)src_len));
    int gzclose_w_ret = gzclose(gz_w);
    gzFile gz_r = gzopen("test_zlib_api_sequence.gz", "rb");
    uLong read_buf_len = src_len + 16;
    Bytef *read_buf = (Bytef *)malloc((size_t)read_buf_len);
    memset(read_buf, 0, (int)read_buf_len);
    int gr = gzread(gz_r, read_buf, (unsigned int)(read_buf_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)read_buf_len));
    int gzclose_r_ret = gzclose(gz_r);

    // step 4: Validate & Cleanup (inflate the compressed buffer and query codes used)
    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)(comp_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)comp_len);
    uLong out_buf_len = src_len + 64;
    Bytef *out_buf = (Bytef *)malloc((size_t)out_buf_len);
    memset(out_buf, 0, (int)out_buf_len);
    inf_strm.next_out = out_buf;
    inf_strm.avail_out = (uInt)(out_buf_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)out_buf_len);
    int inf_ret = inflate(&inf_strm, 4);
    unsigned long codes_used = inflateCodesUsed(&inf_strm);
    int inf_end_ret = inflateEnd(&inf_strm);

    free(comp_buf);
    free(read_buf);
    free(out_buf);

    (void)def_init_ret;
    (void)bound;
    (void)comp_ret;
    (void)def_end_ret;
    (void)gw;
    (void)gzclose_w_ret;
    (void)gr;
    (void)gzclose_r_ret;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)codes_used;
    (void)inf_end_ret;
    (void)version;
    // API sequence test completed successfully
    return 66;
}