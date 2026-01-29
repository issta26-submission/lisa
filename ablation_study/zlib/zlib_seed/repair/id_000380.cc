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
//<ID> 380
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations / Initialize
    const char *ver = zlibVersion();
    z_stream strm;
    z_stream strm_copy;
    Bytef src_buf[] = "Example payload to exercise deflateBound, deflateCopy and gzwrite in a single sequence.";
    uInt src_len = (uInt)(sizeof(src_buf) - 1);
    uLong bound;
    Bytef *comp_buf = NULL;
    gzFile gz_wr;
    int ret_deflate_init;
    int ret_deflate_copy;
    int ret_deflate_ret;
    int ret_deflate_end_copy;
    int ret_deflate_end_orig;
    int ret_gzwrite;
    int ret_gzclose;

    // step 2: Setup / Configure
    memset(&strm, 0, sizeof(strm));
    memset(&strm_copy, 0, sizeof(strm_copy));
    ret_deflate_init = deflateInit_(&strm, 6, ver, (int)sizeof(z_stream));
    bound = deflateBound(&strm, (uLong)src_len);
    comp_buf = new Bytef[(size_t)bound];
    strm.next_in = src_buf;
    strm.avail_in = src_len;
    strm.next_out = comp_buf;
    strm.avail_out = (uInt)bound;

    // step 3: Operate
    ret_deflate_copy = deflateCopy(&strm_copy, &strm);
    gz_wr = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    ret_gzwrite = gzwrite(gz_wr, (voidpc)src_buf, (unsigned int)src_len);
    ret_gzclose = gzclose(gz_wr);
    ret_deflate_ret = deflate(&strm, 4); // 4 == Z_FINISH (using numeric constant to avoid additional macros)

    // step 4: Validate / Cleanup
    ret_deflate_end_copy = deflateEnd(&strm_copy);
    ret_deflate_end_orig = deflateEnd(&strm);
    delete [] comp_buf;
    (void)ver;
    (void)bound;
    (void)ret_deflate_init;
    (void)ret_deflate_copy;
    (void)ret_deflate_ret;
    (void)ret_deflate_end_copy;
    (void)ret_deflate_end_orig;
    (void)ret_gzwrite;
    (void)ret_gzclose;
    // API sequence test completed successfully
    return 66;
}