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
//<ID> 736
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
    Bytef dict[64];
    memset(dict, 'D', sizeof(dict));
    uInt dict_len = (uInt)sizeof(dict);
    uInt half = (uInt)(plain_len / 2UL);
    uLong a1 = adler32(0UL, plain, half);
    uLong a2 = adler32(0UL, plain + half, (uInt)(plain_len - half));
    off64_t part_len = (off64_t)half;
    uLong combined = adler32_combine64(a1, a2, part_len);

    // step 2: Configure
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

    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    int inf_init_ret = inflateInit_(&inf, version, (int)sizeof(z_stream));
    int dict_set_ret = inflateSetDictionary(&inf, dict, dict_len);

    // step 3: Operate
    int def_ret = deflate(&def, 4);
    uLong total_out = def.total_out;
    int def_end_ret = deflateEnd(&def);
    int inf_end_ret = inflateEnd(&inf);

    const char *fname = "test_zlib_api_sequence_tmp.gz";
    gzFile gzw = gzopen(fname, "wb");
    int written = gzwrite(gzw, plain, (unsigned int)plain_len);
    int close_w = gzclose(gzw);
    gzFile gzr = gzopen(fname, "rb");
    off64_t seek_off = gzseek64(gzr, (off64_t)100, 0);
    z_size_t to_read = (z_size_t)128;
    voidp read_buf = malloc((size_t)to_read);
    memset(read_buf, 0, (size_t)to_read);
    z_size_t read_items = gzfread(read_buf, 1, to_read, gzr);
    off64_t new_pos = gzseek64(gzr, (off64_t)0, 1);
    int close_r = gzclose(gzr);

    // step 4: Validate / Cleanup
    free(out_buf);
    free(read_buf);
    (void)version;
    (void)plain_len;
    (void)dict_len;
    (void)def_init_ret;
    (void)inf_init_ret;
    (void)dict_set_ret;
    (void)def_ret;
    (void)total_out;
    (void)def_end_ret;
    (void)inf_end_ret;
    (void)written;
    (void)close_w;
    (void)seek_off;
    (void)read_items;
    (void)new_pos;
    (void)close_r;
    (void)a1;
    (void)a2;
    (void)part_len;
    (void)combined;
    // API sequence test completed successfully
    return 66;
}