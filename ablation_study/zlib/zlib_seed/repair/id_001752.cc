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
//<ID> 1752
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize & allocate
    const Bytef src[] = "zlib API sequence test: deflate -> write gz -> gzseek -> read -> inflatePrime -> inflate -> inflateSyncPoint";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *ver = zlibVersion();
    uLong bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    Bytef *comp_buf2 = (Bytef *)malloc((size_t)bound);
    Bytef *decomp_buf = (Bytef *)malloc((size_t)(src_len + 64));
    z_stream def_strm;
    z_stream inf_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    memset(&inf_strm, 0, (int)sizeof(inf_strm));

    // step 2: Configure & deflate source into comp_buf and write to gz
    def_strm.next_in = (Bytef *)src;
    def_strm.avail_in = (uInt)src_len;
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)bound;
    int def_init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    int def_ret = deflate(&def_strm, 4);
    uLong comp_len = def_strm.total_out;
    int def_end_ret = deflateEnd(&def_strm);
    gzFile gzf_w = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gw_ret = gzwrite(gzf_w, comp_buf, (unsigned int)comp_len);
    int gzfclose_w = gzclose(gzf_w);

    // step 3: Read compressed bytes back, position with gzseek, then inflate using inflatePrime and check sync point
    gzFile gzf_r = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    off_t seek_ret = gzseek(gzf_r, (off_t)0, 0);
    int read_bytes = gzread(gzf_r, comp_buf2, (unsigned int)comp_len);
    int inf_init_ret = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inf_strm.next_in = comp_buf2;
    inf_strm.avail_in = (uInt)read_bytes;
    inf_strm.next_out = decomp_buf;
    inf_strm.avail_out = (uInt)(src_len + 64);
    int prime_ret = inflatePrime(&inf_strm, 1, 0);
    int inf_ret = inflate(&inf_strm, 0);
    int syncp = inflateSyncPoint(&inf_strm);

    // step 4: Validate & Cleanup
    const z_crc_t *crc_tbl = get_crc_table();
    z_crc_t first_crc_val = crc_tbl ? crc_tbl[0] : (z_crc_t)0;
    int inf_end_ret = inflateEnd(&inf_strm);
    int gzfclose_r = gzclose(gzf_r);
    free(comp_buf);
    free(comp_buf2);
    free(decomp_buf);
    (void)ver;
    (void)bound;
    (void)def_init_ret;
    (void)def_ret;
    (void)comp_len;
    (void)def_end_ret;
    (void)gw_ret;
    (void)gzfclose_w;
    (void)seek_ret;
    (void)read_bytes;
    (void)inf_init_ret;
    (void)prime_ret;
    (void)inf_ret;
    (void)syncp;
    (void)first_crc_val;
    (void)inf_end_ret;
    (void)gzfclose_r;
    // API sequence test completed successfully
    return 66;
}