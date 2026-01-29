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
//<ID> 9
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    const char * ver = zlibVersion();
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_init;
    int rc_sync;
    int rc_back;
    // step 2: Initialize -> Configure
    rc_init = deflateInit_(&strm, 6, ver, (int)sizeof(z_stream));
    // step 3: Operate -> Validate
    rc_sync = inflateSyncPoint(&strm);
    rc_back = inflateBackEnd(&strm);
    gzprintf(gz, "zlibVersion=%s init=%d sync=%d back=%d\n", ver, rc_init, rc_sync, rc_back);
    // step 4: Cleanup
    deflateEnd(&strm);
    gzclose(gz);
    // API sequence test completed successfully
    return 66;
}