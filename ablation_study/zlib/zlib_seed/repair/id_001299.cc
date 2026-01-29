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
//<ID> 1299
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src_data[] = "zlib API sequence payload: initialize deflater, deflate, write gz, seek, check EOF, reset inflater and inflate";
    uLong src_len = (uLong)(sizeof(src_data) - 1);
    const char *version = zlibVersion();
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    uLong bound = deflateBound(&def_strm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);

    // step 2: Configure
    def_strm.next_in = (Bytef *)src_data;
    def_strm.avail_in = (uInt)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)src_len);
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)(bound > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)bound);

    // step 3: Operate
    int def_ret = deflate(&def_strm, 4);
    uLong comp_size = def_strm.total_out;
    int def_end_ret = deflateEnd(&def_strm);
    gzFile gzf = gzopen64("test_zlib_api_sequence.gz", "wb");
    int gz_write_ret = gzwrite(gzf, comp_buf, (unsigned int)(comp_size > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)comp_size));
    off64_t gz_pos = gzseek64(gzf, 0, 1);
    int eof_flag = gzeof(gzf);
    int gzclose_ret = gzclose(gzf);

    // step 4: Validate and cleanup
    const z_crc_t *crc_tbl = get_crc_table();
    z_crc_t first_crc_entry = crc_tbl[0];
    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    int inf_reset_ret = inflateReset(&inf_strm);
    uLong out_buf_len = src_len + 64;
    Bytef *out_buf = (Bytef *)malloc((size_t)out_buf_len);
    memset(out_buf, 0, (int)out_buf_len);
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)(comp_size > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)comp_size);
    inf_strm.next_out = out_buf;
    inf_strm.avail_out = (uInt)(out_buf_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)out_buf_len);
    int inf_ret = inflate(&inf_strm, 4);
    int inf_end_ret = inflateEnd(&inf_strm);

    free(comp_buf);
    free(out_buf);
    (void)def_init_ret;
    (void)bound;
    (void)def_ret;
    (void)comp_size;
    (void)def_end_ret;
    (void)gz_write_ret;
    (void)gz_pos;
    (void)eof_flag;
    (void)gzclose_ret;
    (void)crc_tbl;
    (void)first_crc_entry;
    (void)inf_init_ret;
    (void)inf_reset_ret;
    (void)inf_ret;
    (void)inf_end_ret;
    (void)version;
    // API sequence test completed successfully
    return 66;
}