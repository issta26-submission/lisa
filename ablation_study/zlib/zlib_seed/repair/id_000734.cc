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
//<ID> 734
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    Bytef plain[1024];
    memset(plain, 'A', sizeof(plain));
    uLong plain_len = (uLong)sizeof(plain);
    uInt half = (uInt)(plain_len / 2UL);
    uLong a1 = adler32(0UL, plain, half);
    uLong a2 = adler32(0UL, plain + half, (uInt)(plain_len - half));
    off64_t half_off = (off64_t)half;
    uLong combined_adler = adler32_combine64(a1, a2, half_off);

    // step 2: Configure (deflate)
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
    uLong comp_len = def.total_out;
    int def_end_ret = deflateEnd(&def);

    // step 3: Operate (write gz, seek, inflate dictionary)
    const char *fname = "test_zlib_api_sequence_tmp.gz";
    gzFile gzw = gzopen(fname, "wb");
    int written = gzwrite(gzw, out_buf, (unsigned int)comp_len);
    int close_w = gzclose(gzw);
    gzFile gzr = gzopen(fname, "rb");
    off64_t seek_pos = gzseek64(gzr, (off64_t)1, 0);
    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    int inf_init_ret = inflateInit_(&inf, version, (int)sizeof(z_stream));
    uInt dict_len = (uInt)32;
    int set_dict_ret = inflateSetDictionary(&inf, plain, dict_len);

    // step 4: Validate / Cleanup
    size_t to_read = (size_t)256;
    unsigned char *read_buf = (unsigned char *)malloc(to_read);
    memset(read_buf, 0, to_read);
    z_size_t read_items = gzfread(read_buf, 1, (z_size_t)to_read, gzr);
    int close_r = gzclose(gzr);
    int inf_end_ret = inflateEnd(&inf);
    free(out_buf);
    free(read_buf);
    (void)version;
    (void)plain_len;
    (void)def_init_ret;
    (void)def_ret;
    (void)def_end_ret;
    (void)written;
    (void)close_w;
    (void)seek_pos;
    (void)inf_init_ret;
    (void)set_dict_ret;
    (void)read_items;
    (void)close_r;
    (void)inf_end_ret;
    (void)combined_adler;
    // API sequence test completed successfully
    return 66;
}