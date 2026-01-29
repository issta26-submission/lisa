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
//<ID> 1298
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src_data[] = "zlib API sequence payload: initialize deflate and inflate, compress, write to gz, read back and inflate";
    uLong src_len = (uLong)(sizeof(src_data) - 1);
    const char *version = zlibVersion();
    z_stream def_strm;
    z_stream inf_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));

    // step 2: Configure & Operate - deflate into buffer and write using gzopen64, use gzseek64 and gzeof
    uLong bound = deflateBound(&def_strm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    def_strm.next_in = (Bytef *)src_data;
    def_strm.avail_in = (uInt)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)src_len);
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)(bound > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)bound);
    int def_ret = deflate(&def_strm, 4);
    uLong comp_size = def_strm.total_out;
    gzFile gz_w = gzopen64("test_zlib_api_sequence_blob.gz", "wb");
    int gz_write_ret = gzwrite(gz_w, comp_buf, (unsigned int)(comp_size > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)comp_size));
    off64_t pos_after_write = gzseek64(gz_w, 0, 1);
    int eof_after_write = gzeof(gz_w);
    int gzclose_w_ret = gzclose(gz_w);

    // step 3: Operate & Validate - read compressed data back, reset inflate, inflate, and consult CRC table
    gzFile gz_r = gzopen64("test_zlib_api_sequence_blob.gz", "rb");
    off64_t pos_seek_start = gzseek64(gz_r, 0, 0);
    Bytef *comp_read = (Bytef *)malloc((size_t)bound);
    memset(comp_read, 0, (int)bound);
    z_size_t read_cnt = gzfread(comp_read, (z_size_t)1, (z_size_t)(comp_size > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (z_size_t)comp_size), gz_r);
    Bytef *out_buf = (Bytef *)malloc((size_t)(src_len + 64));
    memset(out_buf, 0, (int)(src_len + 64));
    inf_strm.next_in = comp_read;
    inf_strm.avail_in = (uInt)(comp_size > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)comp_size);
    inf_strm.next_out = out_buf;
    inf_strm.avail_out = (uInt)((src_len + 64) > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)(src_len + 64));
    int inf_reset_ret = inflateReset(&inf_strm);
    int inf_ret = inflate(&inf_strm, 4);
    const z_crc_t *crc_table = get_crc_table();
    z_crc_t crc0 = crc_table[0];
    int eof_after_read = gzeof(gz_r);
    int gzclose_r_ret = gzclose(gz_r);

    // step 4: Cleanup & finalize validation
    int def_end_ret = deflateEnd(&def_strm);
    int inf_end_ret = inflateEnd(&inf_strm);
    free(comp_buf);
    free(comp_read);
    free(out_buf);
    (void)version;
    (void)def_init_ret;
    (void)inf_init_ret;
    (void)bound;
    (void)def_ret;
    (void)comp_size;
    (void)gz_write_ret;
    (void)pos_after_write;
    (void)eof_after_write;
    (void)gzclose_w_ret;
    (void)pos_seek_start;
    (void)read_cnt;
    (void)inf_reset_ret;
    (void)inf_ret;
    (void)crc0;
    (void)eof_after_read;
    (void)gzclose_r_ret;
    (void)def_end_ret;
    (void)inf_end_ret;
    // API sequence test completed successfully
    return 66;
}