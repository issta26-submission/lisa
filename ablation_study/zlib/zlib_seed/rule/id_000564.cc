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
//<ID> 564
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
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    Bytef *outBuf1 = (Bytef *)malloc((size_t)inputLen);
    memset(outBuf1, 0, (size_t)inputLen);
    Bytef *outBuf2 = (Bytef *)malloc((size_t)inputLen);
    memset(outBuf2, 0, (size_t)inputLen);

    // step 2: Compress original data
    uLongf compLen_f = (uLongf)bound;
    compress(compBuf, &compLen_f, (const Bytef *)input, inputLen);
    uLong compLen = (uLong)compLen_f;

    // step 3: Initialize inflate stream and set dictionary using part of the original data
    inflateInit_(&infStrm, zlibVersion(), (int)sizeof(z_stream));
    inflateSetDictionary(&infStrm, (const Bytef *)input, (uInt)8);

    // step 4: Decompress using uncompress2 and uncompress, then cleanup
    uLongf destLen1 = (uLongf)inputLen;
    uLong srcLen1 = compLen;
    uncompress2(outBuf1, &destLen1, compBuf, &srcLen1);
    uLongf destLen2 = (uLongf)inputLen;
    uncompress(outBuf2, &destLen2, compBuf, (uLong)compLen);
    inflateEnd(&infStrm);
    free(compBuf);
    free(outBuf1);
    free(outBuf2);

    // API sequence test completed successfully
    return 66;
}