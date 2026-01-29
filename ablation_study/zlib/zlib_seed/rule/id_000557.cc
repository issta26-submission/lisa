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
//<ID> 557
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    Bytef * inBuf = (Bytef *)malloc((size_t)64);
    memset(inBuf, 0x5A, (size_t)64);
    uInt inLen = (uInt)64;
    Bytef * outBuf = (Bytef *)malloc((size_t)128);
    memset(outBuf, 0, (size_t)128);
    uInt outLen = (uInt)128;

    // step 2: Initialize inflate with zlib version info
    inflateInit_(&strm, zlibVersion(), (int)sizeof(z_stream));

    // step 3: Configure stream and perform operations (sync and back-end)
    strm.next_in = inBuf;
    strm.avail_in = inLen;
    strm.next_out = outBuf;
    strm.avail_out = outLen;
    inflateSync(&strm);
    inflateBackEnd(&strm);

    // step 4: Cleanup
    inflateEnd(&strm);
    free(inBuf);
    free(outBuf);

    // API sequence test completed successfully
    return 66;
}