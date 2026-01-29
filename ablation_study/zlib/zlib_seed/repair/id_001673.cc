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
//<ID> 1673
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib api sequence test: dictionary + deflate -> write gz -> inflate";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *ver = zlibVersion();
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    const Bytef preset_dict[] = "example_dictionary_for_deflate";
    int dict_set_ret = deflateSetDictionary(&def_strm, preset_dict, (uInt)(sizeof(preset_dict) - 1));
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
    Bytef dict_copy_buf[128];
    memset(dict_copy_buf, 0, (int)sizeof(dict_copy_buf));
    uInt dict_copy_len = (uInt)sizeof(dict_copy_buf);
    int dict_get_ret = deflateGetDictionary(&def_strm, dict_copy_buf, &dict_copy_len);

    gzFile gzf = gzopen("test_zlib_api_sequence.gz", "wb");
    int gw_ret = gzwrite(gzf, comp_buf, (unsigned int)comp_len);
    off64_t seek_ret = gzseek64(gzf, (off64_t)0, 0);
    int gzclose_ret_w = gzclose(gzf);

    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)comp_len;
    Bytef *out_buf = (Bytef *)malloc((size_t)src_len);
    memset(out_buf, 0, (int)src_len);
    inf_strm.next_out = out_buf;
    inf_strm.avail_out = (uInt)src_len;
    int inf_ret = inflate(&inf_strm, 0);
    unsigned long codes_used = inflateCodesUsed(&inf_strm);

    // step 4: Validate & Cleanup
    const z_crc_t *crc_tbl = get_crc_table();
    z_crc_t crc0 = crc_tbl[0];
    int def_end_ret = deflateEnd(&def_strm);
    int inf_end_ret = inflateEnd(&inf_strm);
    free(comp_buf);
    free(out_buf);
    (void)def_init_ret;
    (void)dict_set_ret;
    (void)def_ret;
    (void)comp_bound;
    (void)comp_len;
    (void)dict_get_ret;
    (void)gw_ret;
    (void)seek_ret;
    (void)gzclose_ret_w;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)codes_used;
    (void)crc0;
    (void)def_end_ret;
    (void)inf_end_ret;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}