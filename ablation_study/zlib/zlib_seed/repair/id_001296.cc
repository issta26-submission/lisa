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
//<ID> 1296
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src_data[] = "zlib API sequence payload: initialize deflater, write compressed data, read back and inflate, query CRC table and EOF";
    uLong src_len = (uLong)(sizeof(src_data) - 1);
    const char *version = zlibVersion();
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    uLong bound = deflateBound(&def_strm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    def_strm.next_in = (Bytef *)src_data;
    def_strm.avail_in = (uInt)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)src_len);
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)(bound > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)bound);
    int def_ret = deflate(&def_strm, 4);
    uLong comp_size = def_strm.total_out;

    // step 2: Configure (write compressed data and query position)
    gzFile gz_w = gzopen64("test_zlib_api_sequence.gz", "wb");
    int gz_write_ret = gzwrite(gz_w, comp_buf, (unsigned int)(comp_size > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)comp_size));
    off64_t write_pos = gzseek64(gz_w, 0, 1);
    int gzclose_w_ret = gzclose(gz_w);

    // step 3: Operate (read compressed data back, prepare inflater, use inflateReset, query CRC table and EOF)
    gzFile gz_r = gzopen64("test_zlib_api_sequence.gz", "rb");
    Bytef *comp_read = (Bytef *)malloc((size_t)bound);
    memset(comp_read, 0, (int)bound);
    z_size_t read_cnt = gzfread(comp_read, (z_size_t)1, (z_size_t)(comp_size > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (z_size_t)comp_size), gz_r);
    int eof_flag = gzeof(gz_r);
    const z_crc_t *crc_table = get_crc_table();
    z_crc_t crc0 = crc_table[0];
    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    inf_strm.next_in = comp_read;
    inf_strm.avail_in = (uInt)(read_cnt > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)read_cnt);
    uLong out_buf_len = src_len + 64;
    Bytef *out_buf = (Bytef *)malloc((size_t)out_buf_len);
    memset(out_buf, 0, (int)out_buf_len);
    inf_strm.next_out = out_buf;
    inf_strm.avail_out = (uInt)(out_buf_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)out_buf_len);
    int inf_reset_ret = inflateReset(&inf_strm);
    int inf_ret = inflate(&inf_strm, 4);

    // step 4: Validate and cleanup
    int inf_end_ret = inflateEnd(&inf_strm);
    int def_end_ret = deflateEnd(&def_strm);
    int gzclose_r_ret = gzclose(gz_r);
    free(comp_buf);
    free(comp_read);
    free(out_buf);
    (void)def_init_ret;
    (void)def_ret;
    (void)comp_size;
    (void)gz_write_ret;
    (void)write_pos;
    (void)gzclose_w_ret;
    (void)read_cnt;
    (void)eof_flag;
    (void)crc_table;
    (void)crc0;
    (void)inf_init_ret;
    (void)inf_reset_ret;
    (void)inf_ret;
    (void)inf_end_ret;
    (void)def_end_ret;
    (void)version;
    // API sequence test completed successfully
    return 66;
}