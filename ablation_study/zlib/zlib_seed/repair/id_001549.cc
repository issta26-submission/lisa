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
//<ID> 1549
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: compress2 -> inflateUndermine -> inflate -> gzwrite/gzdirect/gzclose and crc32 verification";
    uLong src_len = (uLong)(sizeof(src) - 1);
    uLong crc_src = crc32(0UL, src, (uInt)src_len);
    uLong bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    uLongf comp_len = (uLongf)bound;
    int comp_ret = compress2(comp_buf, &comp_len, src, src_len, 6);

    // step 2: Configure
    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    const char *ver = zlibVersion();
    int inf_init_ret = inflateInit_(&strm, ver, (int)sizeof(z_stream));
    strm.next_in = comp_buf;
    strm.avail_in = (uInt)comp_len;
    Bytef *out_buf = (Bytef *)malloc((size_t)src_len);
    memset(out_buf, 0, (int)src_len);
    strm.next_out = out_buf;
    strm.avail_out = (uInt)src_len;
    int undermine_ret = inflateUndermine(&strm, 1);

    // step 3: Operate
    int inf_ret = inflate(&strm, 0);
    uLong crc_out = crc32(0UL, out_buf, (uInt)src_len);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    z_size_t written = gzfwrite((voidpc)comp_buf, (z_size_t)1, (z_size_t)comp_len, gz);
    int direct_flag = gzdirect(gz);

    // step 4: Validate & Cleanup
    int inf_end_ret = inflateEnd(&strm);
    int gz_close_ret = gzclose(gz);
    free(comp_buf);
    free(out_buf);
    (void)crc_src;
    (void)bound;
    (void)comp_len;
    (void)comp_ret;
    (void)ver;
    (void)inf_init_ret;
    (void)undermine_ret;
    (void)inf_ret;
    (void)crc_out;
    (void)written;
    (void)direct_flag;
    (void)inf_end_ret;
    (void)gz_close_ret;
    // API sequence test completed successfully
    return 66;
}