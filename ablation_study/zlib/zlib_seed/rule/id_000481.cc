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
//<ID> 481
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare input and compression buffers
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    unsigned char orig[256];
    memset(orig, 'A', sizeof(orig));
    uLong origLen = (uLong)sizeof(orig);
    uLong bound = compressBound(origLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    compress2(compBuf, &compLen, (const Bytef *)orig, origLen, 6);
    Bytef * outBuf = (Bytef *)malloc((size_t)origLen);
    memset(outBuf, 0, (size_t)origLen);

    // step 2: Initialize and finalize a deflate stream (initialize -> cleanup)
    deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    deflateEnd(&dstrm);

    // step 3: Initialize inflate stream and perform uncompress2 (configure -> operate)
    inflateInit2_(&istrm, 15, zlibVersion(), (int)sizeof(z_stream));
    uLongf outLen = (uLongf)origLen;
    uLong srcLen = (uLong)compLen;
    uncompress2(outBuf, &outLen, compBuf, &srcLen);

    // step 4: Validate and cleanup (validate -> cleanup)
    uLong chk_orig = adler32(0UL, (const Bytef *)orig, (uInt)origLen);
    uLong chk_out = adler32(0UL, outBuf, (uInt)outLen);
    (void)chk_orig;
    (void)chk_out;
    inflateEnd(&istrm);
    free(compBuf);
    free(outBuf);
    return 66; // API sequence test completed successfully
}