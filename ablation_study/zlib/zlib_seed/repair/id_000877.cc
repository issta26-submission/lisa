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
//<ID> 877
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef input[] = "Example payload for zlib compress/uncompress and checksum combine.";
    uLong input_len = (uLong)(sizeof(input) - 1);
    uLongf comp_bound = compressBound(input_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_bound);
    memset(comp_buf, 0, (size_t)comp_bound);
    uLongf comp_len = comp_bound;
    Bytef *uncomp_buf = (Bytef *)malloc((size_t)(input_len + 16));
    memset(uncomp_buf, 0, (size_t)(input_len + 16));
    uLongf uncomp_len = input_len;

    // step 2: Configure
    int comp_ret = compress(comp_buf, &comp_len, input, input_len);
    int uncomp_ret = uncompress(uncomp_buf, &uncomp_len, comp_buf, comp_len);

    // step 3: Operate
    off_t part1_len = (off_t)(input_len / 2);
    off_t part2_len = (off_t)(input_len - (uLong)part1_len);
    uLong a1 = adler32(1UL, input, (uInt)part1_len);
    uLong a2 = adler32(1UL, input + part1_len, (uInt)part2_len);
    uLong combined_adler = adler32_combine(a1, a2, part2_len);

    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    int inf_init = inflateInit_(&inf, version, (int)sizeof(z_stream));
    inf.next_in = comp_buf;
    inf.avail_in = (uInt)comp_len;
    Bytef *inflate_out = (Bytef *)malloc((size_t)(input_len + 32));
    memset(inflate_out, 0, (size_t)(input_len + 32));
    inf.next_out = inflate_out;
    inf.avail_out = (uInt)(input_len + 32);
    int inf_ret = inflate(&inf, 4);

    // step 4: Validate / Cleanup
    int inf_end_ret = inflateEnd(&inf);
    free(comp_buf);
    free(uncomp_buf);
    free(inflate_out);
    (void)version;
    (void)comp_bound;
    (void)comp_len;
    (void)uncomp_len;
    (void)comp_ret;
    (void)uncomp_ret;
    (void)a1;
    (void)a2;
    (void)combined_adler;
    (void)inf_init;
    (void)inf_ret;
    (void)inf_end_ret;
    // API sequence test completed successfully
    return 66;
}