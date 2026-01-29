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
//<ID> 563
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
    unsigned char input[128];
    memset(input, 'Z', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compSize = (uLongf)bound;
    compress2(compBuf, &compSize, (const Bytef *)input, inputLen, 6);

    // step 2: Decompress using uncompress (simple path)
    Bytef * outBuf = (Bytef *)malloc((size_t)inputLen);
    memset(outBuf, 0, (size_t)inputLen);
    uLongf destLen1 = (uLongf)inputLen;
    uncompress(outBuf, &destLen1, compBuf, (uLong)compSize);

    // step 3: Decompress using uncompress2 (source length pointer path)
    Bytef * outBuf2 = (Bytef *)malloc((size_t)inputLen);
    memset(outBuf2, 0, (size_t)inputLen);
    uLongf destLen2 = (uLongf)inputLen;
    uLong sourceLen = (uLong)compSize;
    uncompress2(outBuf2, &destLen2, compBuf, &sourceLen);

    // step 4: Initialize inflate stream, set dictionary, and cleanup
    const char * ver = zlibVersion();
    inflateInit_(&infStrm, ver, (int)sizeof(z_stream));
    inflateSetDictionary(&infStrm, (const Bytef *)input, (uInt)8);
    inflateEnd(&infStrm);

    free(compBuf);
    free(outBuf);
    free(outBuf2);

    // API sequence test completed successfully
    return 66;
}