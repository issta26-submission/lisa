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
//<ID> 1581
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: dictionary, deflate -> inflate, gzwrite, adler32";
    uLong src_len = (uLong)(sizeof(src) - 1);
    uLong adler = adler32(0UL, src, (uInt)src_len);
    const Bytef preset_dict[] = "preset_dictionary_data";
    uInt preset_dict_len = (uInt)(sizeof(preset_dict) - 1);

    // prepare output buffers
    uLong comp_bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_bound);
    memset(comp_buf, 0, (int)comp_bound);
    Bytef *out_buf = (Bytef *)malloc((size_t)src_len);
    memset(out_buf, 0, (int)src_len);

    // step 2: Configure
    const char *ver = zlibVersion();
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    def_strm.next_in = (Bytef *)src;
    def_strm.avail_in = (uInt)src_len;
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)comp_bound;
    int set_dict_def_ret = deflateSetDictionary(&def_strm, preset_dict, preset_dict_len);
    Bytef *retrieved_def_dict = (Bytef *)malloc((size_t)preset_dict_len);
    memset(retrieved_def_dict, 0, (int)preset_dict_len);
    uInt retrieved_def_dict_len = preset_dict_len;
    int def_get_dict_ret = deflateGetDictionary(&def_strm, retrieved_def_dict, &retrieved_def_dict_len);

    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    int set_dict_inf_ret = inflateSetDictionary(&inf_strm, preset_dict, preset_dict_len);
    Bytef *retrieved_inf_dict = (Bytef *)malloc((size_t)preset_dict_len);
    memset(retrieved_inf_dict, 0, (int)preset_dict_len);
    uInt retrieved_inf_dict_len = preset_dict_len;
    int inf_get_dict_ret = inflateGetDictionary(&inf_strm, retrieved_inf_dict, &retrieved_inf_dict_len);

    // also exercise gzwrite by writing the original data to a gzip file
    gzFile gzf = gzopen("test_zlib_api_sequence.gz", "wb");
    int gzwrite_ret = gzwrite(gzf, src, (unsigned int)src_len);
    int gzclose_ret = gzclose(gzf);

    // step 3: Operate
    int def_ret = deflate(&def_strm, 4); // 4 ~ Z_FINISH
    uLong comp_len = def_strm.total_out;
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)comp_len;
    inf_strm.next_out = out_buf;
    inf_strm.avail_out = (uInt)src_len;
    int inf_ret = inflate(&inf_strm, 0);

    // step 4: Validate & Cleanup
    int inf_end_ret = inflateEnd(&inf_strm);
    int def_end_ret = deflateEnd(&def_strm);
    free(comp_buf);
    free(out_buf);
    free(retrieved_def_dict);
    free(retrieved_inf_dict);

    (void)adler;
    (void)def_init_ret;
    (void)set_dict_def_ret;
    (void)def_get_dict_ret;
    (void)inf_init_ret;
    (void)set_dict_inf_ret;
    (void)inf_get_dict_ret;
    (void)gzwrite_ret;
    (void)gzclose_ret;
    (void)def_ret;
    (void)comp_len;
    (void)inf_ret;
    (void)inf_end_ret;
    (void)def_end_ret;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}