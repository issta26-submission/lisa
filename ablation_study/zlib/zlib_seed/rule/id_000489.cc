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
//<ID> 489
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers (create sample data and compress it)
    z_stream infStrm;
    z_stream defStrm;
    memset(&infStrm, 0, sizeof(infStrm));
    memset(&defStrm, 0, sizeof(defStrm));
    unsigned char input[128];
    memset(input, 'A', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    compress2(compBuf, &compLen, (const Bytef *)input, inputLen, 6);
    Bytef * outBuf = (Bytef *)malloc((size_t)inputLen);
    memset(outBuf, 0, (size_t)inputLen);
    uLongf outLen = (uLongf)inputLen;
    uLong compLenU = (uLong)compLen;

    // step 2: Initialize inflate stream (configuration)
    inflateInit2_(&infStrm, 15, zlibVersion(), (int)sizeof(z_stream));

    // step 3: Operate: uncompress the previously produced compressed data and compute checksum (validation)
    uncompress2(outBuf, &outLen, compBuf, &compLenU);
    uLong checksum = adler32(0UL, (const Bytef *)outBuf, (uInt)inputLen);
    (void)checksum;

    // step 4: Cleanup: initialize a deflate stream and then properly end/cleanup both streams and buffers
    deflateInit_(&defStrm, 6, zlibVersion(), (int)sizeof(z_stream));
    deflateEnd(&defStrm);
    inflateEnd(&infStrm);
    free(compBuf);
    free(outBuf);

    return 66; // API sequence test completed successfully
}