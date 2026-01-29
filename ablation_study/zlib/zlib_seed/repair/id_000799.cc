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
//<ID> 799
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    Bytef plain[256];
    memset(plain, 'Z', sizeof(plain));
    uLong plain_len = (uLong)sizeof(plain);

    z_stream def;
    memset(&def, 0, sizeof(def));
    int init_ret = deflateInit_(&def, 6, version, (int)sizeof(z_stream));
    def.next_in = plain;
    def.avail_in = (uInt)plain_len;
    uLong out_bound = deflateBound(&def, plain_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)out_bound);
    memset(comp_buf, 0, (size_t)out_bound);
    def.next_out = comp_buf;
    def.avail_out = (uInt)out_bound;

    // step 2: Configure
    int def_ret = deflate(&def, 4);
    uLong comp_len = def.total_out;
    unsigned long codes_used = inflateCodesUsed(&def);

    // step 3: Operate
    const char *fname = "test_zlib_api_sequence_tmp.gz";
    gzFile gzf_w = gzopen64(fname, "wb");
    int gz_wr = gzwrite(gzf_w, comp_buf, (unsigned int)comp_len);
    int gz_close_w = gzclose(gzf_w);
    gzFile gzf_r = gzopen(fname, "rb");
    int ch = gzgetc_(gzf_r);
    int gz_close_r = gzclose(gzf_r);

    // step 4: Validate / Cleanup
    int def_end = deflateEnd(&def);
    free(comp_buf);
    (void)version;
    (void)init_ret;
    (void)plain_len;
    (void)out_bound;
    (void)def_ret;
    (void)comp_len;
    (void)codes_used;
    (void)fname;
    (void)gz_wr;
    (void)gz_close_w;
    (void)ch;
    (void)gz_close_r;
    (void)def_end;
    // API sequence test completed successfully
    return 66;
}