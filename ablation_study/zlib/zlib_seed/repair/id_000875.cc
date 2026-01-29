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
//<ID> 875
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef sample[] = "This is a test payload for zlib compress/uncompress and adler32 combination.";
    uLong sample_len = (uLong)(sizeof(sample) - 1);
    uInt part1_len = (uInt)(sample_len / 2);
    uInt part2_len = (uInt)(sample_len - part1_len);
    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    int inf_init_ret = inflateInit_(&inf, version, (int)sizeof(z_stream));

    // step 2: Configure
    uLong bound = compressBound(sample_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    uLongf comp_len = (uLongf)bound;

    // step 3: Operate
    int comp_ret = compress(comp_buf, &comp_len, sample, sample_len);
    Bytef *decomp_buf = (Bytef *)malloc((size_t)(sample_len + 16));
    memset(decomp_buf, 0, (size_t)(sample_len + 16));
    uLongf decomp_len = (uLongf)(sample_len + 16);
    int uncomp_ret = uncompress(decomp_buf, &decomp_len, comp_buf, comp_len);
    uLong ad1 = adler32(1L, sample, part1_len);
    uLong ad2 = adler32(1L, sample + part1_len, part2_len);
    off_t tail_len = (off_t)part2_len;
    uLong ad_comb = adler32_combine(ad1, ad2, tail_len);

    // step 4: Validate / Cleanup
    int inf_end = inflateEnd(&inf);
    free(comp_buf);
    free(decomp_buf);
    (void)version;
    (void)inf_init_ret;
    (void)bound;
    (void)comp_ret;
    (void)uncomp_ret;
    (void)decomp_len;
    (void)ad1;
    (void)ad2;
    (void)ad_comb;
    (void)inf_end;
    // API sequence test completed successfully
    return 66;
}