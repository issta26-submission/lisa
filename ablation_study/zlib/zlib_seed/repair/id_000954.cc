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
//<ID> 954
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef source[] = "Zlib API sequence payload for deflatePending and inflateBackEnd test.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    z_stream inf_strm;
    memset(&inf_strm, 0, sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    uLong bound = compressBound(source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    uLongf comp_len = (uLongf)bound;
    int comp_ret = compress2(comp_buf, &comp_len, source, source_len, 6);

    // step 2: Configure
    unsigned int pending = 0;
    int bits = 0;
    int pending_ret = deflatePending(&def_strm, &pending, &bits);

    // step 3: Operate
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)comp_len;
    uLong decomp_cap = source_len + 64;
    Bytef *decomp_buf = (Bytef *)malloc((size_t)decomp_cap);
    memset(decomp_buf, 0, (size_t)decomp_cap);
    inf_strm.next_out = decomp_buf;
    inf_strm.avail_out = (uInt)decomp_cap;
    int inf_ret = inflate(&inf_strm, 0);
    unsigned long codes_used = inflateCodesUsed(&inf_strm);
    int back_end_ret = inflateBackEnd(&inf_strm);
    uLong ad1 = adler32(0L, source, (uInt)source_len);
    uLong ad2 = adler32(0L, decomp_buf, (uInt)inf_strm.total_out);
    uLong combined = adler32_combine64(ad1, ad2, (off64_t)inf_strm.total_out);

    // step 4: Cleanup
    int def_end_ret = deflateEnd(&def_strm);
    int inf_end_ret = inflateEnd(&inf_strm);
    free(comp_buf);
    free(decomp_buf);
    (void)version;
    (void)def_init_ret;
    (void)inf_init_ret;
    (void)bound;
    (void)comp_ret;
    (void)pending;
    (void)bits;
    (void)pending_ret;
    (void)inf_ret;
    (void)codes_used;
    (void)back_end_ret;
    (void)ad1;
    (void)ad2;
    (void)combined;
    (void)def_end_ret;
    (void)inf_end_ret;
    // API sequence test completed successfully
    return 66;
}