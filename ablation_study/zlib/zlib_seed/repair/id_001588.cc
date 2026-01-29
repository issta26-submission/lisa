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
//<ID> 1588
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: deflate with preset dictionary -> write gz -> inflate and retrieve dictionaries";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const Bytef dict[] = "preset_dictionary";
    uInt dict_len = (uInt)(sizeof(dict) - 1);
    const char *ver = zlibVersion();
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    def_strm.next_in = (Bytef *)src;
    def_strm.avail_in = (uInt)src_len;
    int set_dict_ret = deflateSetDictionary(&def_strm, dict, dict_len);

    // step 2: Configure
    uLong def_bound = deflateBound(&def_strm, src_len);
    Bytef *def_out = (Bytef *)malloc((size_t)def_bound);
    memset(def_out, 0, (int)def_bound);
    def_strm.next_out = def_out;
    def_strm.avail_out = (uInt)def_bound;
    Bytef *retrieved_def_dict = (Bytef *)malloc((size_t)dict_len);
    memset(retrieved_def_dict, 0, (int)dict_len);
    uInt retrieved_def_len = dict_len;
    int get_def_dict_ret = deflateGetDictionary(&def_strm, retrieved_def_dict, &retrieved_def_len);
    uLong src_adler = adler32(0L, src, (uInt)src_len);

    // step 3: Operate
    int def_ret = deflate(&def_strm, 4);
    uLong comp_len = def_strm.total_out;
    gzFile gzw = gzopen("test_zlib_api_sequence.gz", "wb");
    int gw_ret = gzwrite(gzw, def_out, (unsigned int)comp_len);
    int gzclose_w_ret = gzclose_w(gzw);

    // step 4: Validate & Cleanup
    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inf_strm.next_in = def_out;
    inf_strm.avail_in = (uInt)comp_len;
    Bytef *out_buf = (Bytef *)malloc((size_t)src_len);
    memset(out_buf, 0, (int)src_len);
    inf_strm.next_out = out_buf;
    inf_strm.avail_out = (uInt)src_len;
    int set_inf_dict_ret = inflateSetDictionary(&inf_strm, dict, dict_len);
    int inf_ret = inflate(&inf_strm, 0);
    Bytef *retrieved_inf_dict = (Bytef *)malloc((size_t)dict_len);
    memset(retrieved_inf_dict, 0, (int)dict_len);
    uInt retrieved_inf_len = dict_len;
    int get_inf_dict_ret = inflateGetDictionary(&inf_strm, retrieved_inf_dict, &retrieved_inf_len);

    int def_end_ret = deflateEnd(&def_strm);
    int inf_end_ret = inflateEnd(&inf_strm);
    free(def_out);
    free(retrieved_def_dict);
    free(out_buf);
    free(retrieved_inf_dict);

    (void)ver;
    (void)def_init_ret;
    (void)set_dict_ret;
    (void)def_bound;
    (void)get_def_dict_ret;
    (void)src_adler;
    (void)def_ret;
    (void)comp_len;
    (void)gzw;
    (void)gw_ret;
    (void)gzclose_w_ret;
    (void)inf_init_ret;
    (void)set_inf_dict_ret;
    (void)inf_ret;
    (void)get_inf_dict_ret;
    (void)def_end_ret;
    (void)inf_end_ret;
    // API sequence test completed successfully
    return 66;
}