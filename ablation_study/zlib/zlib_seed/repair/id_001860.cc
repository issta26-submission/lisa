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
//<ID> 1860
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
    uLong bound = deflateBound(&strm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    strm.next_in = (Bytef *)src;
    strm.avail_in = (uInt)src_len;
    strm.next_out = comp_buf;
    strm.avail_out = (uInt)bound;

    // step 2: Configure & Operate - compress and create a copy of the stream state
    int def_ret = deflate(&strm, 4);
    uLong comp_len = strm.total_out;
    z_stream copy_strm;
    memset(&copy_strm, 0, (int)sizeof(copy_strm));
    int copy_ret = deflateCopy(&copy_strm, &strm);
    Bytef *copy_out_buf = (Bytef *)malloc((size_t)bound);
    memset(copy_out_buf, 0, (int)bound);
    copy_strm.next_out = copy_out_buf;
    copy_strm.avail_out = (uInt)bound;
    int def_copy_ret = deflate(&copy_strm, 4);
    uLong copy_out_len = copy_strm.total_out;

    // step 3: Write compressed data via gzwrite and compute combined adler checksums
    gzFile gzf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gz_wr_1 = gzwrite(gzf, comp_buf, (unsigned int)(comp_len));
    int gz_wr_2 = gzwrite(gzf, copy_out_buf, (unsigned int)(copy_out_len));
    int gz_close_ret = gzclose(gzf);
    uLong ad1 = adler32(0UL, src, (uInt)src_len);
    uLong ad2 = adler32(0UL, comp_buf, (uInt)(comp_len));
    uLong combined = adler32_combine(ad1, ad2, (off_t)comp_len);
    uLong combined64 = adler32_combine64(ad1, ad2, (off64_t)comp_len);

    // step 4: Cleanup & Validate
    int end_ret_src = deflateEnd(&strm);
    int end_ret_copy = deflateEnd(&copy_strm);
    free(comp_buf);
    free(copy_out_buf);
    (void)ver;
    (void)init_ret;
    (void)bound;
    (void)def_ret;
    (void)comp_len;
    (void)copy_ret;
    (void)def_copy_ret;
    (void)copy_out_len;
    (void)gz_wr_1;
    (void)gz_wr_2;
    (void)gz_close_ret;
    (void)ad1;
    (void)ad2;
    (void)combined;
    (void)combined64;
    (void)end_ret_src;
    (void)end_ret_copy;
    // API sequence test completed successfully
    return 66;
}