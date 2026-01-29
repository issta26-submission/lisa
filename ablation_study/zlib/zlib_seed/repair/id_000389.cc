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
//<ID> 389
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
    z_stream copy;
    memset(&src, 0, sizeof(src));
    memset(&copy, 0, sizeof(copy));
    Bytef input[] = "Zlib API sequence payload for deflateBound/deflateCopy/gzwrite test.";
    uLong input_len = (uLong)(sizeof(input) - 1);
    int init_ret_src;
    uLong bound;
    int copy_ret;
    gzFile gz_wr;
    int gzwrite_ret;
    int gzclose_ret;
    int end_ret_copy;
    int end_ret_src;

    // step 2: Setup / Configure
    init_ret_src = deflateInit_(&src, 6, ver, (int)sizeof(z_stream));
    bound = deflateBound(&src, input_len);

    // step 3: Operate
    copy_ret = deflateCopy(&copy, &src);
    gz_wr = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    gzwrite_ret = gzwrite(gz_wr, (voidpc)input, (unsigned int)input_len);
    gzclose_ret = gzclose(gz_wr);

    // step 4: Validate / Cleanup
    end_ret_copy = deflateEnd(&copy);
    end_ret_src = deflateEnd(&src);
    (void)ver;
    (void)init_ret_src;
    (void)bound;
    (void)copy_ret;
    (void)gzwrite_ret;
    (void)gzclose_ret;
    (void)end_ret_copy;
    (void)end_ret_src;
    // API sequence test completed successfully
    return 66;
}