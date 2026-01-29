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
//<ID> 1352
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence: deflate, deflatePending, gzwrite, gzoffset, adler32_combine64";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong ad1 = adler32(0L, src, (uInt)src_len);
    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    int init_ret = deflateInit_(&strm, 6, version, (int)sizeof(z_stream));

    // step 2: Configure
    strm.next_in = (Bytef *)src;
    strm.avail_in = (uInt)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)src_len);
    uLong bound = deflateBound(&strm, src_len);
    Bytef *out_buf = (Bytef *)malloc((size_t)bound);
    memset(out_buf, 0, (int)bound);
    strm.next_out = out_buf;
    strm.avail_out = (uInt)(bound > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)bound);

    // step 3: Operate
    int def_ret = deflate(&strm, 4);
    unsigned int pending = 0;
    int bits = 0;
    int pend_ret = deflatePending(&strm, &pending, &bits);
    int end_ret = deflateEnd(&strm);
    uLong comp_len = (uLong)strm.total_out;
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int gw = gzwrite(gz, out_buf, (unsigned int)(comp_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)comp_len));
    off_t offset = gzoffset(gz);
    int close_ret = gzclose(gz);

    // step 4: Validate & Cleanup
    uLong ad_comb = adler32_combine64(ad1, ad1, (off64_t)comp_len);
    free(out_buf);
    (void)init_ret;
    (void)def_ret;
    (void)pend_ret;
    (void)pending;
    (void)bits;
    (void)end_ret;
    (void)gw;
    (void)offset;
    (void)close_ret;
    (void)ad_comb;
    (void)version;
    // API sequence test completed successfully
    return 66;
}