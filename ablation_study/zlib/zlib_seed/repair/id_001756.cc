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
//<ID> 1756
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize / declarations
    const Bytef src[] = "zlib API sequence test: compress -> gzwrite -> gzseek -> gzread -> inflate with inflatePrime -> inflateSyncPoint -> get_crc_table";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *ver = zlibVersion();
    uLong bound = compressBound(src_len);
    uLongf comp_len = (uLongf)bound;
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    int comp_ret = compress2(comp_buf, &comp_len, src, src_len, 6);

    // step 2: Write compressed data to a gzip file
    gzFile gzf_w = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gw_ret = gzwrite(gzf_w, comp_buf, (unsigned int)comp_len);
    int gf_ret = gzflush(gzf_w, 2);
    int gzclose_w = gzclose(gzf_w);

    // step 3: Read compressed data back and set up inflate stream
    gzFile gzf_r = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    off_t seek_ret = gzseek(gzf_r, 0, 0);
    Bytef *comp_buf2 = (Bytef *)malloc((size_t)comp_len);
    memset(comp_buf2, 0, (int)comp_len);
    int read_bytes = gzread(gzf_r, comp_buf2, (unsigned int)comp_len);
    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inf_strm.next_in = comp_buf2;
    inf_strm.avail_in = (uInt)read_bytes;
    Bytef *decomp_buf = (Bytef *)malloc((size_t)(src_len + 16));
    memset(decomp_buf, 0, (int)(src_len + 16));
    inf_strm.next_out = decomp_buf;
    inf_strm.avail_out = (uInt)(src_len + 16);
    int prime_ret = inflatePrime(&inf_strm, 1, 1);
    int inf_ret = inflate(&inf_strm, 0);
    int sync_ret = inflateSyncPoint(&inf_strm);

    // step 4: Validate & Cleanup
    const z_crc_t *crc_tbl = get_crc_table();
    int inf_end = inflateEnd(&inf_strm);
    int gzclose_r = gzclose(gzf_r);
    free(comp_buf);
    free(comp_buf2);
    free(decomp_buf);
    (void)ver;
    (void)bound;
    (void)comp_len;
    (void)comp_ret;
    (void)gw_ret;
    (void)gf_ret;
    (void)gzclose_w;
    (void)seek_ret;
    (void)read_bytes;
    (void)inf_init_ret;
    (void)prime_ret;
    (void)inf_ret;
    (void)sync_ret;
    (void)crc_tbl;
    (void)inf_end;
    (void)gzclose_r;
    // API sequence test completed successfully
    return 66;
}