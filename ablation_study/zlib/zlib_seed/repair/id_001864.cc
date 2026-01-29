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
//<ID> 1864
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize & Prepare data
    const Bytef src[] = "zlib API sequence test: use deflate, deflateCopy, gzwrite, adler32_combine*";
    uLong src_len = (uLong)(sizeof(src) - 1);
    uLong first_len = src_len / 2;
    uLong second_len = src_len - first_len;
    uLong adler_part1 = adler32(0UL, src, (uInt)first_len);
    uLong adler_part2 = adler32(0UL, src + first_len, (uInt)second_len);

    // step 2: Configure compressor and buffers
    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    const char *ver = zlibVersion();
    int init_ret = deflateInit_(&strm, 6, ver, (int)sizeof(z_stream));
    strm.next_in = (Bytef *)src;
    strm.avail_in = (uInt)src_len;
    uLong bound = deflateBound(&strm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    strm.next_out = comp_buf;
    strm.avail_out = (uInt)bound;

    // step 3: Operate - deflate, copy stream, write compressed data
    int def_ret = deflate(&strm, 4);
    uLong comp_len = strm.total_out;
    z_stream strm_copy;
    memset(&strm_copy, 0, (int)sizeof(strm_copy));
    int copy_ret = deflateCopy(&strm_copy, &strm);
    gzFile gzf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gz_wr = gzwrite(gzf, comp_buf, (unsigned int)comp_len);
    int gz_close_ret = gzclose(gzf);
    int def_end_copy = deflateEnd(&strm_copy);
    int def_end_orig = deflateEnd(&strm);

    // step 4: Validate & Cleanup
    uLong combined32 = adler32_combine(adler_part1, adler_part2, (off_t)second_len);
    uLong combined64 = adler32_combine64(adler_part1, adler_part2, (off64_t)second_len);
    free(comp_buf);
    (void)ver;
    (void)init_ret;
    (void)bound;
    (void)def_ret;
    (void)copy_ret;
    (void)gz_wr;
    (void)gz_close_ret;
    (void)def_end_copy;
    (void)def_end_orig;
    (void)combined32;
    (void)combined64;
    // API sequence test completed successfully
    return 66;
}