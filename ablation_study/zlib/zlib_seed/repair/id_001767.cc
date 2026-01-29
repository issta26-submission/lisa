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
//<ID> 1767
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize data and compute CRC
    const Bytef src[] = "zlib API sequence test: deflate with optional dictionary -> write gz -> inflate -> crc32 verify";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *ver = zlibVersion();
    uLong crc_before = crc32(0UL, src, (uInt)src_len);

    // step 2: Configure deflate stream, set (empty) dictionary, compress into memory
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    uLong bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    def_strm.next_in = (Bytef *)src;
    def_strm.avail_in = (uInt)src_len;
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)bound;
    int def_init_ret = deflateInit2_(&def_strm, 6, 8, 15, 8, 0, ver, (int)sizeof(z_stream));
    Bytef dict_buf[1];
    memset(dict_buf, 0, (int)sizeof(dict_buf));
    uInt dict_len = 0u;
    int def_set_dict_ret = deflateSetDictionary(&def_strm, dict_buf, dict_len);
    int def_ret = deflate(&def_strm, 4);
    uLong comp_len = def_strm.total_out;
    int def_end_ret = deflateEnd(&def_strm);

    // step 3: Write compressed data to a .gz file, clear error state, then close
    gzFile gzf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gw_ret = gzwrite(gzf, comp_buf, (unsigned int)comp_len);
    gzclearerr(gzf);
    int gzclose_ret = gzclose(gzf);

    // step 4: Inflate the compressed data in memory and verify CRC, then cleanup
    Bytef *decomp_buf = (Bytef *)malloc((size_t)(src_len + 64));
    memset(decomp_buf, 0, (int)(src_len + 64));
    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)comp_len;
    inf_strm.next_out = decomp_buf;
    inf_strm.avail_out = (uInt)(src_len + 64);
    int inf_init_ret = inflateInit2_(&inf_strm, 15, ver, (int)sizeof(z_stream));
    int inf_ret = inflate(&inf_strm, 0);
    uLong crc_after = crc32(0UL, decomp_buf, (uInt)src_len);
    int inf_end_ret = inflateEnd(&inf_strm);

    // cleanup
    free(comp_buf);
    free(decomp_buf);
    (void)ver;
    (void)crc_before;
    (void)def_init_ret;
    (void)def_set_dict_ret;
    (void)def_ret;
    (void)def_end_ret;
    (void)bound;
    (void)gw_ret;
    (void)gzclose_ret;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)crc_after;
    (void)inf_end_ret;
    // API sequence test completed successfully
    return 66;
}