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
//<ID> 438
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & initialize input and buffers
    z_stream defStrm;
    memset(&defStrm, 0, sizeof(defStrm));
    unsigned char input[512];
    memset(input, 'Z', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound1 = compressBound(inputLen);
    Bytef * compBuf1 = (Bytef *)malloc((size_t)bound1);
    memset(compBuf1, 0, (size_t)bound1);
    uLongf compLen1 = (uLongf)bound1;
    Bytef * compBuf2 = (Bytef *)malloc((size_t)bound1);
    memset(compBuf2, 0, (size_t)bound1);
    uLongf compLen2 = (uLongf)bound1;

    // step 2: Compress with compress and compress2
    compress(compBuf1, &compLen1, (const Bytef *)input, inputLen);
    compress2(compBuf2, &compLen2, (const Bytef *)input, inputLen, 9);

    // step 3: Initialize deflate stream, operate and query dictionary
    deflateInit_(&defStrm, 6, zlibVersion(), (int)sizeof(z_stream));
    defStrm.next_in = compBuf1;
    defStrm.avail_in = (uInt)compLen1;
    uLong outBound = compressBound((uLong)compLen1);
    Bytef * outBuf = (Bytef *)malloc((size_t)outBound);
    memset(outBuf, 0, (size_t)outBound);
    defStrm.next_out = outBuf;
    defStrm.avail_out = (uInt)outBound;
    deflate(&defStrm, 0);
    Bytef * dictBuf = (Bytef *)malloc(64);
    memset(dictBuf, 0, 64);
    uInt dictLen = 64;
    deflateGetDictionary(&defStrm, dictBuf, &dictLen);

    // step 4: Cleanup and finalize
    deflateEnd(&defStrm);
    free(compBuf1);
    free(compBuf2);
    free(outBuf);
    free(dictBuf);
    return 66;
    // API sequence test completed successfully
}