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
//<ID> 1307
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src_data[] = "zlib API sequence payload: compress to memory, inflate, write/read gz, compute crc combine and cleanup";
    uLong src_len = (uLong)(sizeof(src_data) - 1);
    uLong bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    uLongf comp_len = (uLongf)bound;
    int comp_ret = compress2(comp_buf, &comp_len, src_data, src_len, 6);

    gzFile gz_w = gzopen("test_zlib_api_sequence.gz", "wb");
    int gz_write_ret = gzwrite(gz_w, src_data, (unsigned int)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)src_len));
    int gzclose_w_ret = gzclose(gz_w);

    const char *version = zlibVersion();
    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));

    // step 2: Configure
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)(comp_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)comp_len);
    uLong out_buf_len = src_len + 64;
    Bytef *out_buf = (Bytef *)malloc((size_t)out_buf_len);
    memset(out_buf, 0, (int)out_buf_len);
    inf_strm.next_out = out_buf;
    inf_strm.avail_out = (uInt)(out_buf_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)out_buf_len);

    // step 3: Operate & Validate
    int inf_ret = inflate(&inf_strm, 4);
    int back_end_ret = inflateBackEnd(&inf_strm);
    int inf_end_ret = inflateEnd(&inf_strm);

    uInt len1 = (uInt)(src_len / 2);
    uInt len2 = (uInt)(src_len - (uLong)len1);
    uLong crc1 = crc32(0UL, out_buf, len1);
    uLong crc2 = crc32(0UL, out_buf + len1, len2);
    uLong crc_comb = crc32_combine(crc1, crc2, (off_t)len2);

    gzFile gz_r = gzopen("test_zlib_api_sequence.gz", "rb");
    int first_char = gzgetc(gz_r);
    int gzclose_r_ret = gzclose(gz_r);

    // step 4: Cleanup
    free(comp_buf);
    free(out_buf);
    (void)comp_ret;
    (void)bound;
    (void)comp_len;
    (void)gz_write_ret;
    (void)gzclose_w_ret;
    (void)version;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)back_end_ret;
    (void)inf_end_ret;
    (void)crc1;
    (void)crc2;
    (void)crc_comb;
    (void)first_char;
    (void)gzclose_r_ret;
    // API sequence test completed successfully
    return 66;
}