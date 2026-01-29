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
//<ID> 1292
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src_data[] = "zlib API sequence payload: deflate to memory, write via gzopen64, then inflate from memory";
    uLong src_len = (uLong)(sizeof(src_data) - 1);
    const char *version = zlibVersion();
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    uLong comp_bound = deflateBound(&def_strm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_bound);
    memset(comp_buf, 0, (int)comp_bound);

    // step 2: Configure and Operate (deflate -> write gz file -> seek -> close writer)
    def_strm.next_in = (Bytef *)src_data;
    def_strm.avail_in = (uInt)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)src_len);
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)(comp_bound > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)comp_bound);
    int def_ret = deflate(&def_strm, 4);
    uLong comp_size = def_strm.total_out;
    gzFile gz_w = gzopen64("test_zlib_api_sequence.gz", "wb");
    int gz_write_ret = gzwrite(gz_w, comp_buf, (unsigned int)(comp_size > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)comp_size));
    off64_t gz_pos_after_write = gzseek64(gz_w, 0, 1);
    int gzclose_w_ret = gzclose_w(gz_w);

    // step 3: Configure and Operate (open reader, check EOF, seek, initialize inflate, reset, inflate)
    gzFile gz_r = gzopen64("test_zlib_api_sequence.gz", "rb");
    int eof_flag = gzeof(gz_r);
    off64_t gz_seek_ret = gzseek64(gz_r, 0, 0);
    Bytef *out_buf = (Bytef *)malloc((size_t)(src_len + 64));
    memset(out_buf, 0, (int)(src_len + 64));
    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    int inf_reset_ret = inflateReset(&inf_strm);
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)(comp_size > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)comp_size);
    inf_strm.next_out = out_buf;
    inf_strm.avail_out = (uInt)((src_len + 64) > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)(src_len + 64));
    int inf_ret = inflate(&inf_strm, 4);

    // step 4: Validate and cleanup
    const z_crc_t *crc_tbl = get_crc_table();
    uLong crc_seed = (uLong)(crc_tbl ? crc_tbl[0] : 0U);
    uLong crc_val = crc32(crc_seed, out_buf, (uInt)(inf_strm.total_out > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)inf_strm.total_out));
    int inf_end_ret = inflateEnd(&inf_strm);
    int def_end_ret = deflateEnd(&def_strm);
    int gzclose_r_ret = gzclose_r(gz_r);
    free(comp_buf);
    free(out_buf);
    (void)def_init_ret;
    (void)def_ret;
    (void)comp_size;
    (void)gz_write_ret;
    (void)gz_pos_after_write;
    (void)gzclose_w_ret;
    (void)eof_flag;
    (void)gz_seek_ret;
    (void)inf_init_ret;
    (void)inf_reset_ret;
    (void)inf_ret;
    (void)crc_tbl;
    (void)crc_val;
    (void)inf_end_ret;
    (void)def_end_ret;
    (void)gzclose_r_ret;
    (void)version;
    // API sequence test completed successfully
    return 66;
}