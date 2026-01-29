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
//<ID> 1807
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: deflate -> gz write -> retrieve dictionary -> inflateBack init/end -> cleanup";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const Bytef preset_dict[] = "example_preset_dictionary_for_zlib";
    uInt preset_dict_len = (uInt)(sizeof(preset_dict) - 1);
    const char *ver = zlibVersion();
    z_stream def_strm;
    z_stream inf_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    memset(&inf_strm, 0, (int)sizeof(inf_strm));

    // step 2: Setup deflate, set dictionary, and compress
    int def_init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    int def_setdict_ret = deflateSetDictionary(&def_strm, preset_dict, preset_dict_len);
    uLong bound = deflateBound(&def_strm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    def_strm.next_in = (Bytef *)src;
    def_strm.avail_in = (uInt)src_len;
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)bound;
    int def_ret = deflate(&def_strm, 4);
    uLong comp_len = def_strm.total_out;

    // step 3: Operate - retrieve dictionary from deflate stream, write compressed data to gz, init and end inflateBack
    uInt dict_out_buf_size = 32768U;
    Bytef *dict_out = (Bytef *)malloc((size_t)dict_out_buf_size);
    memset(dict_out, 0, (int)dict_out_buf_size);
    uInt dict_out_len = dict_out_buf_size;
    int def_getdict_ret = deflateGetDictionary(&def_strm, dict_out, &dict_out_len);
    int def_end_ret = deflateEnd(&def_strm);
    gzFile gzf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    z_size_t wrote = gzfwrite(comp_buf, 1, (z_size_t)comp_len, gzf);
    int gzclose_ret = gzclose(gzf);
    unsigned char *window = (unsigned char *)malloc((size_t)32768);
    memset(window, 0, 32768);
    int inf_back_init_ret = inflateBackInit_(&inf_strm, 15, window, ver, (int)sizeof(z_stream));
    int inf_back_end_ret = inflateBackEnd(&inf_strm);

    // step 4: Validate & Cleanup
    uLong checksum = adler32(0UL, src, (uInt)src_len);
    free(comp_buf);
    free(dict_out);
    free(window);
    (void)def_init_ret;
    (void)def_setdict_ret;
    (void)bound;
    (void)def_ret;
    (void)comp_len;
    (void)def_getdict_ret;
    (void)def_end_ret;
    (void)wrote;
    (void)gzclose_ret;
    (void)inf_back_init_ret;
    (void)inf_back_end_ret;
    (void)checksum;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}