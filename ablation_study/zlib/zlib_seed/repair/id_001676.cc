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
//<ID> 1676
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib sequence: set dictionary, deflate, get dictionary, inflate, inspect codes, write gz";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *ver = zlibVersion();
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    const Bytef dict[] = "example_dictionary_data";
    uInt dict_len = (uInt)(sizeof(dict) - 1);
    int dict_set_ret = deflateSetDictionary(&def_strm, dict, dict_len);
    def_strm.next_in = (Bytef *)src;
    def_strm.avail_in = (uInt)src_len;

    // step 2: Configure
    uLong comp_bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_bound);
    memset(comp_buf, 0, (int)comp_bound);
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)comp_bound;

    // step 3: Operate
    int def_ret = deflate(&def_strm, 4);
    uLong comp_len = def_strm.total_out;
    Bytef dict_out[128];
    memset(dict_out, 0, (int)sizeof(dict_out));
    uInt dict_out_len = (uInt)sizeof(dict_out);
    int getdict_ret = deflateGetDictionary(&def_strm, dict_out, &dict_out_len);
    int def_end_ret = deflateEnd(&def_strm);
    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)comp_len;
    Bytef *decomp_buf = (Bytef *)malloc((size_t)src_len);
    memset(decomp_buf, 0, (int)src_len);
    inf_strm.next_out = decomp_buf;
    inf_strm.avail_out = (uInt)src_len;
    int inf_ret = inflate(&inf_strm, 0);
    unsigned long codes_used = inflateCodesUsed(&inf_strm);

    // step 4: Validate & Cleanup
    const z_crc_t *crc_tbl = get_crc_table();
    gzFile gzf = gzopen("test_zlib_api_sequence.gz", "wb");
    int gw_ret = gzwrite(gzf, decomp_buf, (unsigned int)(src_len - (uLong)inf_strm.avail_out));
    off64_t seek_ret = gzseek64(gzf, (off64_t)0, 0);
    int gzflush_ret = gzflush(gzf, 2);
    int gzclose_ret = gzclose(gzf);
    int inf_end_ret = inflateEnd(&inf_strm);
    free(comp_buf);
    free(decomp_buf);
    (void)def_init_ret;
    (void)dict_set_ret;
    (void)def_ret;
    (void)comp_bound;
    (void)comp_len;
    (void)getdict_ret;
    (void)def_end_ret;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)codes_used;
    (void)crc_tbl;
    (void)gw_ret;
    (void)seek_ret;
    (void)gzflush_ret;
    (void)gzclose_ret;
    (void)ver;
    (void)dict_out_len;
    (void)dict_len;
    // API sequence test completed successfully
    return 66;
}