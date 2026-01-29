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
//<ID> 385
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
    Bytef input[] = "Sample payload for deflateBound, deflateCopy and gzwrite demonstration.";
    unsigned int input_len = (unsigned int)(sizeof(input) - 1);
    unsigned char comp_buf[512];
    memset(comp_buf, 0, sizeof(comp_buf));
    gzFile gz_wr;
    int ret_deflate_init;
    uLong bound;
    int ret_deflate_copy;
    int ret_deflate_end_src;
    int ret_deflate_end_dst;
    int ret_gzwrite;
    int ret_gzclose;

    // step 2: Setup / Configure
    ret_deflate_init = deflateInit_(&src, 6, ver, (int)sizeof(z_stream));
    bound = deflateBound(&src, (uLong)input_len);
    comp_buf[0] = (unsigned char)(bound & 0xFF);

    // step 3: Core operations - copy deflate state, write with gzwrite, and cleanup deflate
    ret_deflate_copy = deflateCopy(&dst, &src);
    gz_wr = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    ret_gzwrite = gzwrite(gz_wr, (voidpc)input, input_len);
    ret_gzclose = gzclose(gz_wr);
    ret_deflate_end_src = deflateEnd(&src);
    ret_deflate_end_dst = deflateEnd(&dst);

    // step 4: Validate / Cleanup
    (void)ver;
    (void)input_len;
    (void)comp_buf;
    (void)ret_deflate_init;
    (void)bound;
    (void)ret_deflate_copy;
    (void)ret_gzwrite;
    (void)ret_gzclose;
    (void)ret_deflate_end_src;
    (void)ret_deflate_end_dst;
    // API sequence test completed successfully
    return 66;
}