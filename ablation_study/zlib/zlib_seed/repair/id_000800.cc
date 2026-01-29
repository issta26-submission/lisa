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
//<ID> 800
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    int inf_init_ret = inflateInit_(&inf, version, (int)sizeof(z_stream));

    // step 2: Configure
    inf.next_in = (Bytef *)NULL;
    inf.avail_in = 0;
    unsigned long codes_used = inflateCodesUsed(&inf);

    // step 3: Operate
    gzFile gzf = gzdopen(1, "wb");
    int gz_puts_ret = gzputs(gzf, "zlib gzdopen/gzputs test\n");
    int gz_close_ret = gzclose(gzf);

    // step 4: Validate / Cleanup
    int inf_end_ret = inflateEnd(&inf);
    (void)version;
    (void)inf_init_ret;
    (void)codes_used;
    (void)gz_puts_ret;
    (void)gz_close_ret;
    (void)inf_end_ret;
    // API sequence test completed successfully
    return 66;
}