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
//<ID> 486
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare original data and buffers
    z_stream dstrm;
    z_stream istrm;
    memset(&dstrm, 0, sizeof(dstrm));
    memset(&istrm, 0, sizeof(istrm));
    unsigned char original[256];
    memset(original, 'A', sizeof(original));
    uLong originalLen = (uLong)sizeof(original);
    uLong bound = compressBound(originalLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compBufLen = (uLongf)bound;
    Bytef * decompBuf = (Bytef *)malloc((size_t)originalLen);
    memset(decompBuf, 0, (size_t)originalLen);
    uLongf decompLen = (uLongf)originalLen;

    // step 2: Compress original into compBuf using compress2
    compress2(compBuf, &compBufLen, (const Bytef *)original, originalLen, 6);

    // step 3: Initialize inflate stream, then uncompress2 to restore data
    inflateInit2_(&istrm, 15, zlibVersion(), (int)sizeof(z_stream));
    uLong sourceLenForUncompress = (uLong)compBufLen;
    uncompress2(decompBuf, &decompLen, compBuf, &sourceLenForUncompress);
    inflateEnd(&istrm);

    // step 4: Initialize a deflate stream and finalize it, cleanup buffers
    deflateInit2_(&dstrm, 6, 8, 15, 8, 0, zlibVersion(), (int)sizeof(z_stream));
    deflateEnd(&dstrm);
    free(compBuf);
    free(decompBuf);
    return 66; // API sequence test completed successfully
}