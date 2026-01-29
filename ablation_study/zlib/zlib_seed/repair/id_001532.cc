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
//<ID> 1532
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: open gzip with gzopen64, write with gzfwrite, tune deflate and compute crc32";
    uLong src_len = (uLong)(sizeof(src) - 1);
    uLong crc_initial = crc32(0UL, src, (uInt)src_len);
    z_stream defstrm;
    memset(&defstrm, 0, (int)sizeof(defstrm));
    const char * ver = zlibVersion();
    int def_init_ret = deflateInit_(&defstrm, 6, ver, (int)sizeof(z_stream));
    uLong out_bound = deflateBound(&defstrm, src_len);
    Bytef * out_buf = (Bytef *)malloc((size_t)out_bound);
    memset(out_buf, 0, (int)out_bound);
    defstrm.next_in = (Bytef *)src;
    defstrm.avail_in = (uInt)src_len;
    defstrm.next_out = out_buf;
    defstrm.avail_out = (uInt)out_bound;
    int tune_ret = deflateTune(&defstrm, 16, 32, 64, 128);

    // step 2: Configure
    gzFile gz = gzopen64("test_zlib_api_sequence.gz", "wb");
    z_size_t write_count = gzfwrite((voidpc)src, (z_size_t)1, (z_size_t)src_len, gz);
    off_t seek_ret = gzseek(gz, (off_t)0, SEEK_SET);

    // step 3: Operate
    uLong crc_after_write = crc32(crc_initial, src, (uInt)src_len);
    (void)crc_after_write;
    (void)write_count;
    (void)seek_ret;

    // step 4: Validate & Cleanup
    int def_end_ret = deflateEnd(&defstrm);
    int gz_close_ret = gzclose(gz);
    free(out_buf);
    (void)def_init_ret;
    (void)out_bound;
    (void)tune_ret;
    (void)crc_initial;
    (void)ver;
    (void)def_end_ret;
    (void)gz_close_ret;
    // API sequence test completed successfully
    return 66;
}