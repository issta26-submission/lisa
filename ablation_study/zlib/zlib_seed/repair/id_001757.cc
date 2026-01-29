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
//<ID> 1757
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: compress -> inflateInit_ -> inflatePrime -> inflate -> gzip write/read/seek -> crc";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *ver = zlibVersion();
    const z_crc_t *crc_tbl = get_crc_table();
    uLong bound = compressBound(src_len);
    uLongf comp_len = (uLongf)bound;
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    Bytef *decomp_buf = (Bytef *)malloc((size_t)(src_len + 16));
    memset(comp_buf, 0, (int)bound);
    memset(decomp_buf, 0, (int)(src_len + 16));

    // step 2: Configure (compress and prepare inflate stream)
    int comp_ret = compress2(comp_buf, &comp_len, src, src_len, 6);
    z_stream inf;
    memset(&inf, 0, (int)sizeof(inf));
    int inf_init_ret = inflateInit_(&inf, ver, (int)sizeof(z_stream));
    inf.next_in = comp_buf;
    inf.avail_in = (uInt)comp_len;
    inf.next_out = decomp_buf;
    inf.avail_out = (uInt)(src_len + 16);

    // step 3: Operate (prime, inflate, write compressed to gz, then seek/read operations)
    int prime_ret = inflatePrime(&inf, 1, 1);
    int inf_ret = inflate(&inf, 0);
    int sync_point = inflateSyncPoint(&inf);
    gzFile gzf_w = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gw_ret = gzwrite(gzf_w, comp_buf, (unsigned int)comp_len);
    int gf_ret = gzflush(gzf_w, 2);
    int gzclose_w = gzclose(gzf_w);
    gzFile gzf_r = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    off_t seek_ret = gzseek(gzf_r, (off_t)0, 0);
    int gzclose_r = gzclose(gzf_r);

    // step 4: Validate & Cleanup
    uLong crc_calc = crc32(0UL, decomp_buf, (uInt)src_len);
    z_crc_t sample_tbl_val = crc_tbl ? crc_tbl[0] : (z_crc_t)0;
    int inf_end_ret = inflateEnd(&inf);
    free(comp_buf);
    free(decomp_buf);
    (void)ver;
    (void)comp_ret;
    (void)inf_init_ret;
    (void)prime_ret;
    (void)inf_ret;
    (void)sync_point;
    (void)gw_ret;
    (void)gf_ret;
    (void)gzclose_w;
    (void)seek_ret;
    (void)gzclose_r;
    (void)crc_calc;
    (void)sample_tbl_val;
    (void)inf_end_ret;
    // API sequence test completed successfully
    return 66;
}