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
//<ID> 1723
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: compress2 -> inflate -> gzwrite -> gzseek64/gzoffset -> deflateEnd";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *ver = zlibVersion();
    uLong crc = crc32(0UL, src, (uInt)src_len);
    uLong bound = compressBound(src_len);
    uLongf comp_len = (uLongf)bound;
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    int comp_ret = compress2(comp_buf, &comp_len, src, src_len, 6);
    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));

    // step 2: Configure
    Bytef *decomp_buf = (Bytef *)malloc((size_t)(src_len + 32));
    memset(decomp_buf, 0, (int)(src_len + 32));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)comp_len;
    inf_strm.next_out = decomp_buf;
    inf_strm.avail_out = (uInt)(src_len + 32);

    // step 3: Operate
    int inf_ret = inflate(&inf_strm, 0);
    int def_end_ret = deflateEnd(&def_strm);
    gzFile gzf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gw_ret = gzwrite(gzf, decomp_buf, (unsigned int)inf_strm.total_out);
    off64_t seek_ret = gzseek64(gzf, 0, 0);
    off_t offset_after_seek = gzoffset(gzf);
    int gzclose_ret = gzclose(gzf);

    // step 4: Validate & Cleanup
    int inf_end_ret = inflateEnd(&inf_strm);
    free(comp_buf);
    free(decomp_buf);
    (void)ver;
    (void)crc;
    (void)bound;
    (void)comp_len;
    (void)comp_ret;
    (void)inf_init_ret;
    (void)def_init_ret;
    (void)inf_ret;
    (void)def_end_ret;
    (void)gw_ret;
    (void)seek_ret;
    (void)offset_after_seek;
    (void)gzclose_ret;
    (void)inf_end_ret;
    // API sequence test completed successfully
    return 66;
}