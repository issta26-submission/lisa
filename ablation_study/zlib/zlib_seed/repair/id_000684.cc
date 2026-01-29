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
//<ID> 684
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    Bytef plain_in[256];
    memset(plain_in, 'Z', sizeof(plain_in));
    uLong plain_len = (uLong)sizeof(plain_in);
    uLong comp_bound = compressBound(plain_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_bound);
    memset(comp_buf, 0, (size_t)comp_bound);
    uLongf comp_len = (uLongf)comp_bound;
    int comp_ret = compress2(comp_buf, &comp_len, plain_in, plain_len, 6);

    // step 2: Configure
    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    int inf_init_ret = inflateInit_(&inf, version, (int)sizeof(z_stream));
    inf.next_in = comp_buf;
    inf.avail_in = (uInt)comp_len;
    Bytef decomp1[256];
    memset(decomp1, 0, sizeof(decomp1));
    inf.next_out = decomp1;
    inf.avail_out = (uInt)plain_len;
    Bytef decomp2[256];
    memset(decomp2, 0, sizeof(decomp2));
    gzFile gw = gzopen64("tmp_zlib_api_sequence.gz", "wb");
    int gw_write = gzwrite(gw, comp_buf, (unsigned int)comp_len);
    int gw_close = gzclose(gw);
    gzFile gr = gzopen64("tmp_zlib_api_sequence.gz", "rb");

    // step 3: Operate
    int inf_ret1 = inflate(&inf, 4);
    int inf_reset_ret = inflateReset(&inf);
    inf.next_in = comp_buf;
    inf.avail_in = (uInt)comp_len;
    inf.next_out = decomp2;
    inf.avail_out = (uInt)plain_len;
    int inf_ret2 = inflate(&inf, 4);
    off64_t file_offset = gzoffset64(gr);

    // step 4: Validate / Cleanup
    uLong ad1 = adler32(0UL, plain_in, (uInt)(plain_len / 2));
    uLong ad2 = adler32(0UL, plain_in + (plain_len / 2), (uInt)(plain_len - (plain_len / 2)));
    uLong ad_combined = adler32_combine64(ad1, ad2, (off64_t)(plain_len - (plain_len / 2)));
    int inf_end_ret = inflateEnd(&inf);
    int gr_close = gzclose(gr);
    free(comp_buf);
    (void)version;
    (void)comp_bound;
    (void)comp_ret;
    (void)comp_len;
    (void)inf_init_ret;
    (void)inf_ret1;
    (void)inf_reset_ret;
    (void)inf_ret2;
    (void)file_offset;
    (void)gw_write;
    (void)gw_close;
    (void)ad1;
    (void)ad2;
    (void)ad_combined;
    (void)inf_end_ret;
    (void)gr_close;
    (void)decomp1;
    (void)decomp2;
    // API sequence test completed successfully
    return 66;
}