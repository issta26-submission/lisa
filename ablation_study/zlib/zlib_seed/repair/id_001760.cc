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
//<ID> 1760
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: init -> set dict (deflate) -> compress2 -> write gz -> read gz -> inflateInit2_ -> inflate -> crc32";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *ver = zlibVersion();
    uLong bound = compressBound(src_len);
    uLongf comp_len = (uLongf)bound;
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    Bytef *comp_buf2 = (Bytef *)malloc((size_t)bound);
    Bytef *decomp_buf = (Bytef *)malloc((size_t)(src_len + 64));
    memset(comp_buf, 0, (int)bound);
    memset(comp_buf2, 0, (int)bound);
    memset(decomp_buf, 0, (int)(src_len + 64));
    uLong crc_before = crc32(0UL, src, (uInt)src_len);

    // step 2: Configure (create a deflate stream and set a dictionary)
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    Bytef dict_buf[32];
    memset(dict_buf, 0xA5, (int)sizeof(dict_buf));
    uInt dict_len = (uInt)sizeof(dict_buf);
    int setdict_ret = deflateSetDictionary(&def_strm, dict_buf, dict_len);
    int def_end_ret = deflateEnd(&def_strm);

    // step 3: Operate (compress with compress2, write to gz, clear error, close)
    int comp_ret = compress2(comp_buf, &comp_len, src, src_len, 6);
    gzFile gzf_w = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gw_ret = gzwrite(gzf_w, comp_buf, (unsigned int)comp_len);
    gzclearerr(gzf_w);
    int gzclose_w = gzclose(gzf_w);

    // step 4: Validate & Cleanup (read gz, inflate using inflateInit2_, compute crc, cleanup)
    gzFile gzf_r = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    int read_bytes = gzread(gzf_r, comp_buf2, (unsigned int)comp_len);
    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int inf_init_ret = inflateInit2_(&inf_strm, 15, ver, (int)sizeof(z_stream));
    inf_strm.next_in = comp_buf2;
    inf_strm.avail_in = (uInt)read_bytes;
    inf_strm.next_out = decomp_buf;
    inf_strm.avail_out = (uInt)(src_len + 64);
    int inf_ret = inflate(&inf_strm, 0);
    uLong crc_after = crc32(0UL, decomp_buf, (uInt)src_len);
    int inf_end_ret = inflateEnd(&inf_strm);
    int gzclose_r = gzclose(gzf_r);
    free(comp_buf);
    free(comp_buf2);
    free(decomp_buf);
    (void)ver;
    (void)crc_before;
    (void)bound;
    (void)comp_len;
    (void)def_init_ret;
    (void)setdict_ret;
    (void)def_end_ret;
    (void)comp_ret;
    (void)gw_ret;
    (void)gzclose_w;
    (void)read_bytes;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)crc_after;
    (void)inf_end_ret;
    (void)gzclose_r;
    // API sequence test completed successfully
    return 66;
}