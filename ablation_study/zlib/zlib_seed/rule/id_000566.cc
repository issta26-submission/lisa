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
//<ID> 566
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
    unsigned char input[192];
    memset(input, 'Z', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    Bytef * out1 = (Bytef *)malloc((size_t)inputLen);
    memset(out1, 0, (size_t)inputLen);
    Bytef * out2 = (Bytef *)malloc((size_t)inputLen);
    memset(out2, 0, (size_t)inputLen);

    // step 2: Compress the input into compBuf
    uLongf compLenF = (uLongf)bound;
    compress2(compBuf, &compLenF, (const Bytef *)input, inputLen, 6);

    // step 3: Decompress using uncompress and uncompress2 to validate both APIs
    uLongf out1Len = (uLongf)inputLen;
    uncompress(out1, &out1Len, compBuf, (uLong)compLenF);
    uLong compLenForUn2 = (uLong)compLenF;
    uLongf out2Len = (uLongf)inputLen;
    uncompress2(out2, &out2Len, compBuf, &compLenForUn2);

    // step 4: Initialize inflate stream, set a dictionary from the original input, then cleanup
    inflateInit_(&infStrm, zlibVersion(), (int)sizeof(z_stream));
    inflateSetDictionary(&infStrm, (const Bytef *)input, (uInt)8);
    inflateEnd(&infStrm);

    free(compBuf);
    free(out1);
    free(out2);

    // API sequence test completed successfully
    return 66;
}