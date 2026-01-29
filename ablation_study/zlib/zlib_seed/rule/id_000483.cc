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
//<ID> 483
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers and compress input
    z_stream defStrm;
    memset(&defStrm, 0, sizeof(defStrm));
    z_stream inStrm;
    memset(&inStrm, 0, sizeof(inStrm));
    uLong inputLen = 256UL;
    Bytef * inputBuf = (Bytef *)malloc((size_t)inputLen);
    memset(inputBuf, 'A', (size_t)inputLen);
    uLong bound = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen_f = (uLongf)bound;
    compress2(compBuf, &compLen_f, (const Bytef *)inputBuf, inputLen, 6);

    // step 2: Initialize inflate and deflate streams (configure)
    inflateInit2_(&inStrm, 15, zlibVersion(), (int)sizeof(z_stream));
    deflateInit_(&defStrm, 6, zlibVersion(), (int)sizeof(z_stream));

    // step 3: Operate and validate: uncompress into output buffer and compute checksums
    Bytef * outBuf = (Bytef *)malloc((size_t)inputLen);
    memset(outBuf, 0, (size_t)inputLen);
    uLongf destLen = (uLongf)inputLen;
    uLong sourceLen = (uLong)compLen_f;
    uncompress2(outBuf, &destLen, (const Bytef *)compBuf, &sourceLen);
    uLong chk_in = adler32(0UL, (const Bytef *)inputBuf, (uInt)inputLen);
    uLong chk_out = adler32(0UL, (const Bytef *)outBuf, (uInt)destLen);
    (void)chk_in;
    (void)chk_out;

    // step 4: Cleanup & finalize
    deflateEnd(&defStrm);
    inflateEnd(&inStrm);
    free(inputBuf);
    free(compBuf);
    free(outBuf);
    return 66; // API sequence test completed successfully
}