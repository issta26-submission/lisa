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
//<ID> 387
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations / Initialize
    const char *ver = zlibVersion();
    z_stream src_strm;
    z_stream copy_strm;
    Bytef input_buf[] = "zlib api sequence sample payload";
    uInt input_len = (uInt)(sizeof(input_buf) - 1);
    Bytef scratch_out[512];
    uLong bound;
    gzFile gz_wr;
    int ret_deflate_copy;
    int ret_deflate_end_src;
    int ret_deflate_end_copy;
    int ret_gzwrite;
    int ret_gzclose;

    // step 2: Setup / Configure
    memset(&src_strm, 0, sizeof(src_strm));
    memset(&copy_strm, 0, sizeof(copy_strm));
    memset(scratch_out, 0, sizeof(scratch_out));
    deflateInit_(&src_strm, 6, ver, (int)sizeof(z_stream));
    bound = deflateBound(&src_strm, (uLong)input_len);
    scratch_out[0] = (Bytef)(bound & 0xFF);

    // step 3: Operate
    ret_deflate_copy = deflateCopy(&copy_strm, &src_strm);
    gz_wr = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    ret_gzwrite = gzwrite(gz_wr, (voidpc)input_buf, (unsigned int)input_len);
    ret_gzclose = gzclose(gz_wr);
    ret_deflate_end_src = deflateEnd(&src_strm);
    ret_deflate_end_copy = deflateEnd(&copy_strm);

    // step 4: Validate / Cleanup
    (void)ver;
    (void)input_len;
    (void)bound;
    (void)scratch_out;
    (void)ret_deflate_copy;
    (void)ret_deflate_end_src;
    (void)ret_deflate_end_copy;
    (void)ret_gzwrite;
    (void)ret_gzclose;
    // API sequence test completed successfully
    return 66;
}