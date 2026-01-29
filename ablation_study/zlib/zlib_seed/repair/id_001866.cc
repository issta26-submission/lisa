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
//<ID> 1866
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize & prepare data and streams
    const Bytef src[] = "zlib API sequence test: deflate -> deflateCopy -> gzwrite -> adler32_combine";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *ver = zlibVersion();
    z_stream strm_orig;
    memset(&strm_orig, 0, (int)sizeof(strm_orig));
    int init_ret = deflateInit_(&strm_orig, 6, ver, (int)sizeof(z_stream));

    // step 2: Configure compressor buffers and perform deflate
    strm_orig.next_in = (Bytef *)src;
    strm_orig.avail_in = (uInt)src_len;
    uLong bound = deflateBound(&strm_orig, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    strm_orig.next_out = comp_buf;
    strm_orig.avail_out = (uInt)bound;
    int def_ret = deflate(&strm_orig, 4);
    uLong comp_len = strm_orig.total_out;

    // step 3: Duplicate stream state and write compressed data to a gz file
    z_stream strm_copy;
    memset(&strm_copy, 0, (int)sizeof(strm_copy));
    int copy_ret = deflateCopy(&strm_copy, &strm_orig);
    gzFile gzf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gz_wr_ret = gzwrite(gzf, comp_buf, (unsigned int)(comp_len > 0 ? (unsigned int)comp_len : 0U));
    int gz_close_ret = gzclose(gzf);

    // step 4: Validate checksums using adler32_combine and adler32_combine64, cleanup
    uLong adler_part1 = adler32(0UL, src, (uInt)src_len);
    uLong adler_part2 = adler32(0UL, comp_buf, (uInt)(comp_len > 0 ? (uInt)comp_len : 0U));
    uLong combined32 = adler32_combine(adler_part1, adler_part2, (off_t)comp_len);
    uLong combined64 = adler32_combine64(adler_part1, adler_part2, (off64_t)comp_len);
    int end_ret_orig = deflateEnd(&strm_orig);
    int end_ret_copy = deflateEnd(&strm_copy);
    free(comp_buf);
    (void)ver;
    (void)init_ret;
    (void)bound;
    (void)def_ret;
    (void)comp_len;
    (void)copy_ret;
    (void)gz_wr_ret;
    (void)gz_close_ret;
    (void)adler_part1;
    (void)adler_part2;
    (void)combined32;
    (void)combined64;
    (void)end_ret_orig;
    (void)end_ret_copy;
    // API sequence test completed successfully
    return 66;
}