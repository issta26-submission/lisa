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
//<ID> 554
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
    uLong inLen = 64UL;
    Bytef * inBuf = (Bytef *)malloc((size_t)inLen);
    memset(inBuf, 'A', (size_t)inLen);
    uLong outLen = 128UL;
    Bytef * outBuf = (Bytef *)malloc((size_t)outLen);
    memset(outBuf, 0, (size_t)outLen);

    // step 2: Initialize inflate
    inflateInit_(&strm, zlibVersion(), (int)sizeof(z_stream));

    // step 3: Configure, Operate & Validate
    strm.next_in = inBuf;
    strm.avail_in = (uInt)inLen;
    strm.next_out = outBuf;
    strm.avail_out = (uInt)outLen;
    inflate(&strm, 0);
    inflateSync(&strm);
    inflateBackEnd(&strm);

    // step 4: Cleanup
    inflateEnd(&strm);
    free(inBuf);
    free(outBuf);

    // API sequence test completed successfully
    return 66;
}