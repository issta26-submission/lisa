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
//<ID> 203
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream dstrm;
    z_stream istrm;
    memset(&dstrm, 0, sizeof(dstrm));
    memset(&istrm, 0, sizeof(istrm));
    unsigned char input[512];
    memset(input, 'Z', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong compBound = compressBound(inputLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    uLong compLen = compBound;
    compress2(compBuf, &compLen, (const Bytef *)input, inputLen, 6);

    // step 2: Setup (initialize a deflater instance to exercise deflateInit_)
    deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));

    // step 3: Core operations (uncompress the compressed data, inflate a stream and sync)
    Bytef *uncompBuf = (Bytef *)malloc((size_t)inputLen * 2);
    memset(uncompBuf, 0, (size_t)inputLen * 2);
    uLongf destLen = (uLongf)inputLen * 2;
    uLong srcLen = compLen;
    uncompress2(uncompBuf, &destLen, compBuf, &srcLen);

    inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    Bytef *inflateOut = (Bytef *)malloc((size_t)inputLen * 2);
    memset(inflateOut, 0, (size_t)inputLen * 2);
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)srcLen;
    istrm.next_out = inflateOut;
    istrm.avail_out = (uInt)((uLong)inputLen * 2);
    inflate(&istrm, 0);
    inflateSync(&istrm);

    // step 4: Validate & Cleanup
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, (voidpc)compBuf, (unsigned int)compLen);
    gzclose(gz);
    deflateEnd(&dstrm);
    inflateEnd(&istrm);
    free(compBuf);
    free(uncompBuf);
    free(inflateOut);

    // API sequence test completed successfully
    return 66;
}