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
//<ID> 1358
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API fuzz sequence: deflate, deflatePending, gzwrite, adler32_combine64, gzoffset";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    int init_ret = deflateInit_(&strm, 6, version, (int)sizeof(z_stream));
    strm.next_in = (Bytef *)src;
    strm.avail_in = (uInt)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)src_len);

    // step 2: Configure
    uLong bound = deflateBound(&strm, src_len);
    Bytef *out_buf = (Bytef *)malloc((size_t)bound);
    memset(out_buf, 0, (int)bound);
    strm.next_out = out_buf;
    strm.avail_out = (uInt)(bound > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)bound);

    // step 3: Operate
    int def_ret = deflate(&strm, 4);
    unsigned int pending;
    int bits;
    int pending_ret = deflatePending(&strm, &pending, &bits);
    uLong comp_len = (uLong)strm.total_out;
    int end_ret = deflateEnd(&strm);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    unsigned int write_len = (unsigned int)(comp_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)comp_len);
    int gw = gzwrite(gz, out_buf, write_len);
    off_t offset = gzoffset(gz);
    int gc = gzclose(gz);

    // step 4: Validate & Cleanup
    uLong ad1 = adler32(0L, src, (uInt)src_len);
    uLong ad2 = adler32(ad1, out_buf, (uInt)(comp_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)comp_len));
    uLong combined = adler32_combine64(ad1, ad2, (off64_t)src_len);
    free(out_buf);
    (void)init_ret;
    (void)def_ret;
    (void)pending_ret;
    (void)pending;
    (void)bits;
    (void)end_ret;
    (void)gw;
    (void)gc;
    (void)offset;
    (void)combined;
    (void)version;
    (void)write_len;
    // API sequence test completed successfully
    return 66;
}