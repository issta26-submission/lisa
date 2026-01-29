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
//<ID> 553
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
    unsigned char input[64];
    memset(input, 'A', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    Bytef outBuf[128];
    memset(outBuf, 0, sizeof(outBuf));
    strm.next_in = (Bytef *)input;
    strm.avail_in = (uInt)inputLen;
    strm.next_out = outBuf;
    strm.avail_out = (uInt)sizeof(outBuf);

    // step 2: Initialize inflater with library version
    const char * ver = zlibVersion();
    inflateInit_(&strm, ver, (int)sizeof(z_stream));

    // step 3: Operate & simple validation (sync and back-end teardown)
    inflateSync(&strm);
    int back_result = inflateBackEnd(&strm);
    (void)back_result;

    // step 4: Cleanup
    inflateEnd(&strm);

    return 66; // API sequence test completed successfully
}