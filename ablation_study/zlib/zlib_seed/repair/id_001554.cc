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
//<ID> 1554
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: initialize deflate to get bound, compress with compress2, then inflate and uncompress for validation";
    uLong src_len = (uLong)(sizeof(src) - 1);
    z_stream dstrm;
    memset(&dstrm, 0, (int)sizeof(dstrm));
    const char *ver = zlibVersion();
    int def_init_ret = deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    uLong bound = deflateBound(&dstrm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    uLongf comp_len = (uLongf)bound;
    int comp_ret = compress(comp_buf, &comp_len, src, src_len);

    // step 2: Configure
    Bytef *out_buf = (Bytef *)malloc((size_t)src_len);
    memset(out_buf, 0, (int)src_len);
    z_stream istrm;
    memset(&istrm, 0, (int)sizeof(istrm));
    int inf_init_ret = inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)comp_len;
    istrm.next_out = out_buf;
    istrm.avail_out = (uInt)src_len;
    uLong flags = zlibCompileFlags();

    // step 3: Operate
    int inf_ret = inflate(&istrm, 0);
    unsigned long codes_used = inflateCodesUsed(&istrm);
    Bytef *out_uncomp = (Bytef *)malloc((size_t)src_len);
    memset(out_uncomp, 0, (int)src_len);
    uLongf out_uncomp_len = (uLongf)src_len;
    int uncomp_ret = uncompress(out_uncomp, &out_uncomp_len, comp_buf, (uLong)comp_len);

    // step 4: Validate & Cleanup
    int inf_end_ret = inflateEnd(&istrm);
    int def_end_ret = deflateEnd(&dstrm);
    free(comp_buf);
    free(out_buf);
    free(out_uncomp);
    (void)ver;
    (void)def_init_ret;
    (void)bound;
    (void)comp_len;
    (void)comp_ret;
    (void)inf_init_ret;
    (void)flags;
    (void)inf_ret;
    (void)codes_used;
    (void)uncomp_ret;
    (void)out_uncomp_len;
    (void)inf_end_ret;
    (void)def_end_ret;
    // API sequence test completed successfully
    return 66;
}