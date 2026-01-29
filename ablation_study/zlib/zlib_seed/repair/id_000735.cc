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
//<ID> 735
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
    Bytef *comp = (Bytef *)malloc((size_t)out_bound);
    memset(comp, 0, (size_t)out_bound);
    def.next_out = comp;
    def.avail_out = (uInt)out_bound;

    // step 2: Configure
    int def_ret = deflate(&def, 4);
    uLong comp_size = def.total_out;

    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    int inf_init_ret = inflateInit_(&inf, version, (int)sizeof(z_stream));
    Bytef dict[128];
    memset(dict, 'D', sizeof(dict));
    uInt dict_len = (uInt)sizeof(dict);
    int dict_ret = inflateSetDictionary(&inf, dict, dict_len);

    // step 3: Operate
    inf.next_in = comp;
    inf.avail_in = (uInt)comp_size;
    Bytef *decomp = (Bytef *)malloc((size_t)plain_len);
    memset(decomp, 0, (size_t)plain_len);
    inf.next_out = decomp;
    inf.avail_out = (uInt)plain_len;
    int inf_ret = inflate(&inf, 4);

    uInt half = (uInt)(plain_len / 2UL);
    uLong a1 = adler32(0UL, plain, half);
    uLong a2 = adler32(0UL, plain + half, (uInt)(plain_len - half));
    off64_t half_off = (off64_t)half;
    uLong combined = adler32_combine64(a1, a2, half_off);

    gzFile gw = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int wrote = gzwrite(gw, plain, (unsigned int)plain_len);
    int close_w = gzclose(gw);
    gzFile gr = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    off64_t seek_pos = gzseek64(gr, (off64_t)100, 0);

    // step 4: Validate / Cleanup
    int close_r = gzclose(gr);
    int def_end_ret = deflateEnd(&def);
    int inf_end_ret = inflateEnd(&inf);
    free(comp);
    free(decomp);

    (void)version;
    (void)def_init_ret;
    (void)out_bound;
    (void)def_ret;
    (void)comp_size;
    (void)inf_init_ret;
    (void)dict_ret;
    (void)inf_ret;
    (void)a1;
    (void)a2;
    (void)half_off;
    (void)combined;
    (void)wrote;
    (void)close_w;
    (void)seek_pos;
    (void)close_r;
    (void)def_end_ret;
    (void)inf_end_ret;
    // API sequence test completed successfully
    return 66;
}