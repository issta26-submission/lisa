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
//<ID> 1862
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
    uLong ad_src = adler32(0UL, src, (uInt)src_len);
    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    const char *ver = zlibVersion();
    int init_ret = deflateInit_(&strm, 6, ver, (int)sizeof(z_stream));

    // step 2: Configure compressor and buffers
    strm.next_in = (Bytef *)src;
    strm.avail_in = (uInt)src_len;
    uLong bound = deflateBound(&strm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    strm.next_out = comp_buf;
    strm.avail_out = (uInt)bound;

    // step 3: Operate - deflate, copy stream, write compressed data, combine checksums
    int def_ret = deflate(&strm, 4);
    uLong comp_len = strm.total_out;
    z_stream copy_strm;
    int copy_ret = deflateCopy(&copy_strm, &strm);
    gzFile gzf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gz_wr = gzwrite(gzf, comp_buf, (unsigned int)comp_len);
    uLong ad_comp = adler32(0UL, comp_buf, (uInt)comp_len);
    off_t off_len = (off_t)src_len;
    off64_t off64_len = (off64_t)src_len;
    uLong combined = adler32_combine(ad_src, ad_comp, off_len);
    uLong combined64 = adler32_combine64(ad_src, ad_comp, off64_len);

    // step 4: Validate & Cleanup
    int end_ret = deflateEnd(&strm);
    int end_copy_ret = deflateEnd(&copy_strm);
    int gz_close = gzclose(gzf);
    free(comp_buf);
    (void)ver;
    (void)init_ret;
    (void)bound;
    (void)def_ret;
    (void)comp_len;
    (void)copy_ret;
    (void)gz_wr;
    (void)ad_src;
    (void)ad_comp;
    (void)combined;
    (void)combined64;
    (void)end_ret;
    (void)end_copy_ret;
    (void)gz_close;
    // API sequence test completed successfully
    return 66;
}