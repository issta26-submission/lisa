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
//<ID> 733
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    Bytef plain[512];
    memset(plain, 'A', sizeof(plain));
    uLong plain_len = (uLong)sizeof(plain);

    z_stream def;
    memset(&def, 0, sizeof(def));
    int def_init_ret = deflateInit_(&def, 6, version, (int)sizeof(z_stream));
    def.next_in = plain;
    def.avail_in = (uInt)plain_len;
    uLong out_bound = deflateBound(&def, plain_len);
    Bytef *out_buf = (Bytef *)malloc((size_t)out_bound);
    memset(out_buf, 0, (size_t)out_bound);
    def.next_out = out_buf;
    def.avail_out = (uInt)out_bound;
    int def_ret = deflate(&def, 4);
    int def_end_ret = deflateEnd(&def);

    // step 2: Configure
    const char *fname = "test_zlib_api_sequence_tmp.gz";
    gzFile gzw = gzopen(fname, "wb");
    int written = gzwrite(gzw, out_buf, (unsigned int)def.total_out);
    int close_w = gzclose(gzw);
    gzFile gzr = gzopen(fname, "rb");
    off64_t seek_off = gzseek64(gzr, (off64_t)100, 0);

    // step 3: Operate
    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    int inf_init_ret = inflateInit_(&inf, version, (int)sizeof(z_stream));
    Bytef dict[64];
    memset(dict, 'D', sizeof(dict));
    uInt dict_len = (uInt)sizeof(dict);
    int dict_ret = inflateSetDictionary(&inf, dict, dict_len);
    uInt half = (uInt)(plain_len / 2UL);
    uLong a1 = adler32(0UL, plain, half);
    uLong a2 = adler32(0UL, plain + half, (uInt)(plain_len - half));
    off64_t part_len = (off64_t)half;
    uLong combined = adler32_combine64(a1, a2, part_len);

    // step 4: Validate / Cleanup
    int inf_end_ret = inflateEnd(&inf);
    int close_r = gzclose(gzr);
    free(out_buf);
    (void)version;
    (void)def_init_ret;
    (void)def_ret;
    (void)def_end_ret;
    (void)out_bound;
    (void)written;
    (void)close_w;
    (void)seek_off;
    (void)inf_init_ret;
    (void)dict_ret;
    (void)a1;
    (void)a2;
    (void)part_len;
    (void)combined;
    (void)inf_end_ret;
    (void)close_r;
    // API sequence test completed successfully
    return 66;
}