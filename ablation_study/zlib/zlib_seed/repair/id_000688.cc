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
//<ID> 688
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    Bytef plain_in[1024];
    memset(plain_in, 'A', sizeof(plain_in));
    uLong plain_len = (uLong)sizeof(plain_in);
    uLong comp_bound = compressBound(plain_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_bound);
    memset(comp_buf, 0, (size_t)comp_bound);
    uLongf comp_len = (uLongf)comp_bound;
    int comp_ret = compress(comp_buf, &comp_len, plain_in, plain_len);

    // step 2: Configure
    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    int inf_init_ret = inflateInit_(&inf, version, (int)sizeof(z_stream));
    inf.next_in = comp_buf;
    inf.avail_in = (uInt)comp_len;
    Bytef decomp_buf[1024];
    memset(decomp_buf, 0, sizeof(decomp_buf));
    inf.next_out = decomp_buf;
    inf.avail_out = (uInt)plain_len;
    int inf_reset_ret = inflateReset(&inf);

    // step 3: Operate
    int inf_ret = inflate(&inf, 4); /* 4 == Z_FINISH */
    int inf_end_ret = inflateEnd(&inf);

    gzFile gw = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gw_write = gzwrite(gw, comp_buf, (unsigned int)comp_len);
    int gw_close = gzclose(gw);
    gzFile gr = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    off64_t offset64 = gzoffset64(gr);
    int gr_close = gzclose(gr);

    uLong ad1 = adler32(0UL, plain_in, (uInt)(plain_len / 2));
    uLong ad2 = adler32(0UL, plain_in + (plain_len / 2), (uInt)(plain_len - (plain_len / 2)));
    uLong combined_ad = adler32_combine64(ad1, ad2, (off64_t)plain_len);

    // step 4: Validate / Cleanup
    free(comp_buf);
    (void)version;
    (void)comp_bound;
    (void)comp_ret;
    (void)comp_len;
    (void)inf_init_ret;
    (void)inf_reset_ret;
    (void)inf_ret;
    (void)inf_end_ret;
    (void)decomp_buf;
    (void)gw_write;
    (void)gw_close;
    (void)offset64;
    (void)gr_close;
    (void)ad1;
    (void)ad2;
    (void)combined_ad;
    // API sequence test completed successfully
    return 66;
}