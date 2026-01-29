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
//<ID> 562
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
    memset(input, 'Z', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    Bytef * outBuf1 = (Bytef *)malloc((size_t)inputLen);
    memset(outBuf1, 0, (size_t)inputLen);
    Bytef * outBuf2 = (Bytef *)malloc((size_t)inputLen);
    memset(outBuf2, 0, (size_t)inputLen);

    // step 2: Initialize & compress (one-shot) to produce source for decompression
    compress2(compBuf, &compLen, (const Bytef *)input, inputLen, 6);

    // step 3: Operate - uncompress with uncompress, then uncompress2, then configure stream dictionary and get version
    uLongf destLen1 = (uLongf)inputLen;
    uncompress(outBuf1, &destLen1, compBuf, (uLong)compLen);
    uLongf destLen2 = (uLongf)inputLen;
    uLong srcLenPtr = (uLong)compLen;
    uncompress2(outBuf2, &destLen2, compBuf, &srcLenPtr);
    const char * ver = zlibVersion();
    (void)ver;
    inflateSetDictionary(&strm, (const Bytef *)input, (uInt)8);

    // step 4: Cleanup
    free(compBuf);
    free(outBuf1);
    free(outBuf2);

    // API sequence test completed successfully
    return 66;
}