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
//<ID> 1753
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: compress -> inflateInit_ -> inflatePrime -> inflate -> gzwrite/gzseek";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *ver = zlibVersion();
    uLong bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    uLongf comp_len_f = (uLongf)bound;
    int comp_ret = compress2(comp_buf, &comp_len_f, src, src_len, 6);
    uLong comp_len = (uLong)comp_len_f;
    const z_crc_t *crc_tbl = get_crc_table();
    z_crc_t first_crc = crc_tbl[0];

    // step 2: Setup inflate stream
    z_stream inf;
    memset(&inf, 0, (int)sizeof(inf));
    inf.next_in = comp_buf;
    inf.avail_in = (uInt)comp_len;
    Bytef *decomp_buf = (Bytef *)malloc((size_t)(src_len + 16));
    inf.next_out = decomp_buf;
    inf.avail_out = (uInt)(src_len + 16);
    int inf_init_ret = inflateInit_(&inf, ver, (int)sizeof(z_stream));
    int prime_ret = inflatePrime(&inf, 1, 1);

    // step 3: Operate
    int inf_ret = inflate(&inf, 0);
    int sync_flag = inflateSyncPoint(&inf);
    gzFile gzf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gw_ret = gzwrite(gzf, decomp_buf, (unsigned int)src_len);
    off_t seek_pos = gzseek(gzf, (off_t)0, 0);
    int gzclose_ret = gzclose(gzf);

    // step 4: Validate & Cleanup
    int inf_end_ret = inflateEnd(&inf);
    free(comp_buf);
    free(decomp_buf);
    (void)ver;
    (void)bound;
    (void)comp_ret;
    (void)comp_len_f;
    (void)comp_len;
    (void)crc_tbl;
    (void)first_crc;
    (void)inf_init_ret;
    (void)prime_ret;
    (void)inf_ret;
    (void)sync_flag;
    (void)gw_ret;
    (void)seek_pos;
    (void)gzclose_ret;
    (void)inf_end_ret;
    // API sequence test completed successfully
    return 66;
}