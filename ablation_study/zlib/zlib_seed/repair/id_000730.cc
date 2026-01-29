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
//<ID> 730
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
    uLong compressed_len = def.total_out;

    gzFile gw = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gw_written = gzwrite(gw, out_buf, (unsigned int)compressed_len);
    int gw_close = gzclose(gw);

    // step 2: Configure
    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    int inf_init_ret = inflateInit_(&inf, version, (int)sizeof(z_stream));
    int dict_ret = inflateSetDictionary(&inf, plain, (uInt)plain_len);

    gzFile gr = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    off64_t seek_ret = gzseek64(gr, (off64_t)100, 0);

    // step 3: Operate
    z_size_t to_read = (z_size_t)128;
    voidp read_buf = malloc((size_t)to_read);
    memset(read_buf, 0, (size_t)to_read);
    z_size_t read_items = gzfread(read_buf, (z_size_t)1, to_read, gr);
    off64_t cur_pos = gzseek64(gr, (off64_t)0, 1);

    uInt half = (uInt)(plain_len / 2UL);
    uLong a1 = adler32(0UL, plain, half);
    uLong a2 = adler32(0UL, plain + half, (uInt)(plain_len - half));
    off64_t part_len = (off64_t)half;
    uLong combined = adler32_combine64(a1, a2, part_len);

    // step 4: Validate / Cleanup
    int def_end_ret = deflateEnd(&def);
    int inf_end_ret = inflateEnd(&inf);
    int gr_close = gzclose(gr);
    free(out_buf);
    free(read_buf);

    (void)version;
    (void)def_init_ret;
    (void)def_ret;
    (void)compressed_len;
    (void)gw_written;
    (void)gw_close;
    (void)inf_init_ret;
    (void)dict_ret;
    (void)seek_ret;
    (void)to_read;
    (void)read_items;
    (void)cur_pos;
    (void)a1;
    (void)a2;
    (void)part_len;
    (void)combined;
    (void)def_end_ret;
    (void)inf_end_ret;
    (void)gr_close;
    // API sequence test completed successfully
    return 66;
}