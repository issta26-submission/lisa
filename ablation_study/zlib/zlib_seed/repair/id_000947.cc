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
//<ID> 947
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef source[] = "Example payload for zlib streaming: compress, write to .gz, read back and inflate.";
    uLong src_len = (uLong)(sizeof(source) - 1);
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    int tune_ret = deflateTune(&def_strm, 4, 8, 16, 4);
    uLong bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    def_strm.next_in = (Bytef *)source;
    def_strm.avail_in = (uInt)src_len;
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)bound;
    int def_ret = deflate(&def_strm, 4);
    uLong comp_used = def_strm.total_out;

    // step 2: Configure (write compressed data to a .gz file)
    gzFile gzf_w = gzopen("tmp_zlib_sequence.gz", "wb");
    int gz_written = gzwrite(gzf_w, comp_buf, (unsigned int)comp_used);
    int gzclose_w_ret = gzclose(gzf_w);

    // step 3: Operate (read compressed data back and inflate)
    gzFile gzf_r = gzopen("tmp_zlib_sequence.gz", "rb");
    Bytef *read_buf = (Bytef *)malloc((size_t)comp_used + 1);
    memset(read_buf, 0, (size_t)comp_used + 1);
    z_size_t items_read = gzfread(read_buf, 1, (z_size_t)comp_used, gzf_r);
    int gzclose_r_ret = gzclose(gzf_r);
    z_stream inf_strm;
    memset(&inf_strm, 0, sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    inf_strm.next_in = read_buf;
    inf_strm.avail_in = (uInt)items_read;
    Bytef *decomp_buf = (Bytef *)malloc((size_t)(src_len + 64));
    memset(decomp_buf, 0, (size_t)(src_len + 64));
    inf_strm.next_out = decomp_buf;
    inf_strm.avail_out = (uInt)(src_len + 64);
    int inf_ret = inflate(&inf_strm, 4);
    Bytef dict_buf_local[64];
    uInt dict_len = (uInt)sizeof(dict_buf_local);
    int getdict_ret = inflateGetDictionary(&inf_strm, dict_buf_local, &dict_len);

    // step 4: Validate / Cleanup
    int inf_end_ret = inflateEnd(&inf_strm);
    int def_end_ret = deflateEnd(&def_strm);
    free(comp_buf);
    free(read_buf);
    free(decomp_buf);
    (void)version;
    (void)def_init_ret;
    (void)tune_ret;
    (void)bound;
    (void)def_ret;
    (void)comp_used;
    (void)gz_written;
    (void)gzclose_w_ret;
    (void)items_read;
    (void)gzclose_r_ret;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)getdict_ret;
    (void)inf_end_ret;
    (void)def_end_ret;
    // API sequence test completed successfully
    return 66;
}