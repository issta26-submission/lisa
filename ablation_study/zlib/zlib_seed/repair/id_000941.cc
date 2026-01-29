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
//<ID> 941
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef source[] = "Zlib API sequence payload to compress, write to gz, read back and inflate.";
    uLong src_len = (uLong)(sizeof(source) - 1);
    z_stream def_strm;
    z_stream inf_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    memset(&inf_strm, 0, sizeof(inf_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));

    // step 2: Configure
    uLong bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    def_strm.next_in = (Bytef *)source;
    def_strm.avail_in = (uInt)src_len;
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)bound;
    int tune_ret = deflateTune(&def_strm, 32, 64, 128, 1024);

    // step 3: Operate
    int def_ret = deflate(&def_strm, 4);
    uLong comp_len = def_strm.total_out;
    int def_end_ret = deflateEnd(&def_strm);
    gzFile gzf_w = gzopen("zlib_api_sequence_tmp.gz", "wb");
    int gz_wr = gzwrite(gzf_w, comp_buf, (unsigned int)comp_len);
    int gzclose_w_ret = gzclose(gzf_w);
    gzFile gzf_r = gzopen("zlib_api_sequence_tmp.gz", "rb");
    Bytef *read_buf = (Bytef *)malloc((size_t)comp_len);
    memset(read_buf, 0, (size_t)comp_len);
    z_size_t items_read = gzfread(read_buf, (z_size_t)1, (z_size_t)comp_len, gzf_r);
    inf_strm.next_in = read_buf;
    inf_strm.avail_in = (uInt)items_read;
    Bytef *decomp_buf = (Bytef *)malloc((size_t)(src_len + 64));
    memset(decomp_buf, 0, (size_t)(src_len + 64));
    inf_strm.next_out = decomp_buf;
    inf_strm.avail_out = (uInt)(src_len + 64);
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    Bytef dict_buf[32];
    uInt dict_len = (uInt)sizeof(dict_buf);
    int inf_getdict_ret = inflateGetDictionary(&inf_strm, dict_buf, &dict_len);
    int inf_ret = inflate(&inf_strm, 4);
    int inf_end_ret = inflateEnd(&inf_strm);
    int gzclose_r_ret = gzclose(gzf_r);

    // step 4: Validate / Cleanup
    free(comp_buf);
    free(read_buf);
    free(decomp_buf);
    (void)version;
    (void)def_init_ret;
    (void)bound;
    (void)tune_ret;
    (void)def_ret;
    (void)comp_len;
    (void)def_end_ret;
    (void)gz_wr;
    (void)gzclose_w_ret;
    (void)items_read;
    (void)inf_init_ret;
    (void)inf_getdict_ret;
    (void)inf_ret;
    (void)inf_end_ret;
    (void)gzclose_r_ret;
    // API sequence test completed successfully
    return 66;
}