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
//<ID> 1281
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef payload[] = "zlib API sequence payload: write via gzwrite, seek via gzseek, query inflate usage";
    uLong payload_len = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();
    z_stream infs;
    memset(&infs, 0, (int)sizeof(infs));
    int inf_init_ret = inflateInit_(&infs, version, (int)sizeof(z_stream));

    // step 2: Configure and Operate (write compressed container using gz)
    gzFile gzf_w = gzopen("test_zlib_api_sequence.gz", "wb");
    int gz_write_ret = gzwrite(gzf_w, payload, (unsigned int)(payload_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)payload_len));
    off_t pos_after_write = gzseek(gzf_w, (off_t)0, 0);
    int gz_close_w_ret = gzclose_w(gzf_w);

    // step 3: Operate and Validate (open for read, seek, and query inflate internals)
    gzFile gzf_r = gzopen("test_zlib_api_sequence.gz", "rb");
    off_t pos_read_start = gzseek(gzf_r, (off_t)0, 0);
    unsigned long codes_used = inflateCodesUsed(&infs);
    int gz_close_r_ret = gzclose_r(gzf_r);

    // step 4: Cleanup and finalization
    int inf_end_ret = inflateEnd(&infs);
    (void)version;
    (void)inf_init_ret;
    (void)gz_write_ret;
    (void)pos_after_write;
    (void)gz_close_w_ret;
    (void)pos_read_start;
    (void)codes_used;
    (void)gz_close_r_ret;
    (void)inf_end_ret;
    // API sequence test completed successfully
    return 66;
}