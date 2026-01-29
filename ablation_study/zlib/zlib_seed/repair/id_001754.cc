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
//<ID> 1754
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: compress -> gzwrite -> gzread -> inflateInit_ -> inflatePrime -> inflate";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *ver = zlibVersion();
    const z_crc_t *crc_tbl = get_crc_table();
    z_crc_t crc_tbl_first = crc_tbl[0];
    uLong bound = compressBound(src_len);
    uLongf comp_len = (uLongf)bound;
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    Bytef *comp_buf2 = (Bytef *)malloc((size_t)bound);
    Bytef *decomp_buf = (Bytef *)malloc((size_t)(src_len + 32));
    memset(comp_buf, 0, (int)bound);
    memset(comp_buf2, 0, (int)bound);
    memset(decomp_buf, 0, (int)(src_len + 32));
    int comp_ret = compress2(comp_buf, &comp_len, src, src_len, 6);

    // step 2: Configure & transfer via gzFile
    gzFile gzf_w = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gw_ret = gzwrite(gzf_w, comp_buf, (unsigned int)comp_len);
    int gf_ret = gzflush(gzf_w, 2);
    int gzclose_w = gzclose(gzf_w);
    gzFile gzf_r = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    off_t seek_pos = gzseek(gzf_r, 0, 0);
    int read_bytes = gzread(gzf_r, comp_buf2, (unsigned int)comp_len);

    // step 3: Core operations (inflate)
    z_stream inf;
    memset(&inf, 0, (int)sizeof(inf));
    int inf_init_ret = inflateInit_(&inf, ver, (int)sizeof(z_stream));
    inf.next_in = comp_buf2;
    inf.avail_in = (uInt)read_bytes;
    inf.next_out = decomp_buf;
    inf.avail_out = (uInt)(src_len + 32);
    int prime_ret = inflatePrime(&inf, 3, 5);
    int inf_ret = inflate(&inf, 0);
    int sync_point = inflateSyncPoint(&inf);

    // step 4: Validate & Cleanup
    uLong crc_after = crc32(0UL, decomp_buf, (uInt)src_len);
    int inf_end = inflateEnd(&inf);
    int gzclose_r = gzclose(gzf_r);
    free(comp_buf);
    free(comp_buf2);
    free(decomp_buf);
    (void)ver;
    (void)crc_tbl_first;
    (void)bound;
    (void)comp_len;
    (void)comp_ret;
    (void)gw_ret;
    (void)gf_ret;
    (void)gzclose_w;
    (void)seek_pos;
    (void)read_bytes;
    (void)inf_init_ret;
    (void)prime_ret;
    (void)inf_ret;
    (void)sync_point;
    (void)crc_after;
    (void)inf_end;
    (void)gzclose_r;
    // API sequence test completed successfully
    return 66;
}