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
//<ID> 1872
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize & Prepare data
    const Bytef src[] = "zlib API sequence test: deflate -> gzopen/gzwrite/gzungetc -> inflateMark";
    uLong src_len = (uLong)(sizeof(src) - 1);
    z_stream defstrm;
    memset(&defstrm, 0, (int)sizeof(defstrm));
    const char *ver = zlibVersion();
    int def_init_ret = deflateInit_(&defstrm, 6, ver, (int)sizeof(z_stream));
    defstrm.next_in = (Bytef *)src;
    defstrm.avail_in = (uInt)src_len;

    // step 2: Configure compressor and buffers, perform deflate
    uLong bound = deflateBound(&defstrm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)(bound + 1));
    memset(comp_buf, 0, (int)(bound + 1));
    defstrm.next_out = comp_buf;
    defstrm.avail_out = (uInt)bound;
    int def_ret = deflate(&defstrm, 4);
    uLong comp_len = defstrm.total_out;

    // step 3: Operate - write compressed data via gzopen/gzwrite, push a byte back, then setup inflate and query mark
    gzFile gzf = gzopen("zlib_api_sequence_test.gz", "wb");
    int gz_wr = gzwrite(gzf, comp_buf, (unsigned int)(comp_len > 0 ? (unsigned int)comp_len : 0U));
    int gz_unret = gzungetc('X', gzf);
    z_stream instrm;
    memset(&instrm, 0, (int)sizeof(instrm));
    int inf_init_ret = inflateInit_(&instrm, ver, (int)sizeof(z_stream));
    instrm.next_in = comp_buf;
    instrm.avail_in = (uInt)comp_len;
    Bytef *out_buf = (Bytef *)malloc((size_t)(src_len + 16));
    memset(out_buf, 0, (int)(src_len + 16));
    instrm.next_out = out_buf;
    instrm.avail_out = (uInt)(src_len + 16);
    int inf_ret = inflate(&instrm, 4);
    long mark_val = inflateMark(&instrm);

    // step 4: Cleanup & Validate
    int def_end_ret = deflateEnd(&defstrm);
    int inf_end_ret = inflateEnd(&instrm);
    int gz_close_ret = gzclose(gzf);
    free(comp_buf);
    free(out_buf);
    (void)ver;
    (void)def_init_ret;
    (void)bound;
    (void)def_ret;
    (void)comp_len;
    (void)gz_wr;
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