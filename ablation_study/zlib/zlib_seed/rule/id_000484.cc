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
//<ID> 484
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers and compress input
    z_stream infStrm;
    memset(&infStrm, 0, sizeof(infStrm));
    z_stream defStrm;
    memset(&defStrm, 0, sizeof(defStrm));
    unsigned char input[256];
    memset(input, 'A', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    compress2(compBuf, &compLen, (const Bytef *)input, inputLen, 6);

    // step 2: Initialize inflate and deflate streams
    inflateInit2_(&infStrm, 15, zlibVersion(), (int)sizeof(z_stream));
    deflateInit_(&defStrm, 6, zlibVersion(), (int)sizeof(z_stream));

    // step 3: Operate: uncompress2 using compressed data into output buffer
    Bytef * outBuf = (Bytef *)malloc((size_t)inputLen);
    memset(outBuf, 0, (size_t)inputLen);
    uLongf outLen = (uLongf)inputLen;
    uLong srcLen = (uLong)compLen;
    uncompress2(outBuf, &outLen, compBuf, &srcLen);

    // step 4: Validate & Cleanup
    inflateEnd(&infStrm);
    deflateEnd(&defStrm);
    free(compBuf);
    free(outBuf);
    return 66; // API sequence test completed successfully
}