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
//<ID> 1863
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize & Prepare data
    const Bytef src1[] = "zlib sequence chunk one: compress and checksum";
    const Bytef src2[] = "zlib sequence chunk two: additional data";
    uLong src1_len = (uLong)(sizeof(src1) - 1);
    uLong src2_len = (uLong)(sizeof(src2) - 1);
    uLong ad1 = adler32(0UL, src1, (uInt)src1_len);
    uLong ad2 = adler32(0UL, src2, (uInt)src2_len);
    uLong ad_comb = adler32_combine(ad1, ad2, (off_t)src2_len);
    uLong ad_comb64 = adler32_combine64(ad1, ad2, (off64_t)src2_len);

    // step 2: Configure compressor and buffers
    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    const char *ver = zlibVersion();
    int init_ret = deflateInit_(&strm, 6, ver, (int)sizeof(z_stream));
    strm.next_in = (Bytef *)src1;
    strm.avail_in = (uInt)src1_len;
    uLong bound = deflateBound(&strm, src1_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    strm.next_out = comp_buf;
    strm.avail_out = (uInt)bound;
    int dret = deflate(&strm, 4);
    uLong comp_len1 = strm.total_out;

    // step 3: Operate - copy deflate state, compress second chunk, and write via gzwrite
    z_stream copy;
    memset(&copy, 0, (int)sizeof(copy));
    int copy_ret = deflateCopy(&copy, &strm);
    Bytef *extra_buf = (Bytef *)malloc((size_t)bound);
    memset(extra_buf, 0, (int)bound);
    copy.next_in = (Bytef *)src2;
    copy.avail_in = (uInt)src2_len;
    copy.next_out = extra_buf;
    copy.avail_out = (uInt)bound;
    int dret2 = deflate(&copy, 4);
    uLong comp_len2 = copy.total_out;
    unsigned int write_len1 = (unsigned int)(comp_len1 > 0 ? (uInt)comp_len1 : 0U);
    unsigned int write_len2 = (unsigned int)(comp_len2 > 0 ? (uInt)comp_len2 : 0U);
    gzFile gzf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gz_w1 = gzwrite(gzf, comp_buf, write_len1);
    int gz_w2 = gzwrite(gzf, extra_buf, write_len2);
    int gz_cl = gzclose(gzf);

    // step 4: Validate & Cleanup
    uLong comp_ad1 = adler32(0UL, comp_buf, (uInt)(comp_len1 > 0 ? (uInt)comp_len1 : 0U));
    uLong comp_ad2 = adler32(0UL, extra_buf, (uInt)(comp_len2 > 0 ? (uInt)comp_len2 : 0U));
    uLong combined_from_comp = adler32_combine(comp_ad1, comp_ad2, (off_t)comp_len2);
    int end1 = deflateEnd(&strm);
    int end2 = deflateEnd(&copy);
    free(comp_buf);
    free(extra_buf);
    (void)ver;
    (void)init_ret;
    (void)dret;
    (void)copy_ret;
    (void)dret2;
    (void)gz_w1;
    (void)gz_w2;
    (void)gz_cl;
    (void)ad1;
    (void)ad2;
    (void)ad_comb;
    (void)ad_comb64;
    (void)comp_len1;
    (void)comp_len2;
    (void)combined_from_comp;
    (void)comp_ad1;
    (void)comp_ad2;
    (void)bound;
    (void)write_len1;
    (void)write_len2;
    (void)end1;
    (void)end2;
    // API sequence test completed successfully
    return 66;
}