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
//<ID> 1536
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: compute crc, tune deflate, write gz, seek and cleanup";
    uLong src_len = (uLong)(sizeof(src) - 1);
    uLong crc_initial = crc32(0UL, src, (uInt)src_len);
    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    const char *ver = zlibVersion();
    int def_init_ret = deflateInit_(&strm, 6, ver, (int)sizeof(z_stream));
    uLong out_bound = deflateBound(&strm, src_len);
    Bytef *out_buf = (Bytef *)malloc((size_t)out_bound);
    memset(out_buf, 0, (int)out_bound);

    // step 2: Configure
    int tune_ret = deflateTune(&strm, 4, 8, 16, 32);
    gzFile gz = gzopen64("test_zlib_api_sequence.gz", "wb");
    z_size_t written_items = gzfwrite((voidpc)src, (z_size_t)1, (z_size_t)src_len, gz);

    // step 3: Operate
    off_t seek_pos = gzseek(gz, (off_t)0, SEEK_SET);
    strm.next_in = (Bytef *)src;
    strm.avail_in = (uInt)src_len;
    strm.next_out = out_buf;
    strm.avail_out = (uInt)out_bound;
    /* Note: we initialized and tuned the deflate stream; perform no further deflate calls here to keep linear sequence */
    (void)strm.next_in;
    (void)strm.avail_in;
    (void)strm.next_out;
    (void)strm.avail_out;

    // step 4: Validate & Cleanup
    int def_end_ret = deflateEnd(&strm);
    int gz_close_ret = gzclose(gz);
    free(out_buf);
    (void)crc_initial;
    (void)ver;
    (void)def_init_ret;
    (void)out_bound;
    (void)tune_ret;
    (void)written_items;
    (void)seek_pos;
    (void)def_end_ret;
    (void)gz_close_ret;
    // API sequence test completed successfully
    return 66;
}