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
//<ID> 1557
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    Bytef src[] = "zlib API sequence test: use compress2, uncompress, inflate, and proper init/end calls";
    uLong src_len = (uLong)(sizeof(src) - 1);
    uLong bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    uLongf comp_len = (uLongf)bound;
    int comp_ret = compress2(comp_buf, &comp_len, src, src_len, 6);

    // step 2: Configure
    uLong compile_flags = zlibCompileFlags();
    z_stream istrm;
    memset(&istrm, 0, (int)sizeof(istrm));
    const char *ver = zlibVersion();
    int inf_init_ret = inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)comp_len;
    Bytef *out_buf = (Bytef *)malloc((size_t)src_len);
    memset(out_buf, 0, (int)src_len);
    istrm.next_out = out_buf;
    istrm.avail_out = (uInt)src_len;
    z_stream dstrm;
    memset(&dstrm, 0, (int)sizeof(dstrm));
    int def_init_ret = deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));

    // step 3: Operate
    int inf_ret = inflate(&istrm, 0);
    unsigned long codes_used = inflateCodesUsed(&istrm);
    Bytef *uncomp_buf = (Bytef *)malloc((size_t)src_len);
    memset(uncomp_buf, 0, (int)src_len);
    uLongf destLen = (uLongf)src_len;
    int uncmp_ret = uncompress(uncomp_buf, &destLen, comp_buf, (uLong)comp_len);

    // step 4: Validate & Cleanup
    int inf_end_ret = inflateEnd(&istrm);
    int def_end_ret = deflateEnd(&dstrm);
    free(comp_buf);
    free(out_buf);
    free(uncomp_buf);
    (void)comp_ret;
    (void)compile_flags;
    (void)inf_init_ret;
    (void)def_init_ret;
    (void)inf_ret;
    (void)codes_used;
    (void)uncmp_ret;
    (void)destLen;
    (void)inf_end_ret;
    (void)def_end_ret;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}