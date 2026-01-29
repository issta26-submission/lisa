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
//<ID> 1616
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: compressBound + deflate + gzclearerr + zError usage";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *ver = zlibVersion();
    uLong buf_bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)buf_bound);
    memset(comp_buf, 0, (int)buf_bound);
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    def_strm.next_in = (Bytef *)src;
    def_strm.avail_in = (uInt)src_len;
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)buf_bound;

    // step 2: Configure
    const Bytef preset_dict[] = "preset_dictionary_for_test_zlib";
    uInt preset_len = (uInt)(sizeof(preset_dict) - 1);
    int set_dict_ret = deflateSetDictionary(&def_strm, preset_dict, preset_len);
    int tune_ret = deflateTune(&def_strm, 10, 5, 20, 1024);

    // step 3: Operate
    int def_ret = deflate(&def_strm, 4);
    uLong comp_len = def_strm.total_out;
    gzFile gzf = gzopen("test_zlib_api_sequence.gz", "wb");
    gzclearerr(gzf);
    int gw_ret = gzwrite(gzf, src, (unsigned int)src_len);
    int gz_close_ret = gzclose(gzf);

    // step 4: Validate & Cleanup
    const char *err_str = zError(def_ret);
    int def_end_ret = deflateEnd(&def_strm);
    free(comp_buf);
    (void)ver;
    (void)def_init_ret;
    (void)buf_bound;
    (void)set_dict_ret;
    (void)tune_ret;
    (void)comp_len;
    (void)gw_ret;
    (void)gz_close_ret;
    (void)err_str;
    (void)def_end_ret;
    // API sequence test completed successfully
    return 66;
}