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
//<ID> 1290
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src_data[] = "zlib API sequence payload: initialize deflater, write via gzopen64, query position and eof, reset inflater";
    uLong src_len = (uLong)(sizeof(src_data) - 1);
    const char *version = zlibVersion();
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    int inf_reset_ret = inflateReset(&inf_strm);
    const z_crc_t *crc_table = get_crc_table();
    z_crc_t first_crc = crc_table[0];

    // step 2: Configure
    uLong out_buf_len = src_len + 128;
    Bytef *out_buf = (Bytef *)malloc((size_t)out_buf_len);
    memset(out_buf, 0, (int)out_buf_len);
    def_strm.next_in = (Bytef *)src_data;
    def_strm.avail_in = (uInt)src_len;
    def_strm.next_out = out_buf;
    def_strm.avail_out = (uInt)out_buf_len;

    // step 3: Operate
    int def_ret = deflate(&def_strm, 4);
    uLong comp_size = def_strm.total_out;
    gzFile gz_w = gzopen64("test_zlib_api_sequence.gz", "wb");
    int gz_write_ret = gzwrite(gz_w, out_buf, (unsigned int)(comp_size > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)comp_size));
    off64_t gz_pos = gzseek64(gz_w, 0, 1);
    int gz_eof = gzeof(gz_w);
    int gzclose_ret = gzclose(gz_w);

    // step 4: Validate and cleanup
    int def_end_ret = deflateEnd(&def_strm);
    int inf_end_ret = inflateEnd(&inf_strm);
    free(out_buf);
    (void)version;
    (void)def_init_ret;
    (void)inf_init_ret;
    (void)inf_reset_ret;
    (void)first_crc;
    (void)def_ret;
    (void)comp_size;
    (void)gz_write_ret;
    (void)gz_pos;
    (void)gz_eof;
    (void)gzclose_ret;
    (void)def_end_ret;
    (void)inf_end_ret;
    // API sequence test completed successfully
    return 66;
}