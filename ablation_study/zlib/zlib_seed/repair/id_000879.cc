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
//<ID> 879
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef source[] = "The quick brown fox jumps over the lazy dog - zlib API sequence test";
    uLong source_len = (uLong)(sizeof(source) - 1);
    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    int inf_init_ret = inflateInit_(&inf, version, (int)sizeof(z_stream));

    // step 2: Configure
    uLongf comp_bound = compressBound(source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_bound);
    memset(comp_buf, 0, (size_t)comp_bound);
    uLongf comp_len = comp_bound;
    int comp_ret = compress(comp_buf, &comp_len, source, source_len);

    // step 3: Operate
    uLongf decomp_buf_len = source_len + 64;
    Bytef *decomp_buf = (Bytef *)malloc((size_t)decomp_buf_len);
    memset(decomp_buf, 0, (size_t)decomp_buf_len);
    int uncomp_ret = uncompress(decomp_buf, &decomp_buf_len, comp_buf, comp_len);
    uLong adler_src = adler32(0UL, source, (uInt)source_len);
    uLong adler_decomp = adler32(0UL, decomp_buf, (uInt)decomp_buf_len);
    uLong combined_adler = adler32_combine(adler_src, adler_decomp, (off_t)decomp_buf_len);

    // step 4: Validate / Cleanup
    int inf_end_ret = inflateEnd(&inf);
    free(comp_buf);
    free(decomp_buf);
    (void)version;
    (void)inf_init_ret;
    (void)comp_bound;
    (void)comp_ret;
    (void)comp_len;
    (void)uncomp_ret;
    (void)adler_src;
    (void)adler_decomp;
    (void)combined_adler;
    (void)inf_end_ret;
    // API sequence test completed successfully
    return 66;
}