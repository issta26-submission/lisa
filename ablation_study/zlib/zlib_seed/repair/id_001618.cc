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
//<ID> 1618
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
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    int init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    def_strm.next_in = (Bytef *)src;
    def_strm.avail_in = (uInt)src_len;

    // step 2: Configure
    uLong comp_bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_bound);
    memset(comp_buf, 0, (int)comp_bound);
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)comp_bound;
    const Bytef preset_dict[] = "preset_dictionary_for_test";
    uInt preset_len = (uInt)(sizeof(preset_dict) - 1);
    int set_dict_ret = deflateSetDictionary(&def_strm, preset_dict, preset_len);
    gzFile gzf = gzopen("test_zlib_api_sequence.gz", "wb");
    int gw_ret_before = gzwrite(gzf, src, (unsigned int)src_len);
    gzclearerr(gzf);

    // step 3: Operate
    int def_ret = deflate(&def_strm, 4);
    uLong comp_len = def_strm.total_out;
    int gw_ret = gzwrite(gzf, comp_buf, (unsigned int)comp_len);

    // step 4: Validate & Cleanup
    int def_end = deflateEnd(&def_strm);
    int gz_close_ret = gzclose(gzf);
    const char *errstr = zError(0);
    free(comp_buf);
    (void)ver;
    (void)init_ret;
    (void)comp_bound;
    (void)set_dict_ret;
    (void)gw_ret_before;
    (void)def_ret;
    (void)comp_len;
    (void)gw_ret;
    (void)def_end;
    (void)gz_close_ret;
    (void)errstr;
    // API sequence test completed successfully
    return 66;
}