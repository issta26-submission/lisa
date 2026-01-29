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
//<ID> 1283
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const Bytef src[] = "zlib API sequence payload: compress, write via gz, read back and inflate, then query inflate codes used";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    z_stream def_strm;
    z_stream inf_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));

    // step 2: Configure compression buffers
    uLong comp_bound = deflateBound(&def_strm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_bound);
    memset(comp_buf, 0, (int)comp_bound);
    def_strm.next_in = (Bytef *)src;
    def_strm.avail_in = (uInt)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)src_len);
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)(comp_bound > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)comp_bound);

    // step 3: Operate - compress, write, seek, read, inflate and query codes used
    int def_ret = deflate(&def_strm, 4);
    uLong comp_size = def_strm.total_out;
    gzFile gzf_w = gzopen("test_zlib_api_sequence.gz", "wb");
    int gz_write_ret = gzwrite(gzf_w, comp_buf, (unsigned int)(comp_size > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)comp_size));
    off_t write_pos = gzseek(gzf_w, 0, 1);
    int gzclosew_ret = gzclose_w(gzf_w);
    gzFile gzf_r = gzopen("test_zlib_api_sequence.gz", "rb");
    off_t seek_start = gzseek(gzf_r, 0, 0);
    Bytef *read_buf = (Bytef *)malloc((size_t)comp_size);
    memset(read_buf, 0, (int)comp_size);
    int gz_read_ret = gzread(gzf_r, read_buf, (unsigned int)(comp_size > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)comp_size));
    inf_strm.next_in = read_buf;
    inf_strm.avail_in = (uInt)(gz_read_ret > 0 ? (uInt)gz_read_ret : 0U);
    uLong out_bound = src_len;
    Bytef *out_buf = (Bytef *)malloc((size_t)out_bound);
    memset(out_buf, 0, (int)out_bound);
    inf_strm.next_out = out_buf;
    inf_strm.avail_out = (uInt)(out_bound > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)out_bound);
    int inf_ret = inflate(&inf_strm, 4);
    unsigned long codes_used = inflateCodesUsed(&inf_strm);

    // step 4: Validate and cleanup
    int def_end_ret = deflateEnd(&def_strm);
    int inf_end_ret = inflateEnd(&inf_strm);
    int gzcloser_ret = gzclose_r(gzf_r);
    free(comp_buf);
    free(read_buf);
    free(out_buf);
    (void)version;
    (void)def_init_ret;
    (void)inf_init_ret;
    (void)comp_bound;
    (void)def_ret;
    (void)comp_size;
    (void)gz_write_ret;
    (void)write_pos;
    (void)gzclosew_ret;
    (void)seek_start;
    (void)gz_read_ret;
    (void)inf_ret;
    (void)codes_used;
    (void)def_end_ret;
    (void)inf_end_ret;
    (void)gzcloser_ret;
    // API sequence test completed successfully
    return 66;
}