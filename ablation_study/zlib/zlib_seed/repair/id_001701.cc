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
//<ID> 1701
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: zlibVersion -> compress2 -> uncompress -> inflateInit_ -> gzputs/gzgetc";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *ver = zlibVersion();
    uLong bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    uLongf comp_len = (uLongf)bound;
    int comp_ret = compress2(comp_buf, &comp_len, src, src_len, 6);

    // step 2: Configure
    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    Bytef *decomp_via_inflate = (Bytef *)malloc((size_t)src_len);
    memset(decomp_via_inflate, 0, (int)src_len);
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)comp_len;
    inf_strm.next_out = decomp_via_inflate;
    inf_strm.avail_out = (uInt)src_len;

    // step 3: Operate
    Bytef *decomp_via_uncompress = (Bytef *)malloc((size_t)src_len);
    memset(decomp_via_uncompress, 0, (int)src_len);
    uLongf out_len = (uLongf)src_len;
    int un_ret = uncompress(decomp_via_uncompress, &out_len, comp_buf, comp_len);
    int inf_ret = inflate(&inf_strm, 0);
    int inf_end_ret = inflateEnd(&inf_strm);

    // step 4: Validate & Cleanup
    gzFile gw = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gp_ret = gzputs(gw, (const char *)decomp_via_uncompress);
    int gzclose_w = gzclose(gw);
    gzFile gr = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    int first_char = gzgetc(gr);
    int second_char = gzgetc(gr);
    int gzclose_r = gzclose(gr);
    free(comp_buf);
    free(decomp_via_uncompress);
    free(decomp_via_inflate);
    (void)comp_ret;
    (void)inf_init_ret;
    (void)un_ret;
    (void)inf_ret;
    (void)inf_end_ret;
    (void)gp_ret;
    (void)gzclose_w;
    (void)first_char;
    (void)second_char;
    (void)gzclose_r;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}