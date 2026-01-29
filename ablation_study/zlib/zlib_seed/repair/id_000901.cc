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
//<ID> 901
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef source[] = "Hello zlib API sequence";
    uLong src_len = (uLong)(sizeof(source) - 1);
    const char *version = zlibVersion();
    uLong comp_bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_bound);
    uLong comp_len = comp_bound;
    int comp_ret = compress(comp_buf, &comp_len, source, src_len);
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));

    // step 2: Configure
    z_stream inf_strm;
    memset(&inf_strm, 0, sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)comp_len;
    Bytef *out_buf = (Bytef *)malloc((size_t)src_len);
    memset(out_buf, 0, (size_t)src_len);
    inf_strm.next_out = out_buf;
    inf_strm.avail_out = (uInt)src_len;

    // step 3: Operate
    int inf_ret = inflate(&inf_strm, 0);
    gzFile gw = gzopen("zlib_api_test_tmp.gz", "wb");
    int gw_ret = gzwrite(gw, source, (unsigned int)src_len);
    int gw_cl = gzclose(gw);
    gzFile gr = gzopen("zlib_api_test_tmp.gz", "rb");
    int first_char = gzgetc_(gr);
    int gr_cl = gzclose(gr);

    // step 4: Validate / Cleanup
    int inf_end = inflateEnd(&inf_strm);
    int def_end = deflateEnd(&def_strm);
    free(comp_buf);
    free(out_buf);
    (void)version;
    (void)comp_ret;
    (void)def_init_ret;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)gw_ret;
    (void)gw_cl;
    (void)first_char;
    (void)gr_cl;
    (void)inf_end;
    (void)def_end;
    // API sequence test completed successfully
    return 66;
}