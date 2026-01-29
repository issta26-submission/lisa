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
//<ID> 1303
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src_data[] = "zlib API sequence payload: compress, write gz, read one byte, inflate and combine crc";
    uLong src_len = (uLong)(sizeof(src_data) - 1);
    const char *version = zlibVersion();
    uLong bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    uLongf comp_len = (uLongf)bound;
    int comp_ret = compress2(comp_buf, &comp_len, src_data, src_len, 6);

    // step 2: Configure
    gzFile gz_w = gzopen("test_zlib_api_sequence.gz", "wb");
    int gz_write_ret = gzwrite(gz_w, comp_buf, (unsigned int)(comp_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)comp_len));
    int gzclose_w_ret = gzclose(gz_w);
    gzFile gz_r = gzopen("test_zlib_api_sequence.gz", "rb");

    // step 3: Operate
    int first_byte = gzgetc(gz_r);
    int gzclose_r_ret = gzclose(gz_r);
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
    int ib_end_ret = inflateBackEnd(&inf_strm);

    // step 4: Validate and Cleanup
    uLong crc1 = crc32(0L, src_data, (uInt)src_len);
    uLong crc_combined = crc32_combine(crc1, crc1, (off_t)src_len);
    int inf_end_ret = inflateEnd(&inf_strm);
    free(comp_buf);
    free(out_buf);
    (void)version;
    (void)bound;
    (void)comp_ret;
    (void)comp_len;
    (void)gz_write_ret;
    (void)gzclose_w_ret;
    (void)gzclose_r_ret;
    (void)first_byte;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)ib_end_ret;
    (void)crc1;
    (void)crc_combined;
    (void)inf_end_ret;
    // API sequence test completed successfully
    return 66;
}