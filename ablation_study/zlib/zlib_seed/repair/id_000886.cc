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
//<ID> 886
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef sample[] = "Example payload for deflate -> gz write -> adler32 demonstration.";
    uLong sample_len = (uLong)(sizeof(sample) - 1);
    uLong adler_before = adler32(0UL, sample, (uInt)sample_len);

    // step 2: Configure
    z_stream def;
    memset(&def, 0, sizeof(def));
    int def_init_ret = deflateInit_(&def, 6, version, (int)sizeof(z_stream));
    def.next_in = (Bytef *)sample;
    def.avail_in = (uInt)sample_len;
    uLong out_bound = deflateBound(&def, sample_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)out_bound);
    memset(comp_buf, 0, (size_t)out_bound);
    def.next_out = comp_buf;
    def.avail_out = (uInt)out_bound;

    // step 3: Operate
    int def_ret = deflate(&def, 4);
    uLong comp_len = def.total_out;
    gzFile gzf_w = gzopen("zlib_api_sequence_tmp.gz", "wb");
    int gz_wr = 0;
    if (gzf_w) gz_wr = gzwrite(gzf_w, (voidpc)comp_buf, (unsigned int)comp_len);
    int gz_cl_w = 0;
    if (gzf_w) gz_cl_w = gzclose(gzf_w);
    uLong adler_after = adler32(0UL, comp_buf, (uInt)(comp_len > (uLong)0xFFFFFFFFu ? 0xFFFFFFFFu : (uLong)comp_len));

    // step 4: Validate / Cleanup
    int def_end = deflateEnd(&def);
    free(comp_buf);
    (void)version;
    (void)adler_before;
    (void)def_init_ret;
    (void)out_bound;
    (void)def_ret;
    (void)comp_len;
    (void)gz_wr;
    (void)gz_cl_w;
    (void)adler_after;
    (void)def_end;
    // API sequence test completed successfully
    return 66;
}