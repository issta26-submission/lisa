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
//<ID> 527
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char src[] = "zlib test payload for gzwrite and gzungetc";
    const unsigned int src_len = (unsigned int)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong bound = compressBound((uLong)src_len);
    char *buf = (char *)malloc((size_t)bound);
    memset(buf, 0, (size_t)bound);
    memcpy(buf, src, src_len);

    // step 2: Configure
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");

    // step 3: Operate -> Validate
    int rc_write = gzwrite(gf, (voidpc)buf, src_len);
    int rc_rewind = gzrewind(gf);
    int rc_ungetc = gzungetc((int)(unsigned char)buf[0], gf);

    // step 4: Cleanup
    int rc_close = gzclose(gf);
    free(buf);
    (void)version;
    (void)bound;
    (void)rc_write;
    (void)rc_rewind;
    (void)rc_ungetc;
    (void)rc_close;
    // API sequence test completed successfully
    return 66;
}