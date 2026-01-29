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
//<ID> 488
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare input and compressed buffer
    z_stream defStrm;
    memset(&defStrm, 0, sizeof(defStrm));
    z_stream infStrm;
    memset(&infStrm, 0, sizeof(infStrm));
    Bytef plain[128];
    memset(plain, 'Z', sizeof(plain));
    uLong plainLen = (uLong)sizeof(plain);
    uLong bound = compressBound(plainLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    compress2(compBuf, &compLen, (const Bytef *)plain, plainLen, 6);

    // step 2: Initialize deflate (for cleanup demo) and initialize inflate with zlibVersion
    deflateInit_(&defStrm, 6, zlibVersion(), (int)sizeof(z_stream));
    deflateEnd(&defStrm);
    inflateInit2_(&infStrm, 15, zlibVersion(), (int)sizeof(z_stream));

    // step 3: Operate: uncompress2 into output buffer and validate via checksum
    Bytef * outBuf = (Bytef *)malloc((size_t)plainLen);
    memset(outBuf, 0, (size_t)plainLen);
    uLongf outLen = (uLongf)plainLen;
    uLong srcLen = (uLong)compLen;
    uncompress2(outBuf, &outLen, (const Bytef *)compBuf, &srcLen);
    uLong checksum = adler32(0UL, outBuf, (uInt)outLen);
    (void)checksum;

    // step 4: Cleanup
    inflateEnd(&infStrm);
    free(compBuf);
    free(outBuf);
    return 66; // API sequence test completed successfully
}