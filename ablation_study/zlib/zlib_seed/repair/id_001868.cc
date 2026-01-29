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
//<ID> 1868
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
    uLong part1_len = src_len / 2UL;
    uLong part2_len = src_len - part1_len;
    uLong ad1 = adler32(0UL, src, (uInt)part1_len);
    uLong ad2 = adler32(0UL, src + part1_len, (uInt)part2_len);
    const char *ver = zlibVersion();

    // step 2: Configure compressor and buffers
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    int init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    def_strm.next_in = (Bytef *)src;
    def_strm.avail_in = (uInt)src_len;
    uLong bound = deflateBound(&def_strm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)bound;
    int def_ret = deflate(&def_strm, 4);
    uLong comp_len = def_strm.total_out;

    // step 3: Copy deflate stream and write compressed data to a gz file
    z_stream copy_strm;
    memset(&copy_strm, 0, (int)sizeof(copy_strm));
    int copy_ret = deflateCopy(&copy_strm, &def_strm);
    Bytef *comp_buf2 = (Bytef *)malloc((size_t)bound);
    memset(comp_buf2, 0, (int)bound);
    copy_strm.next_out = comp_buf2;
    copy_strm.avail_out = (uInt)bound;
    int def_copy_ret = deflate(&copy_strm, 4);
    gzFile gzf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gz_wr = gzwrite(gzf, comp_buf, (unsigned int)(comp_len > 0 ? (unsigned int)comp_len : 0U));
    int gz_close_ret = gzclose(gzf);

    // step 4: Validate & Cleanup
    uLong combined = adler32_combine(ad1, ad2, (off_t)part2_len);
    uLong combined64 = adler32_combine64(ad1, ad2, (off64_t)part2_len);
    int end_ret_def = deflateEnd(&def_strm);
    int end_ret_copy = deflateEnd(&copy_strm);
    free(comp_buf);
    free(comp_buf2);
    (void)init_ret;
    (void)def_ret;
    (void)comp_len;
    (void)copy_ret;
    (void)def_copy_ret;
    (void)gz_wr;
    (void)gz_close_ret;
    (void)combined;
    (void)combined64;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}