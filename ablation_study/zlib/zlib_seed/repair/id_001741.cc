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
//<ID> 1741
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize & deflate source data
    const Bytef src[] = "zlib API sequence test: deflate -> gzwrite -> inflate -> crc32";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *ver = zlibVersion();
    uLong crc_before = crc32(0UL, src, (uInt)src_len);
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    uLong bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    def_strm.next_in = (Bytef *)src;
    def_strm.avail_in = (uInt)src_len;
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)bound;
    int def_ret = deflate(&def_strm, 4);
    uLong comp_len = def_strm.total_out;
    int def_end_ret = deflateEnd(&def_strm);

    // step 2: Write original data to a gzFile (gzopen/gzwrite/gzflush/gzclose)
    gzFile gzf_w = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gw_ret = gzwrite(gzf_w, src, (unsigned int)src_len);
    int gf_ret = gzflush(gzf_w, 2);
    int gzclose_w = gzclose(gzf_w);

    // step 3: Inflate the deflated buffer and query dictionary
    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    Bytef small_dict[32];
    memset(small_dict, 0, (int)sizeof(small_dict));
    uInt dict_len = (uInt)sizeof(small_dict);
    int getdict_ret = inflateGetDictionary(&inf_strm, small_dict, &dict_len);
    Bytef *inf_decomp_buf = (Bytef *)malloc((size_t)(src_len + 1));
    memset(inf_decomp_buf, 0, (int)(src_len + 1));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)comp_len;
    inf_strm.next_out = inf_decomp_buf;
    inf_strm.avail_out = (uInt)(src_len + 1);
    int inf_ret = inflate(&inf_strm, 0);
    int inf_end_ret = inflateEnd(&inf_strm);

    // step 4: Read back from gzFile, compute CRC, cleanup
    gzFile gzf_r = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    Bytef *gz_decomp_buf = (Bytef *)malloc((size_t)(src_len + 1));
    memset(gz_decomp_buf, 0, (int)(src_len + 1));
    int read_bytes = gzread(gzf_r, gz_decomp_buf, (unsigned int)(src_len + 1));
    int gzclose_r = gzclose(gzf_r);
    uLong crc_after = crc32(0UL, inf_decomp_buf, (uInt)src_len);
    free(comp_buf);
    free(inf_decomp_buf);
    free(gz_decomp_buf);
    (void)ver;
    (void)crc_before;
    (void)def_init_ret;
    (void)bound;
    (void)def_ret;
    (void)comp_len;
    (void)def_end_ret;
    (void)gw_ret;
    (void)gf_ret;
    (void)gzclose_w;
    (void)inf_init_ret;
    (void)getdict_ret;
    (void)inf_ret;
    (void)inf_end_ret;
    (void)read_bytes;
    (void)gzclose_r;
    (void)crc_after;
    // API sequence test completed successfully
    return 66;
}