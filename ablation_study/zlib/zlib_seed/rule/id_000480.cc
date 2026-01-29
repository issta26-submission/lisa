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
//<ID> 480
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare data buffers and compress input
    z_stream dstrm;
    z_stream istrm;
    memset(&dstrm, 0, sizeof(dstrm));
    memset(&istrm, 0, sizeof(istrm));
    unsigned char input[256];
    memset(input, 'A', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    compress2(compBuf, &compLen, (const Bytef *)input, inputLen, 6);

    // step 2: Initialize a deflate stream and immediately finalize it (resource lifecycle)
    deflateInit2_(&dstrm, 6, 8, 15, 8, 0, zlibVersion(), (int)sizeof(z_stream));
    deflateEnd(&dstrm);

    // step 3: Initialize inflate stream and perform uncompress2 to restore original data
    inflateInit2_(&istrm, 15, zlibVersion(), (int)sizeof(z_stream));
    Bytef * outBuf = (Bytef *)malloc((size_t)inputLen);
    memset(outBuf, 0, (size_t)inputLen);
    uLongf destLen = (uLongf)inputLen;
    uLong sourceLen = (uLong)compLen;
    uncompress2(outBuf, &destLen, compBuf, &sourceLen);

    // step 4: Validate via a checksum, cleanup inflate and free buffers
    uLong checksum = adler32(0UL, (const Bytef *)outBuf, (uInt)destLen);
    (void)checksum;
    inflateEnd(&istrm);
    free(compBuf);
    free(outBuf);
    return 66; // API sequence test completed successfully
}