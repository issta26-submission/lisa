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
//<ID> 1808
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: deflate -> extract dictionary -> inflateBack init/end -> write gz -> cleanup";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const Bytef preset_dict[] = "preset_dictionary_for_test";
    uInt preset_dict_len = (uInt)(sizeof(preset_dict) - 1);
    const char *ver = zlibVersion();
    z_stream def_strm;
    z_stream inf_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    int def_setdict_ret = deflateSetDictionary(&def_strm, preset_dict, preset_dict_len);

    // step 2: Configure & Compress
    uLong bound = deflateBound(&def_strm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    def_strm.next_in = (Bytef *)src;
    def_strm.avail_in = (uInt)src_len;
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)bound;
    int def_ret = deflate(&def_strm, 4);
    uLong comp_len = def_strm.total_out;

    // step 3: Extract dictionary from deflate state, init/back-end inflateBack
    uInt extract_buf_size = (uInt)256;
    Bytef *extracted_dict = (Bytef *)malloc((size_t)extract_buf_size);
    memset(extracted_dict, 0, (int)extract_buf_size);
    uInt extracted_len = extract_buf_size;
    int def_getdict_ret = deflateGetDictionary(&def_strm, extracted_dict, &extracted_len);
    int def_end_ret = deflateEnd(&def_strm);
    unsigned char *window = (unsigned char *)malloc((size_t)(1 << 15));
    memset(window, 0, (int)(1 << 15));
    int inf_back_init_ret = inflateBackInit_(&inf_strm, 15, window, ver, (int)sizeof(z_stream));
    int inf_back_end_ret = inflateBackEnd(&inf_strm);

    // step 4: Operate (write compressed data to gz) & Cleanup
    gzFile gzf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gzwrite_ret = 0;
    gzwrite_ret = gzwrite(gzf, comp_buf, (unsigned int)(comp_len > 0 ? (unsigned int)comp_len : 0U));
    int gzclose_ret = gzclose(gzf);
    uLong check_adler = adler32(0UL, src, (uInt)src_len);
    free(comp_buf);
    free(extracted_dict);
    free(window);
    (void)def_init_ret;
    (void)def_setdict_ret;
    (void)bound;
    (void)def_ret;
    (void)comp_len;
    (void)def_getdict_ret;
    (void)extracted_len;
    (void)def_end_ret;
    (void)inf_back_init_ret;
    (void)inf_back_end_ret;
    (void)gzwrite_ret;
    (void)gzclose_ret;
    (void)check_adler;
    // API sequence test completed successfully
    return 66;
}