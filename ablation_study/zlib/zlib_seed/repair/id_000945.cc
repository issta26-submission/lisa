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
//<ID> 945
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef source[] = "Zlib payload: compress with deflate, write to .gz, read back, and inflate.";
    uLong src_len = (uLong)(sizeof(source) - 1);
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    int tune_ret = deflateTune(&def_strm, 4, 8, 16, 32);

    // step 2: Configure (compress using deflate)
    uLong bound = deflateBound(&def_strm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    def_strm.next_in = (Bytef *)source;
    def_strm.avail_in = (uInt)src_len;
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)bound;
    int def_ret = deflate(&def_strm, 4);
    uLong comp_len = def_strm.total_out;
    int def_end_ret = deflateEnd(&def_strm);

    // step 3: Operate (write compressed to .gz, read it back)
    gzFile gzf_w = gzopen("zlib_api_sequence_tmp.gz", "wb");
    int gz_written = gzwrite(gzf_w, comp_buf, (unsigned int)comp_len);
    int gzclose_w_ret = gzclose(gzf_w);
    gzFile gzf_r = gzopen("zlib_api_sequence_tmp.gz", "rb");
    Bytef *read_buf = (Bytef *)malloc((size_t)bound);
    memset(read_buf, 0, (size_t)bound);
    z_size_t items_read = gzfread(read_buf, 1, (z_size_t)bound, gzf_r);
    int gzclose_r_ret = gzclose(gzf_r);

    // step 4: Validate / Cleanup (initialize inflate, try to get dictionary, inflate, cleanup)
    z_stream inf_strm;
    memset(&inf_strm, 0, sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    Bytef dict_buf[64];
    memset(dict_buf, 0, sizeof(dict_buf));
    uInt dict_len = (uInt)sizeof(dict_buf);
    int get_dict_ret = inflateGetDictionary(&inf_strm, dict_buf, &dict_len);
    inf_strm.next_in = read_buf;
    inf_strm.avail_in = (uInt)items_read;
    Bytef *decomp_buf = (Bytef *)malloc((size_t)(src_len + 64));
    memset(decomp_buf, 0, (size_t)(src_len + 64));
    inf_strm.next_out = decomp_buf;
    inf_strm.avail_out = (uInt)(src_len + 64);
    int inf_ret = inflate(&inf_strm, 4);
    int inf_end_ret = inflateEnd(&inf_strm);

    free(comp_buf);
    free(read_buf);
    free(decomp_buf);
    (void)version;
    (void)def_init_ret;
    (void)tune_ret;
    (void)bound;
    (void)def_ret;
    (void)comp_len;
    (void)def_end_ret;
    (void)gz_written;
    (void)gzclose_w_ret;
    (void)items_read;
    (void)gzclose_r_ret;
    (void)inf_init_ret;
    (void)get_dict_ret;
    (void)inf_ret;
    (void)inf_end_ret;
    // API sequence test completed successfully
    return 66;
}