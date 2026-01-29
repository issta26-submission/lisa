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
//<ID> 1746
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize (source, dictionary, CRC, and deflate stream)
    const Bytef src[] = "zlib API sequence test: deflate -> write gz -> inflateGetDictionary -> crc32";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const Bytef dict[] = "sample_dictionary_for_zlib_usage";
    uInt dict_len = (uInt)(sizeof(dict) - 1);
    const char *ver = zlibVersion();
    uLong crc_before = crc32(0UL, src, (uInt)src_len);
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));

    // step 2: Configure & Operate (set dictionary on deflate, deflate the input)
    uLong bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    int dict_set_ret = deflateSetDictionary(&def_strm, dict, dict_len);
    def_strm.next_in = (Bytef *)src;
    def_strm.avail_in = (uInt)src_len;
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)bound;
    int def_ret = deflate(&def_strm, 4);
    uLong comp_len = def_strm.total_out;
    int def_end_ret = deflateEnd(&def_strm);

    // step 3: Write compressed data to gzFile
    gzFile gzf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gw_ret = gzwrite(gzf, comp_buf, (unsigned int)comp_len);
    int gzclose_w = gzclose(gzf);

    // step 4: Inflate-side: init, set dictionary, get dictionary, crc check, cleanup
    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    int inf_set_ret = inflateSetDictionary(&inf_strm, dict, dict_len);
    Bytef dict_out[64];
    memset(dict_out, 0, (int)sizeof(dict_out));
    uInt dict_out_len = (uInt)sizeof(dict_out);
    int inf_get_ret = inflateGetDictionary(&inf_strm, dict_out, &dict_out_len);
    uLong dict_crc = crc32(0UL, dict_out, dict_out_len);
    int inf_end_ret = inflateEnd(&inf_strm);
    free(comp_buf);
    (void)ver;
    (void)crc_before;
    (void)def_init_ret;
    (void)bound;
    (void)dict_set_ret;
    (void)def_ret;
    (void)comp_len;
    (void)def_end_ret;
    (void)gw_ret;
    (void)gzclose_w;
    (void)inf_init_ret;
    (void)inf_set_ret;
    (void)inf_get_ret;
    (void)dict_crc;
    (void)inf_end_ret;
    // API sequence test completed successfully
    return 66;
}