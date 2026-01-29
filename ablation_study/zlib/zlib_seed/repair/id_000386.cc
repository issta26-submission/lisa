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
//<ID> 386
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations / Initialize
    const char *ver = zlibVersion();
    z_stream src;
    z_stream dst;
    memset(&src, 0, sizeof(src));
    memset(&dst, 0, sizeof(dst));
    Bytef write_buf[] = "Zlib API sequence: using deflateInit_, deflateCopy, deflateBound, deflateEnd and gzwrite.";
    unsigned int write_len = (unsigned int)(sizeof(write_buf) - 1);
    gzFile gz_wr = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    unsigned char comp_buf[2048];
    uLong bound = 0;
    int ret_deflate_init = 0;
    int ret_deflate_copy = 0;
    int ret_deflate_end_src = 0;
    int ret_deflate_end_dst = 0;
    int ret_gzwrite = 0;
    int ret_gzclose = 0;

    // step 2: Setup / Configure
    ret_deflate_init = deflateInit_(&src, 6, ver, (int)sizeof(z_stream));
    ret_deflate_copy = deflateCopy(&dst, &src);
    bound = deflateBound(&src, (uLong)write_len);
    comp_buf[0] = (unsigned char)(bound & 0xFF);
    comp_buf[1] = (unsigned char)((bound >> 8) & 0xFF);

    // step 3: Core operations
    ret_gzwrite = gzwrite(gz_wr, (voidpc)write_buf, write_len);
    ret_gzclose = gzclose(gz_wr);
    ret_deflate_end_src = deflateEnd(&src);
    ret_deflate_end_dst = deflateEnd(&dst);

    // step 4: Validate / Cleanup
    (void)ver;
    (void)comp_buf;
    (void)bound;
    (void)ret_deflate_init;
    (void)ret_deflate_copy;
    (void)ret_deflate_end_src;
    (void)ret_deflate_end_dst;
    (void)ret_gzwrite;
    (void)ret_gzclose;
    // API sequence test completed successfully
    return 66;
}