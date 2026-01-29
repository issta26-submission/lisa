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
//<ID> 524
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    z_stream infStrm;
    memset(&infStrm, 0, sizeof(infStrm));
    unsigned char src[256];
    memset(src, 'A', sizeof(src));
    uLong srcLen = (uLong)sizeof(src);
    uLong bound = compressBound(srcLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    Bytef * outBuf = (Bytef *)malloc((size_t)srcLen);
    memset(outBuf, 0, (size_t)srcLen);

    // step 2: Compress source
    compress(compBuf, &compLen, (const Bytef *)src, srcLen);

    // step 3: Initialize & configure inflate stream, then perform inflate
    inflateInit_(&infStrm, zlibVersion(), (int)sizeof(z_stream));
    inflateReset2(&infStrm, 15);
    infStrm.next_in = compBuf;
    infStrm.avail_in = (uInt)compLen;
    infStrm.next_out = outBuf;
    infStrm.avail_out = (uInt)srcLen;
    inflate(&infStrm, Z_FINISH);

    // step 4: Cleanup
    inflateEnd(&infStrm);
    free(compBuf);
    free(outBuf);

    return 66; // API sequence test completed successfully
}