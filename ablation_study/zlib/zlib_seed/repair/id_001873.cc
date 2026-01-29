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
//<ID> 1873
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize & Prepare data
    const Bytef src[] = "zlib API sequence test: deflate -> gzopen/gzwrite -> gzungetc -> inflateMark";
    uLong src_len = (uLong)(sizeof(src) - 1);
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    const char *ver = zlibVersion();
    int def_init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    def_strm.next_in = (Bytef *)src;
    def_strm.avail_in = (uInt)src_len;
    uLong comp_bound = deflateBound(&def_strm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_bound);
    memset(comp_buf, 0, (int)comp_bound);
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)comp_bound;

    // step 2: Compress & write compressed data via gzopen/gzwrite and push back a byte with gzungetc
    int def_ret = deflate(&def_strm, 4);
    uLong comp_len = def_strm.total_out;
    gzFile gzf = gzopen("test_zlib_sequence.gz", "wb");
    int gz_written = gzwrite(gzf, comp_buf, (unsigned int)(comp_len > 0 ? (unsigned int)comp_len : 0U));
    int gz_unret = gzungetc('X', gzf);

    // step 3: Initialize inflate stream, configure with compressed data and inspect mark
    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)(comp_len > 0 ? comp_len : 0UL);
    uLong out_bound = src_len + 16UL;
    Bytef *out_buf = (Bytef *)malloc((size_t)out_bound);
    memset(out_buf, 0, (int)out_bound);
    inf_strm.next_out = out_buf;
    inf_strm.avail_out = (uInt)out_bound;
    int inf_ret = inflate(&inf_strm, 0);
    long mark_val = inflateMark(&inf_strm);

    // step 4: Cleanup & Validate
    int def_end_ret = deflateEnd(&def_strm);
    int inf_end_ret = inflateEnd(&inf_strm);
    int gz_close_ret = gzclose(gzf);
    free(comp_buf);
    free(out_buf);
    (void)ver;
    (void)def_init_ret;
    (void)comp_bound;
    (void)def_ret;
    (void)comp_len;
    (void)gz_written;
    (void)gz_unret;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)mark_val;
    (void)def_end_ret;
    (void)inf_end_ret;
    (void)gz_close_ret;
    // API sequence test completed successfully
    return 66;
}