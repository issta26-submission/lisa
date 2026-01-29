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
//<ID> 689
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
    memset(plain_in, 'A', sizeof(plain_in));
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
    Bytef decomp_buf[256];
    memset(decomp_buf, 0, sizeof(decomp_buf));
    inf.next_out = decomp_buf;
    inf.avail_out = (uInt)plain_len;
    int inf_reset_ret = inflateReset(&inf);

    uLong a1 = adler32(0UL, plain_in, 128);
    uLong a2 = adler32(0UL, plain_in + 128, 128);
    off64_t part_len = (off64_t)128;
    uLong combined_adler = adler32_combine64(a1, a2, part_len);

    // step 3: Operate
    int inf_ret = inflate(&inf, 4);
    int inf_end_ret = inflateEnd(&inf);
    gzFile gz = gzopen64("test_zlib_api_sequence_tmp.gz", "wb");
    int gz_w = gzwrite(gz, comp_buf, (unsigned int)comp_len);
    off64_t gz_off = gzoffset64(gz);
    int gz_close_ret = gzclose(gz);

    // step 4: Validate / Cleanup
    free(comp_buf);
    (void)version;
    (void)plain_len;
    (void)comp_bound;
    (void)comp_ret;
    (void)comp_len;
    (void)inf_init_ret;
    (void)inf_reset_ret;
    (void)a1;
    (void)a2;
    (void)part_len;
    (void)combined_adler;
    (void)inf_ret;
    (void)inf_end_ret;
    (void)gz_w;
    (void)gz_off;
    (void)gz_close_ret;
    (void)decomp_buf;
    // API sequence test completed successfully
    return 66;
}