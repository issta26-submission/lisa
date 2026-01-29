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
//<ID> 567
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    unsigned char orig[128];
    memset(orig, 'A', sizeof(orig));
    uLong origLen = (uLong)sizeof(orig);
    uLong bound = compressBound(origLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    compress2(compBuf, &compLen, (const Bytef *)orig, origLen, 6);
    uLong compSourceLen = (uLong)compLen;
    Bytef *dest1 = (Bytef *)malloc((size_t)origLen);
    memset(dest1, 0, (size_t)origLen);
    uLongf dest1Len = (uLongf)origLen;
    Bytef *dest2 = (Bytef *)malloc((size_t)origLen);
    memset(dest2, 0, (size_t)origLen);
    uLongf dest2Len = (uLongf)origLen;

    // step 2: Decompress using uncompress (simple path)
    uncompress(dest1, &dest1Len, compBuf, compSourceLen);

    // step 3: Decompress using uncompress2 (consumes source length)
    uLong srcLenVar = compSourceLen;
    uncompress2(dest2, &dest2Len, compBuf, &srcLenVar);

    // step 4: Initialize inflate stream, set dictionary, then cleanup
    z_stream infStrm;
    memset(&infStrm, 0, sizeof(infStrm));
    inflateInit_(&infStrm, ver, (int)sizeof(z_stream));
    inflateSetDictionary(&infStrm, (const Bytef *)orig, (uInt)8);
    inflateEnd(&infStrm);

    free(compBuf);
    free(dest1);
    free(dest2);

    // API sequence test completed successfully
    return 66;
}