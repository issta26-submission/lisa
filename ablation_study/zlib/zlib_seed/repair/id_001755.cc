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
//<ID> 1755
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: compress -> inflateInit_ -> inflatePrime -> inflate -> inflateSyncPoint -> gzwrite/gzseek/gzclose";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *ver = zlibVersion();
    uLong bound = compressBound(src_len);
    uLongf comp_len = (uLongf)bound;
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    Bytef *decomp_buf = (Bytef *)malloc((size_t)(src_len + 16));
    z_stream inf;
    memset(&inf, 0, (int)sizeof(inf));

    // step 2: Configure (compress source and prepare inflate stream)
    int comp_ret = compress(comp_buf, &comp_len, src, src_len);
    inf.next_in = comp_buf;
    inf.avail_in = (uInt)comp_len;
    inf.next_out = decomp_buf;
    inf.avail_out = (uInt)(src_len + 16);
    int inf_init_ret = inflateInit_(&inf, ver, (int)sizeof(z_stream));
    int prime_ret = inflatePrime(&inf, 1, 1);

    // step 3: Operate (inflate and check sync point)
    int inf_ret = inflate(&inf, 0);
    int sync_point = inflateSyncPoint(&inf);
    int inf_end_ret = inflateEnd(&inf);

    // step 4: Validate & Cleanup (use CRC table, write gz, seek, flush, close, free buffers)
    const z_crc_t *crc_tab = get_crc_table();
    z_crc_t crc_first = crc_tab[0];
    gzFile gzf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gw_ret = gzwrite(gzf, decomp_buf, (unsigned int)src_len);
    int gf_ret = gzflush(gzf, 2);
    off_t seek_ret = gzseek(gzf, (off_t)0, 0);
    int gzclose_ret = gzclose(gzf);
    free(comp_buf);
    free(decomp_buf);
    (void)ver;
    (void)bound;
    (void)comp_len;
    (void)comp_ret;
    (void)inf_init_ret;
    (void)prime_ret;
    (void)inf_ret;
    (void)sync_point;
    (void)inf_end_ret;
    (void)crc_first;
    (void)gw_ret;
    (void)gf_ret;
    (void)seek_ret;
    (void)gzclose_ret;
    // API sequence test completed successfully
    return 66;
}