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
//<ID> 365
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream istrm;
    z_stream copystrm;
    z_stream backstrm;
    memset(&istrm, 0, sizeof(istrm));
    memset(&copystrm, 0, sizeof(copystrm));
    memset(&backstrm, 0, sizeof(backstrm));
    unsigned char input[512];
    memset(input, 'Z', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong flags = zlibCompileFlags();
    (void)flags;

    // step 2: Compress source data
    uLong bound = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compDestLen = (uLongf)bound;
    compress2(compBuf, &compDestLen, (const Bytef *)input, inputLen, 6);

    // step 3: Inflate (standard) and copy stream
    memset(&istrm, 0, sizeof(istrm));
    inflateInit2_(&istrm, 15, zlibVersion(), (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compDestLen;
    Bytef * outBuf = (Bytef *)malloc((size_t)inputLen);
    memset(outBuf, 0, (size_t)inputLen);
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)inputLen;
    inflate(&istrm, 0);
    inflateCopy(&copystrm, &istrm);
    inflateEnd(&istrm);

    // step 4: Inflate back, write gz and cleanup
    unsigned char window[32];
    memset(window, 0, sizeof(window));
    memset(&backstrm, 0, sizeof(backstrm));
    inflateBackInit_(&backstrm, 15, window, zlibVersion(), (int)sizeof(z_stream));
    backstrm.next_in = compBuf;
    backstrm.avail_in = (uInt)compDestLen;
    Bytef * outBuf2 = (Bytef *)malloc((size_t)inputLen);
    memset(outBuf2, 0, (size_t)inputLen);
    backstrm.next_out = outBuf2;
    backstrm.avail_out = (uInt)inputLen;
    inflateBack(&backstrm, (in_func)0, (void *)0, (out_func)0, (void *)0);
    inflateBackEnd(&backstrm);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, compBuf, (unsigned int)compDestLen);
    gzflush(gz, 0);
    gzclose(gz);
    inflateEnd(&copystrm);
    free(compBuf);
    free(outBuf);
    free(outBuf2);
    return 66; // API sequence test completed successfully
}