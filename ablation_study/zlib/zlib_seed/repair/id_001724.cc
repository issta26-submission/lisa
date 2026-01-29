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
//<ID> 1724
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence: deflate -> write gz -> seek64 -> inflate -> crc32";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *ver = zlibVersion();
    uLong crc_before = crc32(0UL, src, (uInt)src_len);
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));

    // step 2: Configure compression buffers
    uLong comp_bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_bound);
    memset(comp_buf, 0, (int)comp_bound);
    def_strm.next_in = (Bytef *)src;
    def_strm.avail_in = (uInt)src_len;
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)comp_bound;

    // step 3: Operate - deflate and write to gz, then use gzseek64 and gzoffset
    int def_ret = deflate(&def_strm, 4);
    uLong comp_len = def_strm.total_out;
    int def_end_ret = deflateEnd(&def_strm);
    gzFile gzf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gw_ret = gzwrite(gzf, comp_buf, (unsigned int)comp_len);
    off64_t seek_ret = gzseek64(gzf, 0LL, 0);
    off_t offset = gzoffset(gzf);
    int gzclose_ret = gzclose(gzf);

    // step 4: Validate & Cleanup - inflate compressed buffer and compute crc32, then cleanup
    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    uLong decomp_bound = src_len + 16;
    Bytef *decomp_buf = (Bytef *)malloc((size_t)decomp_bound);
    memset(decomp_buf, 0, (int)decomp_bound);
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)comp_len;
    inf_strm.next_out = decomp_buf;
    inf_strm.avail_out = (uInt)decomp_bound;
    int inf_ret = inflate(&inf_strm, 0);
    uLong decomp_len = inf_strm.total_out;
    uLong crc_after = crc32(0UL, decomp_buf, (uInt)decomp_len);
    int inf_end_ret = inflateEnd(&inf_strm);
    free(comp_buf);
    free(decomp_buf);
    (void)ver;
    (void)crc_before;
    (void)def_init_ret;
    (void)comp_bound;
    (void)def_ret;
    (void)comp_len;
    (void)def_end_ret;
    (void)gw_ret;
    (void)seek_ret;
    (void)offset;
    (void)gzclose_ret;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)decomp_len;
    (void)crc_after;
    (void)inf_end_ret;
    // API sequence test completed successfully
    return 66;
}