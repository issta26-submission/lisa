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
//<ID> 1685
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence: deflateBound -> deflate -> gzwrite -> gzungetc -> adler32_combine";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *ver = zlibVersion();
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    def_strm.next_in = (Bytef *)src;
    def_strm.avail_in = (uInt)src_len;

    // step 2: Configure
    uLong comp_bound = deflateBound(&def_strm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_bound);
    memset(comp_buf, 0, (int)comp_bound);
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)comp_bound;

    // step 3: Operate
    int def_ret = deflate(&def_strm, 4);
    uLong comp_len = def_strm.total_out;
    gzFile gzf_w = gzopen("test_zlib_api_sequence.gz", "wb");
    int gw_ret = gzwrite(gzf_w, comp_buf, (unsigned int)comp_len);
    int gf_ret = gzflush(gzf_w, 2);
    int gzclose_ret_w = gzclose(gzf_w);

    // step 4: Validate & Cleanup
    gzFile gzf_r = gzopen("test_zlib_api_sequence.gz", "rb");
    int ungetc_ret = gzungetc('A', gzf_r);
    int read_char = gzgetc(gzf_r);
    uLong a1 = adler32(0UL, src, (uInt)src_len);
    uLong a2 = adler32(0UL, comp_buf, (uInt)comp_len);
    off_t combined_len = (off_t)comp_len;
    uLong combined_adler = adler32_combine(a1, a2, combined_len);
    int gzclose_ret_r = gzclose(gzf_r);
    int def_end_ret = deflateEnd(&def_strm);
    free(comp_buf);
    (void)def_init_ret;
    (void)def_ret;
    (void)comp_bound;
    (void)comp_len;
    (void)gw_ret;
    (void)gf_ret;
    (void)gzclose_ret_w;
    (void)ungetc_ret;
    (void)read_char;
    (void)a1;
    (void)a2;
    (void)combined_len;
    (void)combined_adler;
    (void)gzclose_ret_r;
    (void)def_end_ret;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}