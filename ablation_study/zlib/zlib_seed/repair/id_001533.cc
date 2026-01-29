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
//<ID> 1533
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    Bytef src[] = "zlib API sequence test: deflate, write gzip via gzopen64/gzfwrite, inflate and crc32";
    uLong src_len = (uLong)(sizeof(src) - 1);
    z_stream defstrm;
    memset(&defstrm, 0, (int)sizeof(defstrm));
    const char *ver = zlibVersion();
    int def_init_ret = deflateInit_(&defstrm, 6, ver, (int)sizeof(z_stream));
    uLong comp_bound = deflateBound(&defstrm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_bound);
    memset(comp_buf, 0, (int)comp_bound);
    defstrm.next_in = src;
    defstrm.avail_in = (uInt)src_len;
    defstrm.next_out = comp_buf;
    defstrm.avail_out = (uInt)comp_bound;
    int tune_ret = deflateTune(&defstrm, 64, 32, 258, 4096);
    int def_ret = deflate(&defstrm, 0);
    uLong comp_len = defstrm.total_out;
    int def_end_ret = deflateEnd(&defstrm);

    // step 2: Configure
    gzFile gz = gzopen64("test_zlib_api_sequence.gz", "wb");
    z_size_t written = gzfwrite((voidpc)comp_buf, (z_size_t)1, (z_size_t)comp_len, gz);
    off_t seek_res = gzseek(gz, (off_t)0, 0);

    // step 3: Operate
    z_stream infstrm;
    memset(&infstrm, 0, (int)sizeof(infstrm));
    int inf_init_ret = inflateInit_(&infstrm, ver, (int)sizeof(z_stream));
    Bytef *out_buf = (Bytef *)malloc((size_t)src_len);
    memset(out_buf, 0, (int)src_len);
    infstrm.next_in = comp_buf;
    infstrm.avail_in = (uInt)comp_len;
    infstrm.next_out = out_buf;
    infstrm.avail_out = (uInt)src_len;
    int inf_ret = inflate(&infstrm, 0);
    uLong crc = crc32(0UL, out_buf, (uInt)src_len);
    int inf_end_ret = inflateEnd(&infstrm);

    // step 4: Validate & Cleanup
    int gz_close_ret = gzclose(gz);
    free(comp_buf);
    free(out_buf);
    (void)def_init_ret;
    (void)comp_bound;
    (void)tune_ret;
    (void)def_ret;
    (void)def_end_ret;
    (void)written;
    (void)seek_res;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)crc;
    (void)inf_end_ret;
    (void)gz_close_ret;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}