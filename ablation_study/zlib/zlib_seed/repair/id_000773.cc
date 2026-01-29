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
//<ID> 773
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    Bytef plain[512];
    memset(plain, 'P', sizeof(plain));
    uLong plain_len = (uLong)sizeof(plain);

    uLong comp_bound = compressBound(plain_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_bound);
    memset(comp_buf, 0, (size_t)comp_bound);
    uLongf comp_len = (uLongf)comp_bound;
    int comp_ret = compress2(comp_buf, &comp_len, plain, plain_len, 6);

    // step 2: Configure
    z_stream src;
    memset(&src, 0, sizeof(src));
    int def_init_ret = deflateInit_(&src, 6, version, (int)sizeof(z_stream));
    src.next_in = comp_buf;
    src.avail_in = (uInt)comp_len;

    z_stream dest;
    memset(&dest, 0, sizeof(dest));
    int copy_ret = deflateCopy(&dest, &src);

    // step 3: Operate
    z_stream infback;
    memset(&infback, 0, sizeof(infback));
    unsigned char *window = (unsigned char *)malloc((size_t)32768);
    memset(window, 0, (size_t)32768);
    int infback_init_ret = inflateBackInit_(&infback, 15, window, version, (int)sizeof(z_stream));

    const char *fname = "test_zlib_api_sequence_tmp.gz";
    gzFile gzw = gzopen64(fname, "wb");
    int gz_wr = gzwrite(gzw, comp_buf, (unsigned int)comp_len);
    int gz_close_w = gzclose(gzw);

    // step 4: Validate / Cleanup
    int def_end_ret_src = deflateEnd(&src);
    int def_end_ret_dest = deflateEnd(&dest);
    int infback_end_ret = inflateBackEnd(&infback);
    free(comp_buf);
    free(window);

    (void)version;
    (void)plain_len;
    (void)comp_bound;
    (void)comp_ret;
    (void)comp_len;
    (void)def_init_ret;
    (void)copy_ret;
    (void)infback_init_ret;
    (void)gz_wr;
    (void)gz_close_w;
    (void)def_end_ret_src;
    (void)def_end_ret_dest;
    (void)infback_end_ret;
    // API sequence test completed successfully
    return 66;
}