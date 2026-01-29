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
//<ID> 1861
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize & Prepare data
    const Bytef src[] = "zlib API sequence test: deflate -> deflateCopy -> gzwrite -> adler32_combine";
    uLong src_len = (uLong)(sizeof(src) - 1);
    uLong adler_part1 = adler32(0UL, src, (uInt)src_len);
    z_stream src_strm;
    memset(&src_strm, 0, (int)sizeof(src_strm));
    const char *ver = zlibVersion();
    int def_init_ret = deflateInit_(&src_strm, 6, ver, (int)sizeof(z_stream));

    // step 2: Configure compressor and buffers
    src_strm.next_in = (Bytef *)src;
    src_strm.avail_in = (uInt)src_len;
    uLong bound = deflateBound(&src_strm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    src_strm.next_out = comp_buf;
    src_strm.avail_out = (uInt)bound;
    int def_ret = deflate(&src_strm, 4);
    uLong comp_len = src_strm.total_out;

    // step 3: Copy deflate state and write compressed data via gzwrite
    z_stream copy_strm;
    memset(&copy_strm, 0, (int)sizeof(copy_strm));
    int copy_ret = deflateCopy(&copy_strm, &src_strm);
    gzFile gzf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gz_written = gzwrite(gzf, comp_buf, (unsigned int)(comp_len > 0 ? (unsigned int)comp_len : 0U));
    int gz_close_ret = gzclose(gzf);

    // step 4: Validate & Cleanup
    uLong adler_part2 = adler32(0UL, comp_buf, (uInt)(comp_len > 0 ? comp_len : 0UL));
    uLong adler_combined = adler32_combine(adler_part1, adler_part2, (off_t)src_len);
    uLong adler_combined64 = adler32_combine64(adler_part1, adler_part2, (off64_t)src_len);
    int def_end_src = deflateEnd(&src_strm);
    int def_end_copy = deflateEnd(&copy_strm);
    free(comp_buf);
    (void)ver;
    (void)def_init_ret;
    (void)bound;
    (void)def_ret;
    (void)comp_len;
    (void)copy_ret;
    (void)gz_written;
    (void)gz_close_ret;
    (void)adler_part1;
    (void)adler_part2;
    (void)adler_combined;
    (void)adler_combined64;
    (void)def_end_src;
    (void)def_end_copy;
    // API sequence test completed successfully
    return 66;
}