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
//<ID> 758
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    Bytef plain[128];
    memset(plain, 'Z', sizeof(plain));
    uLong plain_len = (uLong)sizeof(plain);

    z_stream def;
    memset(&def, 0, sizeof(def));
    int def_init_ret = deflateInit_(&def, 6, version, (int)sizeof(z_stream));
    def.next_in = plain;
    def.avail_in = (uInt)plain_len;

    z_stream def_copy;
    memset(&def_copy, 0, sizeof(def_copy));
    int def_copy_ret = deflateCopy(&def_copy, &def);
    int def_reset_ret = deflateReset(&def);

    // step 2: Configure
    uLong out_bound = compressBound(plain_len);
    uLongf dest_len = (uLongf)out_bound;
    Bytef *comp_buf = (Bytef *)malloc((size_t)out_bound);
    memset(comp_buf, 0, (size_t)out_bound);
    int comp_ret = compress(comp_buf, &dest_len, plain, plain_len);
    unsigned int comp_size = (unsigned int)dest_len;

    const char *fname = "test_zlib_api_sequence_tmp.gz";
    gzFile gzw = gzopen(fname, "wb");
    int gz_wr = gzwrite(gzw, comp_buf, comp_size);
    int gz_close_w = gzclose(gzw);

    // step 3: Operate
    gzFile gzr = gzopen(fname, "rb");
    Bytef *read_buf = (Bytef *)malloc((size_t)comp_size ? (size_t)comp_size : 1);
    memset(read_buf, 0, (size_t)(comp_size ? comp_size : 1));
    z_size_t items_read = gzfread(read_buf, 1, (z_size_t)comp_size, gzr);
    int gz_close_r = gzclose(gzr);

    // step 4: Validate / Cleanup
    int def_end_ret = deflateEnd(&def);
    int def_copy_end_ret = deflateEnd(&def_copy);
    free(comp_buf);
    free(read_buf);

    (void)version;
    (void)def_init_ret;
    (void)def_copy_ret;
    (void)def_reset_ret;
    (void)out_bound;
    (void)dest_len;
    (void)comp_ret;
    (void)comp_size;
    (void)gz_wr;
    (void)gz_close_w;
    (void)items_read;
    (void)gz_close_r;
    (void)def_end_ret;
    (void)def_copy_end_ret;
    // API sequence test completed successfully
    return 66;
}