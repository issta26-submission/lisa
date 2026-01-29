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
//<ID> 487
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare input and compression buffers
    z_stream defStrm;
    memset(&defStrm, 0, sizeof(defStrm));
    unsigned char input[128];
    memset(input, 'A', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;

    // step 2: Initialize deflate briefly and initialize inflate stream
    deflateInit_(&defStrm, 6, zlibVersion(), (int)sizeof(z_stream));
    z_stream infStrm;
    memset(&infStrm, 0, sizeof(infStrm));
    inflateInit2_(&infStrm, 15, zlibVersion(), (int)sizeof(z_stream));

    // step 3: Compress then uncompress using uncompress2 and compute checksums
    compress2(compBuf, &compLen, (const Bytef *)input, inputLen, 6);
    Bytef * outBuf = (Bytef *)malloc((size_t)inputLen);
    memset(outBuf, 0, (size_t)inputLen);
    uLong compLenUL = (uLong)compLen;
    uLongf outLen = (uLongf)inputLen;
    uncompress2(outBuf, &outLen, compBuf, &compLenUL);
    uLong checksum_orig = adler32(0UL, (const Bytef *)input, (uInt)inputLen);
    uLong checksum_out = adler32(0UL, outBuf, (uInt)outLen);
    (void)checksum_orig;
    (void)checksum_out;

    // step 4: Finalize streams and cleanup
    deflateEnd(&defStrm);
    inflateEnd(&infStrm);
    free(compBuf);
    free(outBuf);
    return 66; // API sequence test completed successfully
}