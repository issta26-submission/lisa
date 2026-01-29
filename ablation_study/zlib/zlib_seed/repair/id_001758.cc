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
//<ID> 1758
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: compress -> write gz -> seek/read -> inflatePrime -> inflate -> inflateSyncPoint";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *ver = zlibVersion();
    const z_crc_t *crc_tbl = get_crc_table();
    z_crc_t sample_crc_entry = crc_tbl[0];
    uLong bound = compressBound(src_len);
    uLongf comp_len = (uLongf)bound;
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);

    // step 2: Setup (compress and write to gz file)
    int comp_ret = compress2(comp_buf, &comp_len, src, src_len, 6);
    gzFile gzf_w = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gw_ret = gzwrite(gzf_w, comp_buf, (unsigned int)comp_len);
    int gf_ret = gzflush(gzf_w, 2);
    int gzclose_w_ret = gzclose(gzf_w);

    // step 3: Operate (read gz, seek, inflate with prime and check sync point)
    gzFile gzf_r = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    off_t seek_pos = gzseek(gzf_r, (off_t)0, 0);
    Bytef *comp_buf2 = (Bytef *)malloc((size_t)comp_len);
    memset(comp_buf2, 0, (int)comp_len);
    int read_bytes = gzread(gzf_r, comp_buf2, (unsigned int)comp_len);
    Bytef *decomp_buf = (Bytef *)malloc((size_t)(src_len + 64));
    memset(decomp_buf, 0, (int)(src_len + 64));
    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inf_strm.next_in = comp_buf2;
    inf_strm.avail_in = (uInt)read_bytes;
    inf_strm.next_out = decomp_buf;
    inf_strm.avail_out = (uInt)(src_len + 64);
    int prime_ret = inflatePrime(&inf_strm, 1, 1);
    int inf_ret = inflate(&inf_strm, 0);
    int sync_point = inflateSyncPoint(&inf_strm);
    int inf_end_ret = inflateEnd(&inf_strm);

    // step 4: Cleanup & Validate
    int gzclose_r_ret = gzclose(gzf_r);
    free(comp_buf);
    free(comp_buf2);
    free(decomp_buf);
    (void)ver;
    (void)sample_crc_entry;
    (void)bound;
    (void)comp_ret;
    (void)gw_ret;
    (void)gf_ret;
    (void)gzclose_w_ret;
    (void)seek_pos;
    (void)read_bytes;
    (void)inf_init_ret;
    (void)prime_ret;
    (void)inf_ret;
    (void)sync_point;
    (void)inf_end_ret;
    (void)gzclose_r_ret;
    // API sequence test completed successfully
    return 66;
}