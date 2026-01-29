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
//<ID> 1293
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src_data[] = "zlib API sequence payload: initialize deflater, produce compressed bytes, then validate via inflate and gz APIs";
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

    // step 2: Operate - deflate, write an uncompressed copy via gzopen64, query position with gzseek64, close
    int def_ret = deflate(&def_strm, 4);
    uLong comp_size = def_strm.total_out;
    gzFile gz_w = gzopen64("test_zlib_api_sequence.gz", "wb");
    int gz_write_ret = gzwrite(gz_w, src_data, (unsigned int)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)src_len));
    off64_t gz_pos_after_write = gzseek64(gz_w, 0, 1); // SEEK_CUR = 1
    int gzclose_w_ret = gzclose(gz_w);

    // step 3: Configure & Operate - open for read, read decompressed bytes via gzread, inspect EOF with gzeof, then inflate the previously produced compressed buffer
    gzFile gz_r = gzopen64("test_zlib_api_sequence.gz", "rb");
    off64_t gz_pos_seek_to_start = gzseek64(gz_r, 0, 0); // SEEK_SET = 0
    uLong out_buf_len = src_len + 128;
    Bytef *read_buf = (Bytef *)malloc((size_t)out_buf_len);
    memset(read_buf, 0, (int)out_buf_len);
    int gz_read_ret = gzread(gz_r, read_buf, (unsigned int)(out_buf_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)out_buf_len));
    int gz_eof_flag = gzeof(gz_r);
    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    int inf_reset_ret = inflateReset(&inf_strm);
    Bytef *inflated_buf = (Bytef *)malloc((size_t)out_buf_len);
    memset(inflated_buf, 0, (int)out_buf_len);
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)(comp_size > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)comp_size);
    inf_strm.next_out = inflated_buf;
    inf_strm.avail_out = (uInt)(out_buf_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)out_buf_len);
    int inf_ret = inflate(&inf_strm, 4);
    const z_crc_t *crc_table = get_crc_table();
    z_crc_t crc_first = crc_table ? crc_table[0] : (z_crc_t)0;

    // step 4: Cleanup & Validate
    int inf_end_ret = inflateEnd(&inf_strm);
    int def_end_ret = deflateEnd(&def_strm);
    int gzclose_r_ret = gzclose(gz_r);
    free(comp_buf);
    free(read_buf);
    free(inflated_buf);
    (void)version;
    (void)def_init_ret;
    (void)bound;
    (void)def_ret;
    (void)comp_size;
    (void)gz_write_ret;
    (void)gz_pos_after_write;
    (void)gzclose_w_ret;
    (void)gz_pos_seek_to_start;
    (void)gz_read_ret;
    (void)gz_eof_flag;
    (void)inf_init_ret;
    (void)inf_reset_ret;
    (void)inf_ret;
    (void)crc_first;
    (void)inf_end_ret;
    (void)def_end_ret;
    (void)gzclose_r_ret;
    // API sequence test completed successfully
    return 66;
}