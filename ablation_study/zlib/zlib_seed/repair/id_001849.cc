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
//<ID> 1849
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize & Prepare data
    const Bytef src[] = "zlib API sequence test: compress -> inflate -> gzread -> crc";
    uLong src_len = (uLong)(sizeof(src) - 1);
    uLong crc_src = crc32(0UL, src, (uInt)src_len);
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    const char *ver = zlibVersion();
    int def_init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    uLong bound = deflateBound(&def_strm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    def_strm.next_in = (Bytef *)src;
    def_strm.avail_in = (uInt)src_len;
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)bound;
    int def_ret = deflate(&def_strm, 4);
    uLong comp_len = def_strm.total_out;
    int def_end_ret = deflateEnd(&def_strm);

    // step 2: Configure inflate and buffers
    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    Bytef *out_buf = (Bytef *)malloc((size_t)(src_len + 16UL));
    memset(out_buf, 0, (int)(src_len + 16UL));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)(comp_len > 0 ? comp_len : 0UL);
    inf_strm.next_out = out_buf;
    inf_strm.avail_out = (uInt)(src_len + 16UL);
    int inf_init_ret = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));

    // step 3: Operate - inflate compressed buffer, write/read gz file
    int inf_ret = inflate(&inf_strm, 4);
    uLong out_len = inf_strm.total_out;
    int inf_end_ret = inflateEnd(&inf_strm);
    gzFile gzf_write = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gz_wr_ret = gzwrite(gzf_write, src, (unsigned int)src_len);
    int gz_close_wr = gzclose(gzf_write);
    gzFile gzf_read = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    int gz_rewind_ret = gzrewind(gzf_read);
    z_size_t read_buf_size = (z_size_t)(src_len + 16UL);
    Bytef *read_buf = (Bytef *)malloc((size_t)read_buf_size);
    memset(read_buf, 0, (int)read_buf_size);
    z_size_t items_read = gzfread(read_buf, 1, read_buf_size, gzf_read);
    int gz_close_rd = gzclose(gzf_read);

    // step 4: Validate & Cleanup
    uLong crc_inflated = crc32(0UL, out_buf, (uInt)(out_len > 0 ? out_len : 0UL));
    uLong crc_read = crc32(0UL, read_buf, (uInt)(items_read > 0 ? (uInt)items_read : 0U));
    free(comp_buf);
    free(out_buf);
    free(read_buf);
    (void)ver;
    (void)def_init_ret;
    (void)def_ret;
    (void)def_end_ret;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)inf_end_ret;
    (void)gz_wr_ret;
    (void)gz_close_wr;
    (void)gz_rewind_ret;
    (void)gz_close_rd;
    (void)crc_src;
    (void)crc_inflated;
    (void)crc_read;
    // API sequence test completed successfully
    return 66;
}