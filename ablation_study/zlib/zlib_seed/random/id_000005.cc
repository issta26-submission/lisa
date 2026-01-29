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
//<ID> 5
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream defstrm;
    z_stream infstrm;
    const char in_data[] = "Hello zlib\n";
    uLong in_len = (uLong)(sizeof(in_data) - 1);
    gzFile gf;
    int rc_deflate_init;
    int rc_inflate_init;
    int gzret;
    memset(&defstrm, 0, sizeof(z_stream));
    memset(&infstrm, 0, sizeof(z_stream));

    // step 2: Setup - Initialize deflate stream
    rc_deflate_init = deflateInit_(&defstrm, 6, zlibVersion(), (int)sizeof(z_stream));

    // step 3: Operate - write a gz file using zlib version info and input data
    gf = gzopen("test_zlib_api_sequence_output.gz", "wb");
    gzret = gzprintf(gf, "zlib version: %s; sample: %s", zlibVersion(), in_data);
    gzclose(gf);

    // step 4: Validate & Cleanup - initialize inflate, check sync point, finish inflate backend and cleanup deflate
    rc_inflate_init = inflateInit_(&infstrm, zlibVersion(), (int)sizeof(z_stream));
    (void)inflateSyncPoint(&infstrm);
    (void)inflateBackEnd(&infstrm);
    inflateEnd(&infstrm);
    deflateEnd(&defstrm);

    // API sequence test completed successfully
    return 66;
}