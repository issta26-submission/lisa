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
//<ID> 1865
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
    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    const char *ver = zlibVersion();
    int init_ret = deflateInit_(&strm, 6, ver, (int)sizeof(z_stream));

    // step 2: Configure streams and buffers
    strm.next_in = (Bytef *)src;
    strm.avail_in = (uInt)src_len;
    z_stream copy_strm;
    memset(&copy_strm, 0, (int)sizeof(copy_strm));
    int copy_ret = deflateCopy(&copy_strm, &strm);
    uLong bound = deflateBound(&strm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    Bytef *comp_buf2 = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    memset(comp_buf2, 0, (int)bound);
    strm.next_out = comp_buf;
    strm.avail_out = (uInt)bound;
    copy_strm.next_out = comp_buf2;
    copy_strm.avail_out = (uInt)bound;

    // step 3: Operate - compress with both streams, write compressed data, compute checksums
    int def_ret1 = deflate(&strm, 4);
    int def_ret2 = deflate(&copy_strm, 4);
    uLong comp_len = strm.total_out;
    uLong comp2_len = copy_strm.total_out;
    gzFile gzf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gz_wr = gzwrite(gzf, comp_buf, (unsigned int)(comp_len > 0 ? (unsigned int)comp_len : 0U));
    int gz_close_ret = gzclose(gzf);
    uLong ad1 = adler32(0UL, src, (uInt)src_len);
    uLong ad2 = adler32(0UL, comp_buf, (uInt)(comp_len > 0 ? (uInt)comp_len : 0U));
    uLong combined = adler32_combine(ad1, ad2, (off_t)comp_len);
    uLong combined64 = adler32_combine64(ad1, ad2, (off64_t)comp2_len);

    // step 4: Validate & Cleanup
    int end_ret1 = deflateEnd(&strm);
    int end_ret2 = deflateEnd(&copy_strm);
    free(comp_buf);
    free(comp_buf2);
    (void)ver;
    (void)init_ret;
    (void)copy_ret;
    (void)bound;
    (void)def_ret1;
    (void)def_ret2;
    (void)comp_len;
    (void)comp2_len;
    (void)gz_wr;
    (void)gz_close_ret;
    (void)ad1;
    (void)ad2;
    (void)combined;
    (void)combined64;
    (void)end_ret1;
    (void)end_ret2;
    // API sequence test completed successfully
    return 66;
}