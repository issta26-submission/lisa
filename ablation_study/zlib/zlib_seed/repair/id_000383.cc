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
//<ID> 383
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations / Initialize
    const char *ver = zlibVersion();
    Bytef input[] = "Test payload for zlib deflateBound and gzwrite integration.";
    uLong input_len = (uLong)(sizeof(input) - 1);
    z_stream src;
    z_stream dst;
    gzFile gz;
    unsigned char comp_buf[4096];
    uLong bound;
    int ret_deflate_init;
    int ret_deflate_copy;
    int ret_deflate_end_src;
    int ret_deflate_end_dst;
    int ret_gzwrite;
    int ret_gzclose;
    unsigned int write_len;

    // step 2: Setup / Configure
    memset(&src, 0, sizeof(src));
    memset(&dst, 0, sizeof(dst));
    memset(comp_buf, 0, sizeof(comp_buf));
    ret_deflate_init = deflateInit_(&src, 6, ver, (int)sizeof(z_stream));
    ret_deflate_copy = deflateCopy(&dst, &src);
    bound = deflateBound(&src, input_len);

    // step 3: Operate
    src.next_in = (Bytef *)input;
    src.avail_in = (uInt)input_len;
    src.next_out = comp_buf;
    src.avail_out = (uInt)sizeof(comp_buf);
    write_len = (unsigned int)((bound <= (uLong)sizeof(comp_buf)) ? bound : (uLong)sizeof(comp_buf));
    gz = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    ret_gzwrite = gzwrite(gz, (voidpc)comp_buf, write_len);
    ret_gzclose = gzclose(gz);

    // step 4: Validate / Cleanup
    ret_deflate_end_src = deflateEnd(&src);
    ret_deflate_end_dst = deflateEnd(&dst);
    (void)ver;
    (void)input_len;
    (void)bound;
    (void)ret_deflate_init;
    (void)ret_deflate_copy;
    (void)ret_deflate_end_src;
    (void)ret_deflate_end_dst;
    (void)ret_gzwrite;
    (void)ret_gzclose;
    (void)write_len;
    // API sequence test completed successfully
    return 66;
}