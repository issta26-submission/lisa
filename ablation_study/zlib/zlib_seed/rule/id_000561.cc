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
//<ID> 561
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
    unsigned char input[128];
    memset(input, 'A', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLenF = (uLongf)bound;
    Bytef *outBuf = (Bytef *)malloc((size_t)inputLen);
    memset(outBuf, 0, (size_t)inputLen);
    Bytef *outBuf2 = (Bytef *)malloc((size_t)inputLen);
    memset(outBuf2, 0, (size_t)inputLen);

    // step 2: Compress original data to produce a zlib stream
    compress2(compBuf, &compLenF, (const Bytef *)input, inputLen, 6);
    uLong compLen = (uLong)compLenF;

    // step 3: Decompress using uncompress (simple path)
    uLongf destLen1 = (uLongf)inputLen;
    uncompress(outBuf, &destLen1, compBuf, compLen);

    // step 4: Alternate decompression using uncompress2 and dictionary setup via inflateSetDictionary
    uLong compLenCopy = compLen;
    uLongf destLen2 = (uLongf)inputLen;
    uncompress2(outBuf2, &destLen2, compBuf, &compLenCopy);
    inflateInit_(&strm, zlibVersion(), (int)sizeof(z_stream));
    inflateSetDictionary(&strm, (const Bytef *)input, (uInt)8);
    inflateEnd(&strm);

    free(compBuf);
    free(outBuf);
    free(outBuf2);

    // API sequence test completed successfully
    return 66;
}