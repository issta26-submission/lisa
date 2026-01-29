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
//<ID> 1870
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
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    const char *ver = zlibVersion();
    deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    def_strm.next_in = (Bytef *)src;
    def_strm.avail_in = (uInt)src_len;
    uLong bound = deflateBound(&def_strm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)bound;

    // step 2: Compress
    int def_ret = deflate(&def_strm, 4);
    uLong comp_len = def_strm.total_out;

    // step 3: Write compressed data via gzopen/gzwrite/gzungetc
    gzFile gzf = gzopen("test_zlib_api.gz", "wb");
    int gz_wr = gzwrite(gzf, comp_buf, (unsigned int)comp_len);
    int gz_un = gzungetc('X', gzf);
    int gz_close_ret = gzclose(gzf);

    // step 4: Inflate to validate and Cleanup
    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)comp_len;
    Bytef *out_buf = (Bytef *)malloc((size_t)src_len);
    memset(out_buf, 0, (int)src_len);
    inf_strm.next_out = out_buf;
    inf_strm.avail_out = (uInt)src_len;
    int inf_ret = inflate(&inf_strm, 4);
    long mark = inflateMark(&inf_strm);
    int inf_end = inflateEnd(&inf_strm);
    int def_end = deflateEnd(&def_strm);
    free(comp_buf);
    free(out_buf);
    (void)ver;
    (void)def_ret;
    (void)comp_len;
    (void)gz_wr;
    (void)gz_un;
    (void)gz_close_ret;
    (void)inf_ret;
    (void)mark;
    (void)inf_end;
    (void)def_end;
    // API sequence test completed successfully
    return 66;
}