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
//<ID> 873
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize / Prepare data
    const char *version = zlibVersion();
    const Bytef sample[] = "Hello zlib! Data for compression and checksum combination test.";
    uLong sample_len = (uLong)(sizeof(sample) - 1);
    uLong part_len = sample_len / 2;
    uInt part1_len = (uInt)part_len;
    uInt part2_len = (uInt)(sample_len - part_len);
    uLong adler1 = adler32(0UL, sample, part1_len);
    uLong adler2 = adler32(0UL, sample + part_len, part2_len);
    uLong combined_adler = adler32_combine(adler1, adler2, (off_t)part2_len);

    // step 2: Compress and allocate buffers
    uLong comp_bound = compressBound(sample_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_bound);
    memset(comp_buf, 0, (size_t)comp_bound);
    uLongf comp_len = (uLongf)comp_bound;
    int comp_ret = compress(comp_buf, &comp_len, sample, sample_len);
    uLongf decomp_len = sample_len;
    Bytef *decomp_buf = (Bytef *)malloc((size_t)decomp_len);
    memset(decomp_buf, 0, (size_t)decomp_len);
    int uncomp_ret = uncompress(decomp_buf, &decomp_len, comp_buf, (uLong)comp_len);

    // step 3: Inflate with z_stream and compute/integrate results
    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    int inf_init_ret = inflateInit_(&inf, version, (int)sizeof(z_stream));
    inf.next_in = comp_buf;
    inf.avail_in = (uInt)comp_len;
    inf.next_out = decomp_buf;
    inf.avail_out = (uInt)decomp_len;
    int inf_ret = inflate(&inf, 4);
    int inf_end_ret = inflateEnd(&inf);

    // step 4: Validate / Cleanup
    free(comp_buf);
    free(decomp_buf);
    (void)version;
    (void)part1_len;
    (void)part2_len;
    (void)adler1;
    (void)adler2;
    (void)combined_adler;
    (void)comp_bound;
    (void)comp_len;
    (void)comp_ret;
    (void)decomp_len;
    (void)uncomp_ret;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)inf_end_ret;
    // API sequence test completed successfully
    return 66;
}