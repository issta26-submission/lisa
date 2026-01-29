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
//<ID> 686
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    Bytef plain_in[512];
    memset(plain_in, 'Z', sizeof(plain_in));
    uLong plain_len = (uLong)sizeof(plain_in);
    uLong part1 = plain_len / 2;
    uLong part2 = plain_len - part1;
    uLong ad1 = adler32(0UL, plain_in, (uInt)part1);
    uLong ad2 = adler32(0UL, plain_in + part1, (uInt)part2);
    uLong combined_ad = adler32_combine64(ad1, ad2, (off64_t)part2);

    // step 2: Configure
    uLong bound = compressBound(plain_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    uLongf comp_len = (uLongf)bound;
    int comp_ret = compress2(comp_buf, &comp_len, plain_in, plain_len, 6);

    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    int inf_init_ret = inflateInit_(&strm, version, (int)sizeof(z_stream));
    strm.next_in = comp_buf;
    strm.avail_in = (uInt)comp_len;
    Bytef decomp_buf[512];
    memset(decomp_buf, 0, sizeof(decomp_buf));
    strm.next_out = decomp_buf;
    strm.avail_out = (uInt)plain_len;
    int inf_reset_ret = inflateReset(&strm);

    // step 3: Operate
    int inf_ret = inflate(&strm, 4);
    gzFile gz = gzopen64("test_zlib_api_sequence_tmp.gz", "wb");
    int gw_write = gzwrite(gz, comp_buf, (unsigned int)comp_len);
    off64_t gz_off = gzoffset64(gz);
    int gz_close_ret = gzclose(gz);

    // step 4: Validate / Cleanup
    int inf_end_ret = inflateEnd(&strm);
    free(comp_buf);
    (void)version;
    (void)plain_len;
    (void)part1;
    (void)part2;
    (void)ad1;
    (void)ad2;
    (void)combined_ad;
    (void)bound;
    (void)comp_ret;
    (void)comp_len;
    (void)inf_init_ret;
    (void)inf_reset_ret;
    (void)inf_ret;
    (void)gw_write;
    (void)gz_off;
    (void)gz_close_ret;
    (void)inf_end_ret;
    (void)decomp_buf;
    // API sequence test completed successfully
    return 66;
}