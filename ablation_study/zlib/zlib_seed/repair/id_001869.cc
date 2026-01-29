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
//<ID> 1869
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
    uLong ad1 = adler32(0UL, src, (uInt)src_len);
    uLong half = (uLong)(src_len / 2UL);
    uLong len1 = half;
    uLong len2 = src_len - len1;
    uLong ad2 = adler32(0UL, src + len1, (uInt)len2);
    uLong combined_adler = adler32_combine(ad1, ad2, (off_t)len2);
    uLong combined_adler64 = adler32_combine64(ad1, ad2, (off64_t)len2);

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
    int def_ret = deflate(&strm, 4);
    uLong comp_len = strm.total_out;

    // step 3: Operate - copy compressor state and write compressed data
    z_stream copy_strm;
    memset(&copy_strm, 0, (int)sizeof(copy_strm));
    int copy_ret = deflateCopy(&copy_strm, &strm);
    gzFile gzf = gzdopen(1, "wb");
    int gz_wr = gzwrite(gzf, comp_buf, (unsigned int)(comp_len > 0 ? (unsigned int)comp_len : 0U));
    int gz_close = gzclose(gzf);

    // step 4: Validate & Cleanup
    int end_ret_src = deflateEnd(&strm);
    int end_ret_copy = deflateEnd(&copy_strm);
    uLong ad_full = adler32(0UL, src, (uInt)src_len);
    (void)init_ret;
    (void)def_ret;
    (void)copy_ret;
    (void)gz_wr;
    (void)gz_close;
    (void)end_ret_src;
    (void)end_ret_copy;
    (void)ad_full;
    (void)combined_adler;
    (void)combined_adler64;
    (void)ver;
    free(comp_buf);
    // API sequence test completed successfully
    return 66;
}