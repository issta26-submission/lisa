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
//<ID> 888
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef sample[] = "Example payload for zlib API sequence test to compress and write.";
    uLong sample_len = (uLong)(sizeof(sample) - 1);
    uLong adler_original = adler32(0UL, sample, (uInt)sample_len);

    // step 2: Configure
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    int init_ret = deflateInit_(&strm, 6, version, (int)sizeof(z_stream));
    uLong out_bound = deflateBound(&strm, sample_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)out_bound);
    memset(comp_buf, 0, (size_t)out_bound);
    strm.next_in = (Bytef *)sample;
    strm.avail_in = (uInt)sample_len;
    strm.next_out = comp_buf;
    strm.avail_out = (uInt)out_bound;

    // step 3: Operate
    int def_ret = deflate(&strm, 4);
    uLong comp_len = strm.total_out;
    uLong adler_compressed = 0UL;
    if (comp_len > 0) {
        adler_compressed = adler32(0UL, comp_buf, (uInt)comp_len);
    }
    gzFile gzf = gzopen("zlib_api_sequence_tmp.gz", "wb");
    int gz_wr = gzwrite(gzf, comp_buf, (unsigned int)comp_len);
    int gz_cl = gzclose(gzf);

    // step 4: Validate / Cleanup
    int def_end = deflateEnd(&strm);
    free(comp_buf);
    (void)version;
    (void)adler_original;
    (void)init_ret;
    (void)out_bound;
    (void)def_ret;
    (void)comp_len;
    (void)adler_compressed;
    (void)gz_wr;
    (void)gz_cl;
    (void)def_end;
    // API sequence test completed successfully
    return 66;
}